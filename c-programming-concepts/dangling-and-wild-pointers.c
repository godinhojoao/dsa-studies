/*
Deleting the executable after running:
gcc dangling-and-wild-pointers.c -o test && ./test && rm -rf ./test

Dangling Pointer: A pointer that still holds an address, but the memory it points to is no longer valid or in use by the program.
  - accessing is undefined behavior.
Wild pointer: A pointer that has not been initialized to anything - not even NULL. (garbage)
  - accessing is undefined behavior.

Read more:
https://stackoverflow.com/questions/17997228/what-is-a-dangling-pointer
https://www.geeksforgeeks.org/c/dangling-void-null-wild-pointers/
*/
#include <stdio.h>
#include <stdlib.h>

int* getDanglingPointerFreedByStackMemory() {
  int a = 30;
  return &a; // this is a dangling pointer: I'm returning the address of a local variable, which is on the stack, not the heap -> never do this
}

int* getDanglingPointerManuallyFreedHeapMemory() {
  int* a = malloc(sizeof(int));
  *a = 20;
  free(a); // memory freed becomes a dangling pointer
  return a;
}

int main() {
  // Dangling Pointer example
  int* res = getDanglingPointerFreedByStackMemory();
  printf("res: %d\n", *res); // undefined behavior

  int* res2 = getDanglingPointerManuallyFreedHeapMemory();
  printf("res2: %d\n", *res2); // undefined behavior

  // Wild Pointer example
  int* wildPtr; // uninitialized
  printf("wildPtr: %d\n", *wildPtr); // undefined behavior

  return 0;
}