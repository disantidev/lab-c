#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

void daemonize (const char *name) {
  int i, fd0, fd1, fd2;
  pid_t pid;
  struct rlimit rl;
  struct sigaction sa;

  pid = fork();

  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) exit(EXIT_SUCCESS);

  if (setsid() < 0) exit(EXIT_FAILURE);

  pid = fork();

  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) exit(EXIT_SUCCESS);

  umask(0);

  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
    perror("getrlimit");
    exit(EXIT_FAILURE);
  }

  if (chdir("/") < 0) {
    perror("chdir");
    exit(EXIT_FAILURE);
  }

  if (rl.rlim_max == RLIM_INFINITY) {
    rl.rlim_max = 1024;
  }

  for (i = 0; i < rl.rlim_max; i++) {
    close(i);
  }

  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  openlog(name, LOG_PID | LOG_CONS, LOG_DAEMON);

  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_ERR, "Unexpected file descriptors %d %d %d", fd0, fd1, fd2);
    exit(EXIT_FAILURE);
  }

  syslog(LOG_INFO, "Daemon started successfully");

  closelog();
}

