/*
Deleting the executable after running:
gcc dynamicArray.c -o test && ./test && rm -rf ./test
*/

#include <stdio.h>
#include <stdlib.h>

struct {
  int* values;
  int capacity;
  int length;
} typedef DynamicArray;

DynamicArray* initializeDynamicArray(int initialCapacity);
void pushToDynamicArray(DynamicArray* array, int value);
void freeDynamicArray(DynamicArray* array);
void printDynamicArray(DynamicArray* array, char arrayName[]);

int main() {
  int initialCapacity = 2;
  DynamicArray* array = initializeDynamicArray(initialCapacity);

  pushToDynamicArray(array, 10);
  pushToDynamicArray(array, 11);
  pushToDynamicArray(array, 12);

  printDynamicArray(array, "myarray");

  freeDynamicArray(array);

  return 0;
}

DynamicArray* initializeDynamicArray(int initialCapacity) {
  DynamicArray* array = (DynamicArray*)malloc(sizeof(DynamicArray));
  if(array == NULL) {
    printf("malloc failed.\n");
    exit(1);
  }
  array->values = (int*)(malloc(sizeof(int) * initialCapacity));
  if(array->values == NULL) {
    printf("malloc failed2.\n");
    exit(1);
  }
  array->capacity = initialCapacity;
  array->length = 0;
  return array;
}

void pushToDynamicArray(DynamicArray* array, int value) {
  printf("----\n");
  printf("Length before: %d\n", array->length);
  printf("Capacity before: %d\n", array->capacity);

  array->length += 1;
  if(array->length > array->capacity) {
    array->capacity *= 2;
    array->values = (int*)realloc(array->values, sizeof(int) * array->capacity);  // doubling size
    if(array->values == NULL) {
      printf("realloc failed.\n");
      exit(1);
    }
  }
  array->values[array->length - 1] = value;

  printf("Length after: %d\n", array->length);
  printf("Capacity after: %d\n", array->capacity);
  printf("----\n");
}

void freeDynamicArray(DynamicArray* array) {
  free(array->values);
  free(array);
}

void printDynamicArray(DynamicArray* array, char arrayName[]) {
  printf("----\n");

  printf("%s: ", arrayName);
  for(int i = 0; i < array->length; i++) {
    printf("%d", array->values[i]);
    if(i + 1 == array->length) {
      printf(".\n");
    } else {
      printf(", ");
    }
  }

  printf("----\n");
}
