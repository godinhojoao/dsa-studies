#include <stdio.h>
#include <stdlib.h>

struct ListNode {
  int val;
  struct ListNode* next;
};

// k = amount of lists
// n = each list item
struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
  if(listsSize == 0) return NULL;
  if(listsSize == 1) return lists[0];

  struct ListNode* headToReturn = NULL;  // O(n) space, (fixed list size listsSize)
  struct ListNode* tail = NULL;

  struct ListNode* currentListsNodes[listsSize];
  int i = 0;
  while(i < listsSize) {  // O(k) time
    currentListsNodes[i] = lists[i];
    i++;
  }

  // O(n*k) time
  while(1) {  // O(n) time (items amount)
    int smallest = -1;
    int smallestIndex = -1;

    int j = 0;
    while(j < listsSize) {  // O(k) time (listsSize)
      if(currentListsNodes[j] != NULL && (currentListsNodes[j]->val < smallest || smallestIndex == -1)) {
        smallest = currentListsNodes[j]->val;
        smallestIndex = j;
      }
      j++;
    }
    if(smallestIndex == -1) break;  // iterated all items, break

    if(tail == NULL) {
      headToReturn = currentListsNodes[smallestIndex];
      tail = headToReturn;
    } else {
      tail->next = currentListsNodes[smallestIndex];
      tail = tail->next;
    }

    currentListsNodes[smallestIndex] = currentListsNodes[smallestIndex]->next;
    tail->next = NULL;
  }

  return headToReturn;
}

// ---------- just to test with valgrind ----------
struct ListNode* newNode(int val) {
  struct ListNode* node = malloc(sizeof(struct ListNode));
  node->val = val;
  node->next = NULL;
  return node;
}

void printList(struct ListNode* head) {
  while(head != NULL) {
    printf("%d ", head->val);
    head = head->next;
  }
  printf("\n");
}

void freeList(struct ListNode* head) {
  while(head != NULL) {
    struct ListNode* tmp = head;
    head = head->next;
    free(tmp);
  }
}

int main() {
  struct ListNode* l1 = newNode(1);
  l1->next = newNode(4);
  l1->next->next = newNode(5);

  struct ListNode* l2 = newNode(1);
  l2->next = newNode(3);
  l2->next->next = newNode(4);

  struct ListNode* l3 = newNode(2);
  l3->next = newNode(6);

  struct ListNode* lists[3] = {l1, l2, l3};

  struct ListNode* merged = mergeKLists(lists, 3);

  printList(merged);

  freeList(merged);

  return 0;
}