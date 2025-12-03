#include <stdio.h>
#include <stdlib.h>

char *get_status (char n) {
  return (n - '0') == 1 ? "\e[32mSUCCESS" : "\e[31mFAILED";
}

