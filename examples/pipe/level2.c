/*
* Level 2: The Parallel Math Engine: A parent process forks 4 children.
* The parent sends a large array of numbers through a pipe. Each child reads
* a portion, calculates the square root, and pipes the result back.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main () {
  double nums[] = { 4.0, 9.0, 16.0, 25.0, 36.0, 49.0, 64.0, 81.0 };
  int count = sizeof(nums) / sizeof(double);
  int mid = count / 2;
  int p1[2], p2[2];

  if (pipe(p1) == -1 || pipe(p2) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (fork() == 0) {
    char buf[10];
    for (int i = 0; i < mid; i++) {
      int n = snprintf(buf, sizeof(buf), "%.0f ", sqrt(nums[i]));
      write(p1[1], buf, n);
    }
    exit(EXIT_SUCCESS);
  }

  if (fork() == 0) {
    char buf[10];
    for (int i = mid; i < count; i++) {
      int n = snprintf(buf, sizeof(buf), "%.0f ", sqrt(nums[i]));
      write(p2[1], buf, n);
    }
    exit(EXIT_SUCCESS);
  }

  close(p1[1]); close(p2[1]);

  char buf[10];
  int n;

  while ((n = read(p1[0], buf, sizeof(buf))) > 0)
    write(STDOUT_FILENO, buf, n);
  while ((n = read(p2[0], buf, sizeof(buf))) > 0)
    write(STDOUT_FILENO, buf, n);

  wait(NULL); wait(NULL);

  return 0;
}
