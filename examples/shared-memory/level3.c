/*
* Level 3: The Multi-Process Hash Table: Implement a basic Hash Table in a shared memory segment.
* Multiple unrelated processes must be able to insert() and lookup() keys simultaneously using
* semaphores to lock specific "buckets" of the table.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define MAX_KEY_LENGTH 64
#define MAX_NODES 10

typedef struct {
  char  key[MAX_KEY_LENGTH];
  int   value;
  int   next_value;
} SharedNode;

typedef struct {
  int size;
  int capacity;
  SharedNode nodes[MAX_NODES];
} SharedHashTable;

unsigned long get_hash (char *key, int capacity) {
  unsigned long hash = 5381;

  int c;
  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % capacity;
}

void insert (SharedHashTable *table, char *key, int value) {
  unsigned long hash = get_hash(key, table->capacity);
  strcpy(table->nodes[hash].key, key);
  table->nodes[hash].next_value = table->nodes[hash].value;
  table->nodes[hash].value = value;
  table->size++;
}

int get (SharedHashTable *table, char *key) {
  unsigned long hash = get_hash(key, table->capacity);
  return table->nodes[hash].value;
}

int main (void) {
  SharedHashTable *table = mmap(NULL, sizeof(SharedHashTable), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  table->size = 0;
  table->capacity = MAX_NODES;
  
  sem_unlink("/table");
  sem_t *global_lock = sem_open("/table", O_CREAT | O_EXCL, 0666, 1);

  if (global_lock == SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }

  printf("Initial size: %d\n", table->size);

  for (int i = 0; i < 10; i++) {
    if (fork() == 0) {
      char key[MAX_KEY_LENGTH];
      for (int x = 0; x < 100000; x++) {
        memset(key, 0, MAX_KEY_LENGTH);
        sprintf(key, "idx%d", x * (i + 1));
        sem_wait(global_lock);
        insert(table, key, getpid());
        sem_post(global_lock);
      }
      sem_close(global_lock);
      exit(0);
    }
  }

  for (int i = 0; i < 10; i++) wait(NULL);

  printf("Final size: %d\n", table->size);

  char *key = "idx10000";
  int value = get(table, key);

  printf("Value for %s = %d\n", key, value);

  sem_close(global_lock);
  sem_unlink("/table");

  return 0;
}

