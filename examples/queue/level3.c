/*
* Level 3: The Async File Uploader: A web-simulator process puts "File Paths" into a queue.
* A worker process picks them up and "uploads" them (simulated by a sleep and a log).
* This handles "bursty" traffic without crashing the main process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>

#define QUEUE_KEY 12345

struct upload_t {
  long type;
  char path[1024];
};

void server (int msgid) {
  struct upload_t upload;

  while (msgrcv(msgid, &upload, sizeof(upload.path), 0, 0) != -1) {
    printf("uplading \"%s\" file...\n", upload.path);
    sleep(1);
    printf("file \"%s\" uploaded successfully.\n", upload.path);
  }

  perror("msgrcv");
  msgctl(msgid, IPC_RMID, NULL);

  exit(0);
}

void client (int msgid, char path[1024]) {
  struct upload_t upload;

  upload.type = 1;
  strcpy(upload.path, path);

  int result = msgsnd(msgid, &upload, sizeof(upload.path), 0);

  if (result == 0) {
    printf("file \"%s\" added to uploader queue.\n", upload.path);
  } else {
    perror("msgsnd");
  }
}

int main (int argc, char *argv[]) {
  int msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);

  if (strcmp(argv[1], "server") == 0) server(msgid);
  else if (strcmp(argv[1], "client") == 0) client(msgid, argv[2]);

  return 0;
}
