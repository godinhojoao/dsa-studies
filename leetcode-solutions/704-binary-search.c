// https://leetcode.com/problems/binary-search/description/
// read explanation on ./dsa-in-c/binarySearch.md

int search(int* nums, int numsSize, int target) {
  if(numsSize == 0) {
    return -1;
  }
  int left = -1;
  int right = numsSize;

  while(left < right - 1) {  // the value on middle is our value
    int middle = left + ((right - left) / 2);  // this is important in c to avoid overflow

    if(nums[middle] < target) {
      left = middle;
    } else {
      right = middle;
    }
  }

  return right >= 0 && right < numsSize && nums[right] == target ? right : -1;
}