// https://leetcode.com/problems/reverse-string/
// we dit it using two pointers, read more on ./dsa-in-c/twoPointers.md

void reverseString(char* s, int sSize) {
  int left = 0;
  int right = sSize - 1;

  while(left < right) {  // they can't be equal, and left can't be higher than right
    char temp = s[left];
    s[left] = s[right];
    s[right] = temp;

    left += 1;
    right -= 1;
  }
}