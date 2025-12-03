#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char get_strobonum (char num);
int is_strobogrammatic (const char num[]);
void test_is_strobogrammatic (void);

int main (void) {
  test_is_strobogrammatic();
  return 0;
}

char get_strobonum (char num) {
  switch (num) {
    case '0': return '0';
    case '1': return '1';
    case '6': return '9';
    case '8': return '8';
    case '9': return '6';
    default: return ' ';
  }
}

int is_strobogrammatic (const char num[]) {
  int left = 0, right = strlen(num) - 1;

  while (left <= right) {
    char charL = num[left], charR = num[right];
    char sCharL = get_strobonum(charL), sCharR = get_strobonum(charR);

    if (sCharL == ' ' || sCharR == ' ') return 0;
    if (charL != sCharR) return 0;

    left++;
    right--;
  }

  return 1;
}

void test_is_strobogrammatic () {
  const char *const nums[][2] = {
    { "1", "1" },
    { "1", "1" },
    { "8", "1" },
    { "88", "1" },
    { "89", "0" },
    { "888", "1" },
    { "000", "1" },
    { "111", "1" },
    { "123", "0" },
    { "1234", "0" },
    { "69", "1" },
    { "696", "0" },
    { "6969", "1" },
    { "808", "1" },
    { "818", "1" }
  };

  const int numsSize = sizeof(nums) / sizeof(nums[0]);

  for (int i = 0; i < numsSize; i++) {
    printf(
      "Testing if %s %s strobogrammatic. %s\e[0m\n",
      nums[i][0],
      nums[i][1][0] == '1' ? "is" : "is not",
      (nums[i][1][0] - '0') == is_strobogrammatic(nums[i][0]) ? "\e[32mSUCCESS" : "\e[31mFAILED"
    );
  }
}









