// https://leetcode.com/problems/implement-queue-using-stacks/description/
struct Stack {
  int topIndex;  // -1 == empty
  int limit;
  int* values;
} typedef Stack;

typedef struct {
  Stack* pushStack;
  Stack* popStack;
} MyQueue;

int isStackEmpty(Stack* stack) {
  return stack->topIndex == -1 ? 1 : 0;
}

int stackPop(Stack* stack) {
  if(isStackEmpty(stack)) {
    return -1;
  }
  int currTopToReturn = stack->values[stack->topIndex];
  stack->topIndex -= 1;
  return currTopToReturn;
}

int stackPeek(Stack* stack) {
  if(isStackEmpty(stack)) {
    return -1;
  }
  return stack->values[stack->topIndex];
}

void stackPush(Stack* stack, int val) {
  if(stack->topIndex >= stack->limit - 1) {
    // reached limit, increase it 2x with realloc
    stack->limit *= 2;
    stack->values = (int*)realloc(stack->values, sizeof(int) * stack->limit);
  }
  stack->topIndex += 1;
  stack->values[stack->topIndex] = val;
}

Stack* stackCreate() {
  Stack* stack = (Stack*)malloc(sizeof(Stack));
  stack->topIndex = -1;
  stack->limit = 1;
  stack->values = (int*)malloc(sizeof(int) * 1);
  return stack;
}

void stackFree(Stack* stack) {
  free(stack->values);
  free(stack);
}

MyQueue* myQueueCreate() {
  Stack* pushStack = stackCreate();
  Stack* popStack = stackCreate();
  MyQueue* queue = (MyQueue*)malloc(sizeof(MyQueue));
  queue->pushStack = pushStack;
  queue->popStack = popStack;
  return queue;
}

void myQueuePush(MyQueue* obj, int x) {
  stackPush(obj->pushStack, x);
}

bool myQueueEmpty(MyQueue* obj) {
  return !!(isStackEmpty(obj->pushStack) && isStackEmpty(obj->popStack));
}

int myQueuePop(MyQueue* obj) {
  if(myQueueEmpty(obj)) {
    return -1;
  }

  if(isStackEmpty(obj->popStack)) {
    while(!isStackEmpty(obj->pushStack)) {
      int val = stackPop(obj->pushStack);
      stackPush(obj->popStack, val);
    }
  }
  return stackPop(obj->popStack);
}

int myQueuePeek(MyQueue* obj) {
  if(myQueueEmpty(obj)) {
    return -1;
  }

  if(isStackEmpty(obj->popStack)) {
    while(!isStackEmpty(obj->pushStack)) {
      int val = stackPop(obj->pushStack);
      stackPush(obj->popStack, val);
    }
  }
  return stackPeek(obj->popStack);
}

void myQueueFree(MyQueue* obj) {
  stackFree(obj->pushStack);
  stackFree(obj->popStack);
}