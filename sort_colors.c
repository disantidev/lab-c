#include <stdio.h>
#include <stdlib.h>

void swap (int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

void sort_colors (int *colors, int colorsSize) {
  int left = 0, mid = 0, high = colorsSize - 1;

  while (mid < high) {
    if (colors[mid] == 0) {
      swap(&colors[mid], &colors[left]);
      left++;
      mid++;
    } else if (colors[mid] == 1) {
      mid++;
    } else {
      swap(&colors[mid], &colors[high]);
      high--;
    }
  }
}

int main (void) {
  int colors[] = { 1, 0, 2, 1, 2, 2 };
  int colorsSize = sizeof(colors) / sizeof(colors[0]);

  sort_colors(colors, colorsSize);

  for (int i = 0; i < colorsSize; i++) {
    printf("%d ", colors[i]);
  }

  return 0;
}
