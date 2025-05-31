/*
Deleting the executable after running:
gcc queue.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[30];
} User;

struct Node {
  User* value;
  struct Node* next;
} typedef Node;

typedef struct {
  Node* head;
  Node* tail;
} Queue;

void reset(Queue* queue) {
  queue->head = NULL;
  queue->tail = NULL;
}

int isEmpty(Queue* queue) {
  return queue->head == NULL;
}

int enqueue(Queue* queue, User* user) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  if(newNode == NULL) {
    printf("error on malloc node\n");
    exit(1);
  }

  User* newUser = (User*)malloc(sizeof(User));
  if(newUser == NULL) {
    printf("error on malloc user\n");
    exit(1);
  }
  strcpy(newUser->name, user->name);

  newNode->value = newUser;
  newNode->next = NULL;

  if(isEmpty(queue)) {
    queue->head = newNode;
    queue->tail = newNode;
  } else {
    Node* tempOldTail = queue->tail;
    queue->tail = newNode;
    tempOldTail->next = newNode;
  }

  return 1;
}

User dequeue(Queue* queue) {
  if(isEmpty(queue)) {
    printf("queue is empty, can't dequeue\n");
    return;
  }

  Node* tempHead = queue->head;
  User userCopy = *tempHead->value;  // copy before freeing to return

  queue->head = queue->head->next;

  // free old head value
  free(tempHead->value);
  free(tempHead);

  return userCopy;
}

User peek(Queue* queue) {
  if(isEmpty(queue)) {
    printf("queue is empty, can't peek\n");
    return;
  }

  return *queue->head->value;
}

void clear(Queue* queue) {
  Node* curr = queue->head;
  while(curr != NULL) {
    Node* next = curr->next;
    free(curr->value);
    free(curr);
    curr = next;
  }
  free(queue);
}

int main() {
  User* joaoUser = (User*)malloc(sizeof(User));
  strcpy(joaoUser->name, "joao");
  User* doeUser = (User*)malloc(sizeof(User));
  strcpy(doeUser->name, "doe");
  User* johnUser = (User*)malloc(sizeof(User));
  strcpy(johnUser->name, "john");

  Queue* usersQueue = (Queue*)malloc(sizeof(Queue));
  reset(usersQueue);

  dequeue(usersQueue);  // queue is empty, can't dequeue
  peek(usersQueue);  // queue is empty, can't peek

  // add 3 users
  enqueue(usersQueue, joaoUser);
  enqueue(usersQueue, doeUser);
  enqueue(usersQueue, johnUser);
  free(joaoUser);
  free(doeUser);
  free(johnUser);

  // FIFO - first in first out
  User headUser1 = dequeue(usersQueue);
  printf("1 headUser1->name: %s\n", headUser1.name);  // joao
  User headUser2 = dequeue(usersQueue);
  printf("2 headUser2.name: %s\n", headUser2.name);  // doe
  User headUser3 = dequeue(usersQueue);
  printf("3 headUser3.name: %s\n", headUser3.name);  // john

  dequeue(usersQueue);  // queue is empty, can't dequeue

  User* beforeClearUser = (User*)malloc(sizeof(User));
  strcpy(beforeClearUser->name, "before clear");
  enqueue(usersQueue, beforeClearUser);
  free(beforeClearUser);

  User headUser4 = peek(usersQueue);
  printf("4 headUser4.name: %s\n", headUser4.name);  // before clear

  clear(usersQueue);

  return 0;
}