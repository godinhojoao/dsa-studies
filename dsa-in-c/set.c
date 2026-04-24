/*
Deleting the executable after running:
gcc set.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET_LIMIT_SIZE 1000

/*
-> Using a hashtable and handling collision with linked list, not handling load factor (fixed size for buckets)

m = number of buckets (in this case fixed SET_LIMIT_SIZE)
k = elements in one bucket
n = all elements (in all buckets)

Current worst cases:
insert O(k)
find O(k)
remove O(k)
iterate O(m + n)
isEmpty O(1)

Current average cases:
insert O(1)
find O(1)
remove O(1)
iterate O(m + n)  // O(n) if m is constant as it is
isEmpty O(1)

Possible improvements:
- I'm not handling load factor with rehash logic for simplicity, fixed size. -> a good improvement to allow dynamic
sizes (shrink and grow)

Possible changes and its trade-offs:
- Using a balanced BST instead of a hashtable would get faster worst case (but slower avg case):
  - worst case: insert, find, remove
  -- from O(n) hashtable to O(log n) balanced BST
  - avg case: insert, find, remove
  -- from O(1) hashtable to O(log n) balanced BST

- A hashtable is generally better for unordered sets, because it provides constant-time average performance and does not maintain order.
- A balanced BST is preferable only when worst-case guarantees or sorted order are more important than average performance.
--> A set is an unordered data structure, so using a hashtable is preferable.
*/

typedef struct SetNode {
  struct SetNode* next;
  int value;
} SetNode;

typedef struct Set {
  SetNode** nodes;
  int currLength;
} Set;

// 32-bit FNV-1a https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
unsigned int fnv1a_int(int value) {
  unsigned int hash = 2166136261u;
  unsigned char* p = (unsigned char*)&value;

  for(int i = 0; i < (int)sizeof(int); i++) {
    hash ^= (unsigned int)p[i];
    hash *= 16777619u;
  }

  return hash;
}

int itemIndex(int value, int arrLimit) {
  unsigned int hash = fnv1a_int(value);
  return hash % arrLimit;
}

Set* initializeSet() {
  Set* set = malloc(sizeof(Set));
  set->currLength = 0;
  set->nodes = malloc(sizeof(SetNode*) * SET_LIMIT_SIZE);

  for(int i = 0; i < SET_LIMIT_SIZE; i++) {
    set->nodes[i] = NULL;
  }

  return set;
}

SetNode* createNode(int value) {
  SetNode* newNode = malloc(sizeof(SetNode));
  newNode->next = NULL;
  newNode->value = value;
  return newNode;
}

void insert(Set* set, int value) {
  int index = itemIndex(value, SET_LIMIT_SIZE);
  SetNode* currentNode = set->nodes[index];

  SetNode* prevNode = NULL;
  while(currentNode != NULL) {
    if(currentNode->value == value) {
      printf("set do not allow repeated values: %d\n", value);
      return;
    }
    prevNode = currentNode;
    currentNode = currentNode->next;
  }

  SetNode* newNode = createNode(value);
  set->currLength += 1;

  if(prevNode == NULL) {
    set->nodes[index] = newNode;
  } else {
    prevNode->next = newNode;
  }
}

int isEmpty(Set* set) {
  return set->currLength == 0;
}

SetNode* find(Set* set, int value) {
  if(isEmpty(set))
    return NULL;

  int index = itemIndex(value, SET_LIMIT_SIZE);
  SetNode* currNode = set->nodes[index];

  while(currNode != NULL && currNode->value != value) {
    currNode = currNode->next;
  }

  return currNode;
}

int removeItem(Set* set, int value) {
  if(isEmpty(set))
    return -1;

  int index = itemIndex(value, SET_LIMIT_SIZE);
  SetNode* currentNodeToDelete = set->nodes[index];

  SetNode* prevNode = NULL;
  while(currentNodeToDelete != NULL && currentNodeToDelete->value != value) {
    prevNode = currentNodeToDelete;
    currentNodeToDelete = currentNodeToDelete->next;
  }

  if(currentNodeToDelete == NULL)
    return -1;

  if(prevNode != NULL) {
    prevNode->next = currentNodeToDelete->next;
  } else {
    set->nodes[index] = currentNodeToDelete->next;
  }

  free(currentNodeToDelete);
  set->currLength -= 1;

  return 1;
}

int main() {
  Set* set = initializeSet();

  printf("== insert 10, 20, 30, 10 ==\n");
  insert(set, 10);
  insert(set, 20);
  insert(set, 30);
  insert(set, 10);  // duplicate test

  printf("== find ==\n");
  SetNode* n1 = find(set, 10);
  printf("find 10: %s\n", n1 ? "found" : "not found");

  SetNode* n2 = find(set, 99);
  printf("find 99: %s\n", n2 ? "found" : "not found");

  printf("== remove ==\n");
  printf("remove 99: %d\n", removeItem(set, 99));
  printf("remove 20: %d\n", removeItem(set, 20));
  printf("remove 10: %d\n", removeItem(set, 10));
  printf("remove 10 again: %d\n", removeItem(set, 10));

  printf("== final finds ==\n");
  printf("find 10: %s\n", find(set, 10) ? "found" : "not found");
  printf("find 20: %s\n", find(set, 20) ? "found" : "not found");
  printf("find 30: %s\n", find(set, 30) ? "found" : "not found");

  return 0;
}
