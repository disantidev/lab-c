#include <stdio.h>
#include <stdlib.h>

int compare (const void *a, const void *b) {
  return (*(const int *) a) - (*(const int *) b);
}

int **three_sum (int *nums, int numsSize) {
  int **sum;
  int sumSize = 0;
  
  qsort(nums, numsSize, sizeof(int), compare);

  sum = malloc(numsSize * sizeof(int *));

  for (int i = 0; i < numsSize - 2; i++) {
    if (i > 0 && nums[i] == nums[i-1]) continue;
    if (nums[i] > 0) break;

    int left = i + 1, right = numsSize - 1;

    while (left < right) {
      int s = nums[i] + nums[left] + nums[right];

      if (s > 0) {
        right--;
      } else if (s < 0) {
        left++;
      } else {
        sum[sumSize] = (int *) malloc(3 * sizeof(int));
        sum[sumSize][0] = nums[i];
        sum[sumSize][1] = nums[left];
        sum[sumSize][2] = nums[right];
        sumSize++;

        while (left < right && nums[left] == nums[left+1]) left++;
        while (left < right && nums[right] == nums[right-1]) right--;

        left++;
        right--;
      }
    }
  }

  return sum;
}

int main (void) {
  int nums[] = { 1, 0, 2, -2, -1, 2, -1, 2 };
  int numsSize = sizeof(nums) / sizeof(nums[0]);

  int **triplets = three_sum(nums, numsSize);

  while (*triplets) {
    int *r = *triplets;
    printf("[%d, %d, %d]\n", r[0], r[1], r[2]);
    triplets++;
  }

  return 0;
}


