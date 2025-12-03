#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int is_num (char chr) {
  return chr > '0' && chr <= '9';
}

int valid_word_abbrevation (char *word, int wordSize, char *abbr, int abbrSize) {
  int wl = 0, al = 0;

  while (wl < wordSize && al < abbrSize) {
    if (word[wl] == abbr[al]) {
      wl++;
      al++;
      continue;
    } else if (is_num(abbr[al])) {
      int n = abbr[al] - '0';

      while (al < abbrSize && is_num(abbr[al+1])) {
        int nn = abbr[al+1] - '0';
        n = n * 10 + nn;
        al++;
      }

      for (int i = 0; i < n; i++) {
        if (wl < wordSize && word[wl]) wl++;
        else return 1;
      }

      al++;
    } else {
      return 1;
    }
  }

  return wl == wordSize && al == abbrSize ? 0 : 1;
}

void test_valid_word_abbrevation () {
  int size = 4;

  char words[4][256] = {
    "innovation",
    "mindset",
    "leadership",
    "holy craaaaaaaaaaaaaaaaaap"
  };

  char abbrs[4][256] = {
    "in5ion",
    "mi3et",
    "lead04ip",
    "holy cr18p"
  };

  int expected[] = { 0, 0, 1, 0 };

  for (int i = 0; i < size; i++) {
    printf("Testing if %s is abbr of %s...\n", abbrs[i], words[i]);
    assert(valid_word_abbrevation(words[i], strlen(words[i]), abbrs[i], strlen(abbrs[i])) == expected[i]);
  }

  printf("Tests are passed!\n");
}

int main (void) {
 test_valid_word_abbrevation();

  return 0;
}
