/*
* Level 1: The Multi-Stage Filter: Create a program that forks two children.
* The first child runs cat on a file, pipes it to the second child which runs grep,
* which then pipes the final output back to the parent to be printed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
  int p1[2], p2[2];

  if (pipe(p1) == -1 || pipe(p2) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (fork() == 0) {
    dup2(p1[1], STDOUT_FILENO);
    close(p1[0]); close(p1[1]);
    close(p2[0]); close(p2[1]);
    execlp("cat", "cat", "/etc/passwd", NULL);
    exit(EXIT_FAILURE);
  }

  if (fork() == 0) {
    dup2(p1[0], STDIN_FILENO);
    dup2(p2[1], STDOUT_FILENO);
    close(p1[0]); close(p1[1]);
    close(p2[0]); close(p2[1]);
    execlp("grep", "grep", "root", NULL);
    exit(EXIT_FAILURE);
  }

  close(p1[0]); close(p1[1]);
  close(p2[1]);

  char buffer[1024];
  ssize_t bytes_read;

  while ((bytes_read = read(p2[0], buffer, sizeof(buffer))) > 0) {
    write(STDOUT_FILENO, buffer, bytes_read);
  }

  close(p2[0]);
  wait(NULL);
  wait(NULL);

  return EXIT_SUCCESS;
}
