//gcc mutable-vs-immutable.c -o test && ./test && rm -rf ./test
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // We can't modify a string literal stored in read-only memory (would crash).
  // Even though we can change the pointer *myName, we can't modify the value it points to (because it's a string literal).
  char *myNameImutable = "Joao";
  // myName[0] = 'M'; // can't do this
  printf("myNameImutable: %s\n", myNameImutable);

  // Here we are storing an array of chars on the stack (or data section).
  // This array is mutable, so you can change its values normally.
  // But you can't make the array point to another address, since it's an array and not a pointer.
  char myNameMutable[] = "Joao";
  myNameMutable[0] = 'M';
  printf("myNameMutable: %s\n", myNameMutable);

  // Here we are allocating memory on the heap.
  // The values in the memory pointed to by myName are mutable.
  // And we can also change the address that myName points to.
  // We need to free the memory allocated on the heap.
  char *myNameMalloc = malloc(5 * sizeof(char));
  strcpy(myNameMalloc, "Joao");
  myNameMalloc[0] = 'M';
  printf("myNameMalloc: %s\n", myNameMalloc);

  free(myNameMalloc);
  return 0;
}
