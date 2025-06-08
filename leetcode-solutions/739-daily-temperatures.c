// brute force solution
// time complexity O(n^2), space complexity O(n)
int* dailyTemperatures(int* temperatures, int temperaturesSize, int* returnSize) {
  int* arr = (int*)malloc(sizeof(int) * temperaturesSize);
  *returnSize = temperaturesSize;

  for(int i = 0; i < temperaturesSize; i++) {
    int currentTemperature = temperatures[i];
    int amountOfAttempts = 0;
    int found = 0;
    for(int j = i + 1; j < temperaturesSize; j++) {
      amountOfAttempts += 1;
      if(currentTemperature < temperatures[j]) {
        found = 1;
        break;
      }
    }

    arr[i] = found == 1 ? amountOfAttempts : 0;
  }

  return arr;
}

// stack solution