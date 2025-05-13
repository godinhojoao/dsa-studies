/*
Deleting the executable after running:
gcc pointers-to-functions.c -o test && ./test && rm -rf ./test

Reference:
- https://www.ibm.com/docs/en/xl-c-and-cpp-aix/13.1.3?topic=functions-pointers
- https://medium.com/@togunchan/understanding-function-pointers-in-c-a-comprehensive-guide-76c1990a10d2
- https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work

Important:
- A pointer that stores the memory address for a function.
- We can't perform pointer arithmetic on pointers to functions.
- It can be useful to implement methods for structs.
- Using it incorrectly can make your code hard to read.
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct User {
  char* name;
  void (*ptrToSayName)(struct User* self);
} User;

void sayName(User* self) {
  printf("Name: %s\n", self->name);
};

int sum(int a, int b);

int main() {
  User joao;
  joao.name = "joao test";
  joao.ptrToSayName = &sayName;

  sayName(&joao);
  joao.ptrToSayName(&joao);

  // you need to define: the return type, pointer name, and inputs
  int (*ptrToSum)(int, int) = &sum;
  int sumResult = (*ptrToSum)(1,2);
  int sumResult2 = ptrToSum(1,2);
  printf("sumResult: %d\n", sumResult);
  printf("sumResult2: %d\n", sumResult2);

  return 0;
}

int sum(int a, int b) {
  return a+b;
}