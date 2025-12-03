#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

Node *create_nodes(int *nums, int numsSize) {
  Node *head = NULL;
  Node *last = NULL; 

  for (int i = numsSize - 1; i > 0; i--) {
    Node *node = NULL;
    node = malloc(sizeof(Node *));
    node->value = nums[i];
    node->next = last;
    last = node;
  }

  head = malloc(sizeof(Node *));
  head->value = nums[0];
  head->next = last;

  return head;
}

Node *remove_nth_last_node (Node *node, int nth) {
  Node *left = node;
  Node *right = node;

  for (int i = 0; i < nth; i++) {
    right = right->next;
  }

  if (!right) {
    return node->next;
  }

  while (right->next != NULL) {
    right = right->next;
    left = left->next;
  }

  left->next = left->next->next;

  return node;
}

int main (void) {
  Node *head;
  int nums[] = { 11, 13, 15, 17, 19 };
  int numsSize = sizeof(nums) / sizeof(nums[0]);

  head = create_nodes(nums, numsSize);
  head = remove_nth_last_node(head, 2);

  Node *current;
  current = head;

  while (current) {
    printf("%d\n", current->value);
    current = current->next;
  }

  return 0;
}
