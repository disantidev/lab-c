#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap (char *a, char *b) {
  char t = *a;
  *a = *b;
  *b = t;
}

void remove_extra_spaces (char *str, int strSize) {
  int i = 0, j = 0, s = 0;

  while (str[i] == ' ') i++;

  for (; i < strSize; i++) {
    if (str[i] != ' ') {
      str[j++] = str[i];
      s = 0;
    } else if (str[i] == ' ' && s == 0) {
      str[j++] = str[i];
      s = 1;
    }
  }

  if (j > 0 && s == 1) str[j-1] = '\0';
  else str[j] = '\0';
}

void reverse_string (char *str, int start, int end) {
  int left = start, right = end;

  while (left < right) {
    swap(&str[left], &str[right]);
    left++;
    right--;
  }
}

void reverse_words (char *str, int strSize) {
  int left = 0;

  for (int i = 0; i <= strSize; i++) {
    if (str[i] == ' ' || str[i] == '\0') {
      reverse_string(str, left, i - 1);
      left = i + 1;
    }
  }
}

int main (void) {
  char str[] = "     Reverse    this  string         ";
  int strSize = strlen(str);

  remove_extra_spaces(str, strlen(str));
  reverse_string(str, 0, strlen(str) - 1);
  reverse_words(str, strlen(str));

  printf("Reversed: %s\n", str);

  return 0;
}
