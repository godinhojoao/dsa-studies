// https://leetcode.com/problems/min-stack/description
// read explanation about stack in: ./dsa-in-c/stack.md
#include <stdlib.h>

typedef struct {
  int top;
  int capacity;
  int* stackItems;
  int* minStackItems;
} MinStack;

MinStack* minStackCreate() {
  MinStack* myStack = (MinStack*)malloc(sizeof(MinStack));
  myStack->top = 0;
  myStack->capacity = 1;

  // starting with 1 capacity (as we did on stack.c example to increase doubling when needed)
  myStack->stackItems = (int*)malloc(sizeof(int) * myStack->capacity);
  myStack->minStackItems = (int*)malloc(sizeof(int) * myStack->capacity);
  return myStack;
}

void minStackPush(MinStack* obj, int val) {
  obj->top += 1;

  if(obj->top > obj->capacity) {  // only realloc if we reach limit capacity (as we did on stack.c example)
    obj->capacity *= 2;
    obj->stackItems = (int*)realloc(obj->stackItems, sizeof(int) * obj->capacity);
    obj->minStackItems = (int*)realloc(obj->minStackItems, sizeof(int) * obj->capacity);
  }
  obj->stackItems[obj->top - 1] = val;

  // this way we guarantee our min stack has always the smallest value
  if((obj->top - 2) >= 0) {
    int currentMinElement = obj->minStackItems[obj->top - 2];
    obj->minStackItems[obj->top - 1] = currentMinElement < val ? currentMinElement : val;
  } else {
    obj->minStackItems[obj->top - 1] = val;
  }
}

void minStackPop(MinStack* obj) {
  obj->top -= 1;
}

int minStackTop(MinStack* obj) {
  return obj->stackItems[obj->top - 1];
}

int minStackGetMin(MinStack* obj) {
  return obj->minStackItems[obj->top - 1];  // last item is the smallest
}

void minStackFree(MinStack* obj) {
  free(obj->stackItems);
  free(obj->minStackItems);
  free(obj);
}
