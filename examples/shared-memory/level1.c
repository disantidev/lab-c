/*
* Level 1: The Shared Counter: Create two processes that both try to increment
* a single integer in shared memory 1,000,000 times. Use a Semaphore to prove
* that without it, the final count is wrong (Race Condition), and with it,
* the count is exactly 2,000,000.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

int main (void) {
  const int size = sizeof(int);

  int shm_fd;
  char *shm_name = "shmtest";
  sem_t *sem;

  if ((sem = sem_open("semtest", O_CREAT, 0666, 1)) == SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }

  if ((shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666)) == -1) {
    perror("shm_open");
    exit(1);
  }

  ftruncate(shm_fd, size);

  int *count = (int *) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  *count = 0;

  if (fork() == 0) {
    for (int i = 0; i < 1000000; i++) {
      sem_wait(sem);
      (*count)++;
      sem_post(sem);
    }
    exit(0);
  }

  if (fork() == 0) {
    for (int i = 0; i < 1000000; i++) {
      sem_wait(sem);
      (*count)++;
      sem_post(sem);
    }
    exit(0);
  }

  wait(NULL);
  wait(NULL);

  printf("Count: %d\n", *count);

  munmap(count, size);
  shm_unlink(shm_name);

  return 0;
}

