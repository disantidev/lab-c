/*
* Level 1: The Centralized Logger: Create a background "Logger" daemon that reads from a FIFO
* at /tmp/my_log. Write a separate "Client" program. If you run 5 instances of the client at once,
* ensure the logger prints all their messages without interleaving lines.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define LOG_NAME "/tmp/my_log"

void server () {
  char buf[100];
  int fd, nread;

  if (access(LOG_NAME, F_OK) == -1) {
    mkfifo(LOG_NAME, 0666);
  }

  if ((fd = open(LOG_NAME, O_RDWR)) < 0) {
    perror("open");
    exit(1);
  }

  printf("Server listening on %s...\n", LOG_NAME);

  while ((nread = read(fd, buf, sizeof(buf) - 1)) > 0) {
    buf[nread] = '\0';
    printf("Logged: %s", buf);
  }

  close(fd);
}

void client () {
  for (int i = 0; i < 5; i++) {
    if (fork() == 0) {
      int fd;
      char buf[100];
      int nbuf = snprintf(buf, sizeof(buf), "Message from child %d (PID: %d)\n", i, getpid());

      if ((fd = open(LOG_NAME, O_WRONLY)) < 0) {
        perror("open");
        exit(1);
      }

      write(fd, buf, nbuf);
      close(fd);
      exit(0);
    }
  }

  for (int i = 0; i < 5; i++)
    wait(NULL);
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <client|server>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "client") == 0) {
    client();
  } else if (strcmp(argv[1], "server") == 0) {
    server();
  }

  return 0;
}

