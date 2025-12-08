#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

// s = aabbcc
// left 0, right 5
//
// [a]abbc[c]
//  [a] != [c]
//    [a](a)bbc[c]
//      (a) >< [c]
//      moves++
//
// [a]cbbc[a]
//  left++
//  right--
//
// a[c]bb[c]a
//  left++
//  right--
//
// ac[b][b]ca
//  left++
//  right--
//

void swap (char *a, char *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int main (void) {
  char s[] = "skwhhaaunskegmdtutlgtteunmuuludii";
  int len = strlen(s);
  int left = 0, right = len - 1, moves = 0;

  while (left < right) {
    if (s[left] == s[right]) {
      left++;
      right--;
      continue;
    }

    int index = right;

    while (index > left && s[index] != s[left]) {
      index--;
    }

    if (index == left) {
        swap(&s[left], &s[left + 1]);
        moves++;
    } else {
      while (index < right) {
        swap(&s[index], &s[index + 1]);
        index++;
        moves++;
      }

      left++;
      right--;
    }
  }

  printf("Palindrome: %s\n", s);
  printf("Moves: %d\n", moves);

  return 0;
}




