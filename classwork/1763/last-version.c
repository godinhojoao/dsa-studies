#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInvalidIndex(char* s) {
  int len = strlen(s);
  for(int i = 0; i < len; i++) {
    int hasUpper = 0;
    int hasLower = 0;
    for(int j = 0; j < len; j++) {
      if(tolower(s[i]) == s[j]) {
        hasLower = 1;
      }
      if(toupper(s[i]) == s[j]) {
        hasUpper = 1;
      };
    }
    if(!(hasUpper && hasLower)) {
      return i;
    }
  }
  return -1;
}

char** getSubstrings(char* s, int index) {
  // idea here is to divide this string in two new strings (cutting one invalid char)
  int len = strlen(s);
  char** substrings = (char**)malloc(sizeof(char*) * 2);

  if(index >= 1) {
    substrings[0] = (char*)malloc(index + 1);
    for(int i = 0; i < index; i++) {
      substrings[0][i] = s[i];
    }
    substrings[0][index] = '\0';
  } else {
    substrings[0] = (char*)malloc(1);
    substrings[0][0] = '\0';
  }

  int rightLen = len - (index + 1);
  substrings[1] = (char*)malloc(rightLen + 1);
  for(int i = 0; i < rightLen; i++) {
    substrings[1][i] = s[index + 1 + i];
  }
  substrings[1][rightLen] = '\0';

  return substrings;
}

char* longestNiceSubstring(char* s) {
  if(!s || strlen(s) <= 1) {
    char* empty = (char*)malloc(1);
    empty[0] = '\0';
    return empty;
  }

  int invalidIndex = getInvalidIndex(s);
  if(invalidIndex < 0) {
    int len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    strcpy(copy, s);
    return copy;
  }

  char** substrings = getSubstrings(s, invalidIndex);
  char* left = longestNiceSubstring(substrings[0]);
  char* right = longestNiceSubstring(substrings[1]);

  int leftLength = strlen(left);
  int rightLength = strlen(right);

  char* result;
  if(leftLength == rightLength) {
    result = (char*)malloc(leftLength + 1);
    strcpy(result, left);
  } else if(rightLength > leftLength) {
    result = (char*)malloc(rightLength + 1);
    strcpy(result, right);
  } else {
    result = (char*)malloc(leftLength + 1);
    strcpy(result, left);
  }

  free(substrings[0]);
  free(substrings[1]);
  free(substrings);
  free(left);
  free(right);

  return result;
}

int main() {
  char s1[] = "YazaAay";
  char* res1 = longestNiceSubstring(s1);
  printf("res23: %s\n", res1);

  free(res1);
  return 0;
}
