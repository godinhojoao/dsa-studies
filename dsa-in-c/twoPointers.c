/*
Deleting the executable after running:
gcc twoPointers.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverseString(char* string, int stringLength);

int main() {
  char myString[] = "Hannah rac rac rac ";
  reverseString(myString, strlen(myString));

  printf("reversed: %s\n", myString);

  return 0;
}

void reverseString(char* string, int stringLength) {
  int left = 0;
  int right = stringLength - 1;

  while(left < right) {  // they can't be equal, and left can't be higher than right
    char temp = string[left];
    string[left] = string[right];
    string[right] = temp;

    left += 1;
    right -= 1;
  }
}