// https://leetcode.com/problems/implement-stack-using-queues/description/
// read explanation about stack in: ./dsa-in-c/stack.md
// read explanation about queue in: ./dsa-in-c/queue.md

// Using a queue


// USING a doubly linked list
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