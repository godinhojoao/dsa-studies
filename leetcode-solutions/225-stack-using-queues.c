// https://leetcode.com/problems/implement-stack-using-queues/description/
// read explanation about stack in: ./dsa-in-c/stack.md
// read explanation about queue in: ./dsa-in-c/queue.md
// Check also the image with explanation: https://github.com/godinhojoao/dsa-studies/blob/main/assets/225-stack-using-queues.png

// Big O
// push time complexity: O(1)
// pop/top time complexity: O(n)
// space complexity O(n)

// Using two queues
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node* next;
} Node;

typedef struct Queue {
  Node* head;
  Node* tail;
} Queue;

Queue* initQueue() {
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  queue->head = NULL;
  queue->tail = NULL;
  return queue;
}

bool isEmpty(Queue* queue) {
  return queue->head == NULL;
}

void enqueue(Queue* queue, int value) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->value = value;
  newNode->next = NULL;

  if(isEmpty(queue)) {
    queue->head = newNode;
    queue->tail = newNode;
  } else {
    queue->tail->next = newNode;
    queue->tail = newNode;
  }
}

int dequeue(Queue* queue) {
  if(isEmpty(queue)) {
    printf("queue is empty, can't dequeue\n");
    return -1;
  }

  Node* temp = queue->head;
  queue->head = queue->head->next;
  int tempVal = temp->value;
  free(temp);
  return tempVal;
}

void freeQueue(Queue* queue) {
  while(!isEmpty(queue)) {
    dequeue(queue);
  }
  free(queue);
}

int peek(Queue* queue) {
  if(isEmpty(queue)) {
    printf("queue is empty, can't dequeue\n");
    return -1;
  }
  printf("no peek queue->head->value: %d \n", queue->head->value);
  return queue->head->value;
}

typedef struct {
  Queue* mainQueue;
  Queue* auxiliaryQueue;
} MyStack;

MyStack* myStackCreate() {
  MyStack* stack = malloc(sizeof(MyStack));
  stack->mainQueue = initQueue();
  stack->auxiliaryQueue = initQueue();
  return stack;
}

void myStackPush(MyStack* obj, int x) {
  enqueue(obj->mainQueue, x);
}

int myStackPop(MyStack* obj) {
  while(obj->mainQueue->head != obj->mainQueue->tail) {  // only last element remains (last of queue is first of the stack)
    int val = dequeue(obj->mainQueue);
    enqueue(obj->auxiliaryQueue, val);
  }

  int top = dequeue(obj->mainQueue);

  Queue* temp = obj->mainQueue;
  obj->mainQueue = obj->auxiliaryQueue;
  obj->auxiliaryQueue = temp;

  return top;
}

int myStackTop(MyStack* obj) {
  int top;

  while(obj->mainQueue->head != obj->mainQueue->tail) {  // only last element remains (last of queue is first of the stack)
    int val = dequeue(obj->mainQueue);
    enqueue(obj->auxiliaryQueue, val);
  }

  top = dequeue(obj->mainQueue);
  enqueue(obj->auxiliaryQueue, top);

  Queue* temp = obj->mainQueue;
  obj->mainQueue = obj->auxiliaryQueue;
  obj->auxiliaryQueue = temp;

  return top;
}

bool myStackEmpty(MyStack* obj) {
  return isEmpty(obj->mainQueue) && isEmpty(obj->auxiliaryQueue);
}

void myStackFree(MyStack* obj) {
  freeQueue(obj->mainQueue);
  freeQueue(obj->auxiliaryQueue);
  free(obj);
}

/**
 * Your MyStack struct will be instantiated and called as such:
 * MyStack* obj = myStackCreate();
 * myStackPush(obj, x);

 * int param_2 = myStackPop(obj);

 * int param_3 = myStackTop(obj);

 * bool param_4 = myStackEmpty(obj);

 * myStackFree(obj);
*/

// USING a doubly linked list -> ignore
// #include <stdbool.h>
// #include <stdlib.h>

// struct Node {
//   int value;
//   struct Node* next;
//   struct Node* prev;
// } typedef Node;

// typedef struct {
//   struct Node* top;
//   struct Node* base;
//   int length;
// } MyStack;

// MyStack* myStackCreate() {
//   MyStack* stack = (MyStack*)malloc(sizeof(MyStack));
//   stack->base = NULL;
//   stack->top = NULL;
//   stack->length = 0;
//   return stack;
// }

// bool myStackEmpty(MyStack* obj) { return obj->length == 0; }

// void myStackPush(MyStack* obj, int x) {
//   struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
//   newNode->value = x;
//   newNode->next = NULL;
//   newNode->prev = NULL;

//   if(myStackEmpty(obj)) {
//     obj->base = newNode;
//     obj->top = newNode;
//     obj->length += 1;

//     return;
//   }

//   obj->length += 1;
//   newNode->prev = obj->top;
//   obj->top->next = newNode;
//   obj->top = newNode;
// }

// int myStackPop(MyStack* obj) {
//   if(myStackEmpty(obj)) {
//     return -1;
//   }

//   struct Node* toDelete = obj->top;
//   int topValue = toDelete->value;

//   obj->top = toDelete->prev;
//   obj->length -= 1;
//   free(toDelete);

//   if(myStackEmpty(obj)) {
//     obj->base = NULL;
//   }

//   return topValue;
// }

// int myStackTop(MyStack* obj) {
//   if(myStackEmpty(obj)) {
//     return -1;
//   }
//   int topValue = obj->top->value;
//   return topValue;
// }

// void myStackFree(MyStack* obj) {
//   while(!myStackEmpty(obj)) {
//     myStackPop(obj);
//   }
//   free(obj);
// }