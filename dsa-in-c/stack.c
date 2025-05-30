/*
Deleting the executable after running:
gcc stack.c -o test && ./test && rm -rf ./test
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[30];
} User;

typedef struct {
  int top;
  int base;
  int limit;
  User* users;
} Stack;

void reset(Stack* stack, int startLimit) {
  stack->base = 0;
  stack->top = 0;
  stack->limit = startLimit;  // once it reaches the limit we will double (dynamic)
  if(stack->limit > 0) {
    stack->users = (User*)malloc(sizeof(User) * stack->limit);
  } else {
    stack->users = NULL;
  }
}

int isFull(Stack* stack) {
  return stack->top == stack->limit;
}

int isEmpty(Stack* stack) {
  return stack->top == 0;
}

int push(Stack* stack, User* user) {
  if(isFull(stack)) {
    stack->limit *= 2;  // doubling capacity
    stack->users = (User*)realloc(stack->users, sizeof(User) * stack->limit);

    if(stack->users == NULL) {
      printf("realloc failed.\n");
      exit(1);
    }
  }

  stack->users[stack->top] = *user;
  stack->top += 1;
  return 1;
}

User* pop(Stack* stack) {
  if(isEmpty(stack)) {
    printf("stack is empty, can't pop\n");
    return NULL;
  }
  stack->top -= 1;

  return &stack->users[stack->top];
}

void clear(Stack* stack) {
  free(stack->users);
  reset(stack, 0);
  free(stack);
}

int main() {
  User* joaoUser = (User*)malloc(sizeof(User));
  strcpy(joaoUser->name, "joao");
  User* doeUser = (User*)malloc(sizeof(User));
  strcpy(doeUser->name, "doe");
  User* johnUser = (User*)malloc(sizeof(User));
  strcpy(johnUser->name, "john");

  Stack* usersStack = (Stack*)malloc(sizeof(Stack));
  reset(usersStack, 2);  // start with limit size of 2

  pop(usersStack);  // stack is empty, can't pop

  // add 3 users
  push(usersStack, joaoUser);
  push(usersStack, doeUser);
  push(usersStack, johnUser);  // here it will double the size limit to 4
  free(joaoUser);
  free(doeUser);
  free(johnUser);

  // LIFO - last in first out
  User* topUser1 = pop(usersStack);
  printf("1 topUser1->name: %s\n", topUser1->name);  // john
  User* topUser2 = pop(usersStack);
  printf("2 topUser2->name: %s\n", topUser2->name);  // doe
  User* topUser3 = pop(usersStack);
  printf("3 topUser3->name: %s\n", topUser3->name);  // joao

  pop(usersStack);  // stack is empty, can't pop

  clear(usersStack);

  return 0;
}