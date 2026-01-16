/*
* Level 2: The Heartbeat Monitor: A "Monitor" process expects a message from 3 different "Worker"
* processes every 2 seconds. If a worker fails to send its message to the queue, the Monitor declares
* that process "Dead."
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/msg.h>

#define MAX_WORKERS 3

struct beat_t {
  long mtype;
  char mtext[20];
};

void worker (int msgid) {
  struct beat_t beat;

  beat.mtype = getpid();
  sprintf(beat.mtext, "alive");

  for (;;) {
    sleep(1);
    msgsnd(msgid, &beat, sizeof(beat.mtext), 0);
    printf("worker: %d | heartbeat sent\n", (int) getpid());
  }

  exit(0);
}

void monitor (int msgid) {
  struct beat_t beat;

  printf("monitor starting...\n");
  sleep(3);

  for (;;) {
    ssize_t result = msgrcv(msgid, &beat, sizeof(beat.mtext), 0, IPC_NOWAIT);

    if (result > 0) {
      printf("monitor: [ok] received heartbeat from pid %ld\n", beat.mtype);
    } else if (errno == ENOMSG) {
      printf("monitor: [error] no heartbeats. system idle...\n");
    } else {
      perror("msgrcv");
    }

    usleep(500000);
  }
}

int main (void) {
  srand(time(NULL));
  
  int msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);

  for (int i = 0; i < MAX_WORKERS; i++) {
    if (fork() == 0) worker(msgid);
  }

  monitor(msgid);

  msgctl(msgid, IPC_RMID, NULL);

  return 0;
}

