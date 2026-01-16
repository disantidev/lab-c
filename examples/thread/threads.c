#include <stdio.h>
#include <pthread.h>

void *thr_fn (void *arg) {
  printf("thr_fn %s\n", (char *) arg);
  return 0;
}

int main (void) {
  int err;
  pthread_t tid1, tid2;

  if ((err = pthread_create(&tid1, NULL, thr_fn, "1")) != 0) {
    perror("pthread_create error 1");
    return 1;
  }

  if ((err = pthread_create(&tid2, NULL, thr_fn, "2")) != 0) {
    perror("pthread_create error 2");
    return 1;
  }

  (void) pthread_join(tid1, NULL);
  (void) pthread_join(tid2, NULL);

  return 0;
}

