#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sig_handler(int);

int main (int argc, char *argv[])
{
  int stat;
  pid_t pid = fork();

  if (pid == 0) {
    char *args[] = {"node", "main.js", NULL};
    execvp(args[0], args);
    perror("execvp failed");
    exit(1);
  } else if (pid > 0) {
    wait(&stat);

    if (WIFEXITED(stat)) {
      printf("exit status: %d\n", WEXITSTATUS(stat));
    } else if (WIFSIGNALED(stat)) {
      psignal(WTERMSIG(stat), "exit signal");
    }

    printf("child has ended\n");
  } else {
    perror("fork failed");
  }

  exit(0);
}

