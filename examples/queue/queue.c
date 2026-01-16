#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>

struct msg_buffer {
  long msg_type;
  char msg_text[100];
};

int main (void) {
  int msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
  struct msg_buffer message;

  if (fork() == 0) {
    sleep(1);

    msgrcv(msgid, &message, sizeof(message.msg_text), 2, 0);
    printf("Received: %ld | %s\n", message.msg_type, message.msg_text);

    msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0);
    printf("Received: %ld | %s\n", message.msg_type, message.msg_text);

    exit(0);
  }

  message.msg_type = 1;
  strcpy(message.msg_text, "Normal task");
  msgsnd(msgid, &message, sizeof(message.msg_text), 0);

  message.msg_type = 2;
  strcpy(message.msg_text, "Urgent task");
  msgsnd(msgid, &message, sizeof(message.msg_text), 0);

  wait(NULL);
  msgctl(msgid, IPC_RMID, NULL);

  return 0;
}
