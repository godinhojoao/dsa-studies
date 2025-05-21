/*
Deleting the executable after running:
gcc binarySearch.c -o test && ./test && rm -rf ./test
*/

#include <stdio.h>

// Assumes array is sorted in ascending order.
int binarySearch(int target, int arraySize, int array[]) {
  if(arraySize == 0) {
    return -1;
  }

  int left = -1, right = arraySize;
  while(left < right - 1) {
    int middle = left + ((right - left) / 2);  // this is important in c to avoid overflow
    // Invariants: v[l] < x <= v[r]
    if(array[middle] < target) {
      left = middle;
    } else {
      right = middle;
    }
  }

  return right >= 0 && right < arraySize && array[right] == target ? right : -1;
}

int main() {
  int numbers[] = {1, 2, 3, 99, 100, 101};  // size 6
  int numbers2[] = {};  // size 0

  int res99 = binarySearch(99, sizeof(numbers) / sizeof(int), numbers);
  int res333 = binarySearch(333, sizeof(numbers) / sizeof(int), numbers);
  int res0 = binarySearch(1, sizeof(numbers2) / sizeof(int), numbers2);

  printf("index: %d\n", res99);  // index: 3
  printf("index: %d\n", res333);  // index: -1
  printf("index: %d\n", res0);  // index: -1

  return 0;
}