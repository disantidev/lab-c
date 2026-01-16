/*
* Level 3: The Mini-Shell: Write a program that parses a user string like ls -l | sort | uniq
* and manually sets up the pipe() and dup2() calls to make the execution work just like a real bash shell.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_TOKENS 64

void trim (char *str) {
  if (str == NULL) return;

  int index = 0, left = 0, right = strlen(str) - 1;

  while (left < right && isspace((unsigned char) str[left])) left++;
  while (left < right && isspace((unsigned char) str[right])) right--;
  for (; left <= right; index++) str[index] = str[left++];

  str[index] = '\0';
}

char **parse_string (char *str, char *delim) {
  int capacity = MAX_TOKENS, count = 0;
  char **argv, *token;

  if (str == NULL) return argv;

  argv = (char **) malloc(capacity * sizeof(char *));
  token = strtok(str, delim);

  while (token != NULL) {
    trim(token);
    argv[count++] = token;

    if (count >= capacity) {
      capacity += MAX_TOKENS;
      argv = realloc(argv, capacity * sizeof(char *));
    }

    token = strtok(NULL, delim);
  }

  argv[count] = NULL;

  return argv;
}

int main (void) {
  char line[1024];

  while (fgets(line, sizeof(line), stdin)) {
    line[strcspn(line, "\n")] = '\0';

    int pp[2], fdin = 0;

    char **cmds = (char **) parse_string(line, "|");

    for (int i = 0; cmds[i] != NULL; i++) {
      if (cmds[i+1] != NULL) {
        if (pipe(pp) == -1) {
          perror("pipe");
          exit(EXIT_FAILURE);
        }
      }

      char **argv = parse_string(cmds[i], " ");

      if (fork() == 0) {
        if (fdin != 0) {
          dup2(fdin, STDIN_FILENO);
          close(fdin);
        }

        if (cmds[i+1] != NULL) {
          dup2(pp[1], STDOUT_FILENO);
          close(pp[0]);
          close(pp[1]);
        }

        close(fdin);
        close(pp[1]);

        execvp(argv[0], argv);
        perror("execvp");
        exit(EXIT_FAILURE);
      }

      if (fdin != 0) close(fdin);
      if (cmds[i+1] != NULL) {
        close(pp[1]);
        fdin = pp[0];
      }

      wait(NULL);

      free(argv);
    }

    free(cmds);
  }

  return EXIT_SUCCESS;
}
