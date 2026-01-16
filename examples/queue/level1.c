/*
* Level 1: The Print Spooler: Create a system where "Job" messages have different priorities (1 for low, 10 for high).
* Ensure the consumer process always pulls the Priority 10 jobs off the queue first, even if they arrived later.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>

struct job_t {
  long mtype;
  char mtext[100];
};

int main () {
  struct job_t job;

  int msgid = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);

  if (fork() == 0) {
    sleep(1);

    msgrcv(msgid, &job, sizeof(job.mtext), 10, 0);
    printf("%s", job.mtext);

    msgrcv(msgid, &job, sizeof(job.mtext), 1, 0);
    printf("%s", job.mtext);

    exit(0);
  }

  job.mtype = 1;
  strcpy(job.mtext, "World");
  msgsnd(msgid, &job, sizeof(job.mtext), 0);

  job.mtype = 10;
  strcpy(job.mtext, "Hello, ");
  msgsnd(msgid, &job, sizeof(job.mtext), 0);

  wait(NULL);
  msgctl(msgid, IPC_RMID, NULL);

  return 0;
}
