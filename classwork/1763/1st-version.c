#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int invalidindexfunc(char* s) {
  for(int i = 0; i < strlen(s); i++) {
    int hasUpper = 0;
    int hasLower = 0;
    for(int j = 0; j < strlen(s); j++) {
      int hasUpperCase = 0;  // how to check uppercase?
      if(s[i] >= 'a' && s[i] <= 'z' && hasUpperCase) {
        hasLower = 1;
      }
      int hasLowerCase = 0;  // how to check lowercase?
      if(s[i] >= 'A' && s[i] <= 'Z' && hasLowerCase) {
        hasUpper = 1;
      };
    }
    if(!(hasUpper && hasLower)) {
      return i;
    }
  }
  return -1;
}

char** removeinvalidIdx(char* s, int index) {
  // idea here is to divide this string in two new strings (cutting one invalid char)
  char** stringsWithoutInvalidIdx = (char**)malloc(sizeof(char*) * 2);

  if(index >= 1) {
    stringsWithoutInvalidIdx[0] = (char*)malloc(index + 1);
    for(int i = 0; i < index; i++) {
      stringsWithoutInvalidIdx[0][i] = s[i];
    }
    stringsWithoutInvalidIdx[0][index] = '\0';
  } else {
    stringsWithoutInvalidIdx[0] = (char*)malloc(1);
    stringsWithoutInvalidIdx[0][0] = '\0';
  }

  int rightLen = strlen(s) - (index + 1);
  stringsWithoutInvalidIdx[1] = (char*)malloc(rightLen + 1);
  for(int i = 0; i < rightLen; i++) {
    stringsWithoutInvalidIdx[1][i] = s[index + 1 + i];
  }
  stringsWithoutInvalidIdx[1][rightLen] = '\0';

  return stringsWithoutInvalidIdx;
}

char* longestNiceSubstring(char* s) {
  if(!s || strlen(s) <= 1) {
    return "\0";
  }

  int invalidIndex = invalidindexfunc(s);
  if(invalidIndex < 0) {
    int len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    strcpy(copy, s);
    return copy;
  }

  char** newstrings = removeinvalidIdx(s, invalidIndex);
  char* first = longestNiceSubstring(newstrings[0]);
  char* scnd = longestNiceSubstring(newstrings[1]);

  char* result;
  if(strlen(scnd) > strlen(first)) {
    result = (char*)malloc(strlen(scnd) + 1);
    strcpy(result, first);
  } else {
    result = (char*)malloc(strlen(first) + 1);
    strcpy(result, scnd);
  }

  return result;
}

int main() {
  char s[] = "YazaAay";
  printf("test\n");
  char* res = longestNiceSubstring(s);
  printf("res %s", res);
  return 0;
}
