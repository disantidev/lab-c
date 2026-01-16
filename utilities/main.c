#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_handler(int);

int main (void)
{
  signal(SIGINT, sig_handler);
  signal(SIGALRM, sig_handler);

  alarm(5);

  for (;;)
    pause();

  return 0;
}

static void sig_handler(int signo)
{
  printf("Received signo %d\n", signo);
}
