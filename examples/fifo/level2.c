/*
* Level 2: The Request-Response Bridge: Create two FIFOs: request_pipe and reply_pipe.
* A server reads a string from the request pipe, reverses it, and writes it to the reply pipe.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

#define REQ_PATH "/tmp/math_req"
#define RES_PATH "/tmp/math_res"

char **parse_req (char *req) {
  int count = 0;
  char **args;
  char *token;

  args = malloc(5 * sizeof(char *));
  token = strtok(req, " ");

  while (token != NULL) {
    args[count++] = token;
    token = strtok(NULL, " ");
  }

  args[count] = NULL;

  return args;
}

int calc (char *op, int x, int y) {
  if (strcmp(op, "ADD") == 0) return x + y;
  else if (strcmp(op, "SUB") == 0) return x - y;
  else if (strcmp(op, "MUL") == 0) return x * y;
  else if (strcmp(op, "DIV") == 0) return x / y;
  return 0;
}

int get_random(int n) {
  return (rand() % n) + 1;
}

void server () {
  int req_fd, nread;
  char buf[100];

  if (access(REQ_PATH, F_OK) < 0) {
    mkfifo(REQ_PATH, 0666);
  }

  if ((req_fd = open(REQ_PATH, O_RDWR)) < 0) {
    perror("open");
    exit(1);
  }

  printf("Server listening on %s...\n", REQ_PATH);

  while ((nread = read(req_fd, buf, sizeof(buf) - 1)) > 0) {
    buf[nread] = '\0';
    
    printf("Received %s\n", buf);

    char **args = parse_req(buf);

    int res_fd;
    char res[100], res_path[100];

    char *op = args[1];
    int x = atoi(args[2]);
    int y = atoi(args[3]);

    int nres = snprintf(res, sizeof(res), "%d", calc(op, x, y));

    snprintf(res_path, sizeof(res_path), "%s%s", RES_PATH, args[0]);

    if (access(res_path, F_OK) < 0) {
      mkfifo(res_path, 0666);
    }

    if ((res_fd = open(res_path, O_WRONLY)) < 0) {
      perror("open");
      exit(1);
    }

    write(res_fd, res, nres);
    close(res_fd);
  }

  close(req_fd);
}

void client () {
  if (fork() == 0) {
    int req_fd, res_fd, nreq;
    char req[100], res[100];
    char res_path[100];

    snprintf(res_path, sizeof(res_path), "%s%d", RES_PATH, getpid());

    if (access(res_path, F_OK)) mkfifo(res_path, 0666);
    if (access(REQ_PATH, F_OK) < 0) mkfifo(REQ_PATH, 0666);

    if (
      (res_fd = open(res_path, O_RDWR)) < 0 ||
      (req_fd = open(REQ_PATH, O_WRONLY)) < 0
    ) {
      perror("open");
      exit(1);
    }

    srand(time(NULL));

    nreq = snprintf(req, sizeof(req), "%d %s %d %d", getpid(), "ADD", get_random(100), get_random(100));

    write(req_fd, req, nreq);

    read(res_fd, res, sizeof(res) - 1);

    printf("Response for %s = %s\n", req, res);

    close(res_fd);
    close(req_fd);
    exit(0);
  }

  wait(NULL);
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <client|server>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "client") == 0) client();
  else if (strcmp(argv[1], "server") == 0) server();

  return 0;
}

