#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <sys/wait.h>

int main (void)
{
  pid_t pid;

  if ((pid = fork()) < 0) {
    perror("fork");
    return 1;
  } else if (pid == 0) {
    printf("Child process first message\n");
    printf("Child process second message");
  } else {
    printf("Parent process first message\n");
    printf("Parent process second message\n");
    printf("Parent process final message\n");
  }

  return 0;
}

