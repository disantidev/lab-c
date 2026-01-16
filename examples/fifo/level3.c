/*
* Level 3: The Chat Room: Build a primitive chat system where every user writes to a common GLOBAL_FIFO,
* and a "Broadcast Server" reads from it and sends the text to each user’s private individual FIFO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#define GLOBAL_FIFO "/tmp/global_fifo_chat"
#define OPERATOR_SIZE 256
#define USERNAME_SIZE 256
#define MESSAGE_SIZE 1024
#define COMMAND_SIZE 2048
#define MAX_CLIENTS 100

void clear_last_line () {
  printf("\x1b[1F");
  printf("\x1b[2K");
  fflush(stdout);
}

int open_fifo (char *path, int oflag) {
  int fd;

  if (access(path, F_OK) < 0) {
    mkfifo(path, 0666);
  }

  if ((fd = open(path, oflag)) < 0) {
    fprintf(stderr, "open %s: %s\n", path, strerror(errno));
    exit(1);
  }

  return fd;
}

void connect (int fd, char *username) {
  char command[1024];
  int length = snprintf(command, sizeof(command), "CONNECT|%s", username);
  write(fd, command, length);
}

void disconnect (int fd, char *username) {
  char command[1024];
  int length = snprintf(command, sizeof(command), "DISCONNECT|%s", username);
  write(fd, command, length);
}

void send_message (int fd, char *username, char *message) {
  char command[1024];
  int length = snprintf(command, sizeof(command), "MESSAGE|%s|%s", username, message); 
  write(fd, command, length);
}

char **parse_command (char *command) {
  int count = 0;
  char *message = strdup(command);
  char **args = malloc(10 * sizeof(char *));
  char *token = strtok(message, "|");

  while (token != NULL) {
    args[count++] = token;
    token = strtok(NULL, "|");
  }

  args[count] = NULL;

  return args;
}

void server () {
  int   users_count = 0;
  char  **users = malloc(MAX_CLIENTS * sizeof(char *));
  int   global_fd = open_fifo(GLOBAL_FIFO, O_RDONLY | O_NONBLOCK);

  printf("Server listening on %s...\n", GLOBAL_FIFO);

  struct pollfd fds[1];

  fds[0].fd = global_fd;
  fds[0].events = POLLIN;

  while (1) {
    int ret = poll(fds, 1, -1);

    if (ret > 0) {
      if (fds[0].revents & POLLIN) {
        char command[1024];
        ssize_t bytes = read(fds[0].fd, command, sizeof(command) - 1);

        if (bytes > 0) {
          printf("Received %s\n", command);

          char **args = parse_command(command);

          if (strcmp(args[0], "CONNECT") == 0) {
            users[users_count++] = args[1];
          } else if (strcmp(args[0], "DISCONNECT") == 0) {
            int user_index = -1;

            for (int i = 0; i < users_count; i++) {
              if (strcmp(users[i], args[1]) == 0) {
                user_index = i;
              }

              if (user_index > -1) {
                users[i] = users[i+1];
              }
            }
          } else if (strcmp(args[0], "MESSAGE") == 0) {
            for (int i = 0; i < users_count; i++) {
              char user_fifo[1024];
              snprintf(user_fifo, sizeof(user_fifo), "/tmp/%s_fifo_chat", users[i]);
              int user_fd = open_fifo(user_fifo, O_WRONLY | O_NONBLOCK);
              write(user_fd, command, strlen(command));
              close(user_fd);
            }
          }

          free(args);
        } else if (bytes == 0) {
          close(global_fd);
          global_fd = open_fifo(GLOBAL_FIFO, O_RDONLY | O_NONBLOCK);
          fds[0].fd = global_fd;
        }

        memset(command, '\0', sizeof(command));
      }
    }
  }

  exit(0);
}

void client () {
  int   global_fd, user_fd;
  char  username[1024];
  char  user_fifo[1024];
  struct pollfd fds[2];

  printf("username> ");
  fgets(username, sizeof(username), stdin);
  int length = strcspn(username, "\n");
  username[length] = '\0';

  snprintf(user_fifo, sizeof(user_fifo), "/tmp/%s_fifo_chat", username);

  global_fd = open_fifo(GLOBAL_FIFO, O_WRONLY | O_NONBLOCK);
  user_fd = open_fifo(user_fifo, O_RDONLY | O_NONBLOCK);

  connect(global_fd, username);

  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  fds[1].fd = user_fd;
  fds[1].events = POLLIN;

  clear_last_line();
  printf("<%s> ", username);
  fflush(stdout);

  while(1) {
    int ret = poll(fds, 2, -1);

    if (ret > 0) {
      if (fds[0].revents & POLLIN) {
        char message[1024];
        fgets(message, sizeof(message), stdin);
        size_t length = strcspn(message, "\n");
        message[length] = '\0';

        if (strcmp(message, ".exit") == 0) {
          disconnect(global_fd, username);
          printf("Disconnected.\n");
          break;
        }

        send_message(global_fd, username, message);
        printf("<%s> ", username);
        fflush(stdout);
      }

      if (fds[1].revents & POLLIN) {
        char command[1024];
        ssize_t bytes = read(fds[1].fd, command, sizeof(command) - 1);

        if (bytes > 0) {
          char **args = parse_command(command);

          if (strcmp(args[1], username)) {
            printf("\n<%s> %s\n", args[1], args[2]);
            printf("<%s> ", username);
            fflush(stdout);
          }
        } else if (bytes == 0) {
          close(user_fd);
          user_fd = open_fifo(user_fifo, O_RDONLY | O_NONBLOCK);
          fds[1].fd = user_fd;
        }

        memset(command, '\0', sizeof(command));
      }
    }
  }

  close(global_fd);
  close(user_fd);
  exit(0);
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <server|client>\n", argv[0]);
    exit(0);
  }

  if (strcmp(argv[1], "server") == 0) server();
  else if (strcmp(argv[1], "client") == 0) client();
  return 0;
}

