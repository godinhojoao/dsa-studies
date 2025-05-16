// https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/
// I did it using two pointers, read more on ./dsa-in-c/twoPointers.md

int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
  int* response = (int*)malloc(sizeof(int) * 2);

  int left = 0;
  int right = numbersSize - 1;

  while(left < right) {
    if(numbers[left] + numbers[right] == target) {
      response[0] = left + 1;
      response[1] = right + 1;
      *returnSize = 2;
      return response;
    }

    if(numbers[left] + numbers[right] > target) {
      right -= 1;
    } else {
      left += 1;
    }
  }

  *returnSize = 0;
  return response;
}