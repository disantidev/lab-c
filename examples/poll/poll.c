#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

int main () {
  int fifo_fd;
  char buffer[1024];
  struct pollfd fds[2];

  if (access("my_fifo", F_OK) < 0) {
    mkfifo("my_fifo", 0666);
  }

  fifo_fd = open("my_fifo", O_RDONLY | O_NONBLOCK);

  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  fds[1].fd = fifo_fd;
  fds[1].events = POLLIN;

  while (1) {
    int ret = poll(fds, 2, -1);

    if (ret > 0) {
      if (fds[0].revents & POLLIN) {
        fgets(buffer, sizeof(buffer), stdin);
        ssize_t bytes = strcspn(buffer, "\n");
        buffer[bytes] = '\0';
        printf("stdin %s\n", buffer);
        buffer[0] = '\0';
      }

      if (fds[1].revents & POLLIN) {
        ssize_t bytes = read(fifo_fd, buffer, sizeof(buffer) - 1);

        if (bytes > 0) {
          buffer[bytes] = '\0';
          printf("fifo %s\n", buffer);
          buffer[0] = '\0';
        } else if (bytes == 0) {
          close(fifo_fd);
          fifo_fd = open("my_fifo", O_RDONLY | O_NONBLOCK);
          fds[1].fd = fifo_fd;
        }
      }
    }
  }

  close(fifo_fd);

  return 0;
}
