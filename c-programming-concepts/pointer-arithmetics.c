/*
Deleting the executable after running:
gcc pointer-arithmetics.c -o test && ./test && rm -rf ./test

Reference:
- https://www.geeksforgeeks.org/pointer-arithmetics-in-c-with-examples/
- https://www.log2base2.com/C/pointer/arr+1-vs-address-of-arr+1.html
  // - if we do &arr+1 (we would move to the next block of x elements of type int)
  // -- because we are getting the array memory address (x items of type int) and doing pointer arithmetics
  // - if we do arr+1 we go to the next item inside the arr (check the code below)
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
  // an array is a pointer
  int arr[2] = {1, 2};
  int* firstItemPtr = &(arr[0]);
  printf("*arr: %d\n", *arr);
  printf("*firstItemPtr: %d\n", *firstItemPtr);
  if(firstItemPtr == arr) {  // compairson of pointers
    printf("same memory address\n");
  }
  if(*firstItemPtr == *arr) {  // compairson of pointer values
    printf("same value\n");
  }

  int* numbers = (int*)calloc(10, sizeof(int));  // init all items as zeros with calloc
  numbers[0] = 11;
  numbers[9] = 99;
  printf("Using numbers[i]: ");
  for(int i = 0; i <= 9; i++) {
    printf("%d", numbers[i]);
    if(i == 9) {
      printf(".\n");
    } else {
      printf(", ");
    }
  }

  printf("Using *numbers (pointer arithmetics): ");
  for(int i = 0; i <= 9; i++) {
    printf("%d", *numbers);
    if(i == 9) {
      printf(".\n");
    } else {
      numbers++;  // it will increment 1 * sizeof(int) on the pointer's memory address -> going to the next int
      printf(", ");
    }
  }
  printf("*numbers now: %d\n", *numbers);  // we need to return the pointer to the first item.

  int* endPtr = numbers;  // saving last value in another pointer

  // returning to first element
  numbers -= 9;  // it will decrement 9 * sizeof(int) on the pointer's memory address -> going to the first int

  printf("*numbers start again: %d\n", *numbers);
  printf("*endPtr: %d\n", *endPtr);

  return 0;
}