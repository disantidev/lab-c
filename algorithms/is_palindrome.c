#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "util.h"

int is_palindrome (const char str[]);
void test_is_palindrome ();

int main (void) {
  test_is_palindrome();
  return 0;
}

int is_palindrome (const char str[]) {
  int left = 0, right = strlen(str) - 1;

  while (left < right) {
    while (left < right && !isalnum(str[left])) left++;
    while (left < right && !isalnum(str[right])) right--;

    if (tolower(str[left]) != tolower(str[right])) return 0;

    left++;
    right--;
  }

  return 1;
}

void test_is_palindrome (void) {
  const char *const phrases[][2] = {
    {"A man, a plan, a canal: Panama!", "1"},
    {"No ‘x’ in Nixon.", "1"},
    {"Racecar", "1"},
    {"Was it a car or a cat I saw?", "1"},
    {"Madam, in Eden, I'm Adam.", "1"},
    {"I am Sam", "0"},
    {"The quick brown fox", "0"},
    {"Hello world", "0"},
    {"Coding is fun", "0"},
    {"A big red apple", "0"}
  };

  const int size = sizeof(phrases) / sizeof(phrases[0]);

  for (int i = 0; i < size; i++) {
    printf(
      "Testing if \"%s\" is a palindrome. %s\n\e[0m",
      phrases[i][0],
      get_status(phrases[i][1][0])
    );
  }
}
