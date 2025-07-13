// https://leetcode.com/problems/valid-parentheses/description
// Check also the image with explanation: https://github.com/godinhojoao/dsa-studies/blob/main/assets/20-valid-parentheses.png
// gcc 20-valid-parentheses.c -o test && ./test && rm -rf ./test

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack {
  int base;
  int top;
  char* values;
};

void push(struct Stack* stack, char val) {
  stack->values[stack->top++] = val;
}

char pop(struct Stack* stack) {
  return stack->values[--stack->top];
}

bool isEmpty(struct Stack* stack) {
  return stack->top == stack->base;
}

bool isValid(char* s) {
  int n = strlen(s);
  struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
  stack->base = 0;
  stack->top = 0;
  stack->values = (char*)malloc(n * sizeof(char));

  for(int i = 0; i < n; i++) {
    char current = s[i];

    if(current == '(' || current == '[' || current == '{') {
      push(stack, current);
    } else {
      if(isEmpty(stack)) {  // first char and is closing (invalid)
        free(stack->values);
        free(stack);
        return false;
      }

      char top = pop(stack);
      if((current == ')' && top != '(') ||
         (current == ']' && top != '[') ||
         (current == '}' && top != '{')) {
        free(stack->values);
        free(stack);
        return false;
      }
    }
  }

  bool result = isEmpty(stack);
  free(stack->values);
  free(stack);

  return result;
}

int main() {
  char* myString = "()[]{}";
  printf("is valid? %d\n", isValid(myString));
  return 0;
}
