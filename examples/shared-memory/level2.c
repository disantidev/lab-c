/*
* Level 2: The Video Frame Buffer: Process A "renders" a 2D ASCII art frame into shared memory.
* Process B "displays" it. Use semaphores to ensure Process B never starts drawing until Process A
* has finished the entire frame (Double Buffering logic).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

int main (void) {
  int shm_id, shm_size = 4096;
  char *sem_name = "/vfb_sem", *shm_name = "/vfb_shm";
  sem_t *sem_id;

  sem_id = sem_open(sem_name, O_CREAT, 0666, 0);
  if (sem_id == SEM_FAILED) { perror("sem_open"); exit(1); }

  shm_id = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
  if (shm_id == -1) { perror("shm_open"); exit(1); }

  ftruncate(shm_id, shm_size);

  char *frame = (char *) mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);

  if (fork() == 0) {
    char buffer[shm_size];
    memset(buffer, 0, shm_size);

    for (int i = 1; i < 11; i++) {
      char line[64];
      sprintf(line, "#frame line %d\n", i);
      strcat(buffer, line);
      sleep(1);
    }

    memcpy(frame, buffer, shm_size);
    sem_post(sem_id);
    exit(0);
  }

  if (fork() == 0) {
    sem_wait(sem_id);
    printf("%s", frame);
    fflush(stdout);
    exit(0);
  }

  wait(NULL); wait(NULL);

  munmap(frame, shm_size);
  shm_unlink(shm_name);
  sem_close(sem_id);
  sem_unlink(sem_name);

  return 0;
}

