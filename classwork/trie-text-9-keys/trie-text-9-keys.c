/*
Deleting the executable after running:
gcc trie-text-9-keys.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCEPTED_SYMBOLS_AMOUNT 8 // values 2 to 9
#define MAX_CHARS_PER_LINE 150

struct TrieNode {
  char val;
  struct TrieNode* next[ACCEPTED_SYMBOLS_AMOUNT];
  int isEndOfWord;
  char* wordsBuffer;
} typedef TrieNode;

int getTrieDigitIndex(char digit) {
  return (digit - '0') - 2; // since it's a digit between 2 and 9, indexes will vary between 0 and 7
}

void wordToDigitsSequence(char* s, char* digits) {
  int currDigitsIndex = 0;

  for (size_t i = 0; s[i]; ++i) {
    unsigned char c = (unsigned char)s[i];

    if (c == 0xC3) {  // handle 2-byte UTF-8 accented letters
      unsigned char d = (unsigned char)s[i + 1];

      switch (d) {
        // à á â ã ç À Á Â Ã Ç
      case 0xA0:
      case 0xA1:
      case 0xA2:
      case 0xA3:
      case 0xA7:
      case 0x80:
      case 0x81:
      case 0x82:
      case 0x83:
      case 0x87:
        digits[currDigitsIndex] = '2';
        break;

        // è é ê È É Ê
      case 0xA8:
      case 0xA9:
      case 0xAA:
      case 0x88:
      case 0x89:
      case 0x8A:
        digits[currDigitsIndex] = '3';
        break;

        // ì í î Ì Í Î
      case 0xAC:
      case 0xAD:
      case 0xAE:
      case 0x8C:
      case 0x8D:
      case 0x8E:
        digits[currDigitsIndex] = '4';
        break;

        // ò ó ô õ Ò Ó Ô Õ
      case 0xB2:
      case 0xB3:
      case 0xB4:
      case 0xB5:
      case 0x92:
      case 0x93:
      case 0x94:
      case 0x95:
        digits[currDigitsIndex] = '6';
        break;

        // ù ú û Ù Ú Û
      case 0xB9:
      case 0xBA:
      case 0xBB:
      case 0x99:
      case 0x9A:
      case 0x9B:
        digits[currDigitsIndex] = '8';
        break;

      default:
        break;
      }

      i++;  // skip next byte
      currDigitsIndex++;
      continue;
    }

    switch (c) {
    case 'A': case 'a':
    case 'B': case 'b':
    case 'C': case 'c':
      digits[currDigitsIndex++] = '2';
      break;
    case 'D': case 'd':
    case 'E': case 'e':
    case 'F': case 'f':
      digits[currDigitsIndex++] = '3';
      break;
    case 'G': case 'g':
    case 'H': case 'h':
    case 'I': case 'i':
      digits[currDigitsIndex++] = '4';
      break;
    case 'J': case 'j':
    case 'K': case 'k':
    case 'L': case 'l':
      digits[currDigitsIndex++] = '5';
      break;
    case 'M': case 'm':
    case 'N': case 'n':
    case 'O': case 'o':
      digits[currDigitsIndex++] = '6';
      break;
    case 'P': case 'p':
    case 'Q': case 'q':
    case 'R': case 'r':
    case 'S': case 's':
      digits[currDigitsIndex++] = '7';
      break;
    case 'T': case 't':
    case 'U': case 'u':
    case 'V': case 'v':
      digits[currDigitsIndex++] = '8';
      break;
    case 'W': case 'w':
    case 'X': case 'x':
    case 'Y': case 'y':
    case 'Z': case 'z':
      digits[currDigitsIndex++] = '9';
      break;
    default:
      break;
    }

    digits[currDigitsIndex] = '\0';  // null-terminate result
  }
}

TrieNode* createNode(char val) {
  TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
  for (int i = 0; i < ACCEPTED_SYMBOLS_AMOUNT; i++) {
    newNode->next[i] = NULL;
  }
  newNode->val = val;
  newNode->isEndOfWord = 0;
  newNode->wordsBuffer = NULL;
  return newNode;
}

void insertWordOnTrie(TrieNode* root, char* trieWord, char* realWordToStore) {
  struct TrieNode* currNode = root;
  int lastCharIndex = strlen(trieWord) - 1;  // removing /0
  int currCharIndex = 0;

  while (currCharIndex <= lastCharIndex) {
    char currChar = trieWord[currCharIndex];
    int trieCharIndex = getTrieDigitIndex(currChar);
    if (currNode->next[trieCharIndex] == NULL) {
      currNode->next[trieCharIndex] = createNode(currChar);
    }

    currNode = currNode->next[trieCharIndex];
    if (currCharIndex == lastCharIndex) {
      // I'm not handling word duplicates right now considering the dictionary will be right.
      // But if we need to handle it would be here or before when reading file.
      currNode->isEndOfWord = 1;
      int currWordsBufferSize = currNode->wordsBuffer != NULL ? strlen(currNode->wordsBuffer) + 2 : 0; // +2 = 1 for line break, 1 for \0
      int newSize = currWordsBufferSize + strlen(realWordToStore) + 2; // 1 for \n if needed + 1 for \0
      currNode->wordsBuffer = realloc(currNode->wordsBuffer, newSize);
      if (!currNode->wordsBuffer) { return exit(1); }
      if (currWordsBufferSize == 0) {
        strcpy(currNode->wordsBuffer, realWordToStore); // avoid uninitialised write error with strcat
      }
      else {
        strcat(currNode->wordsBuffer, "\n");
        strcat(currNode->wordsBuffer, realWordToStore);
      }
    }
    currCharIndex++;
  }
}

void loadFileIntoTrie(TrieNode* root) {
  FILE* filePtr;
  filePtr = fopen("dicionario.txt", "r");
  if (filePtr == NULL) {
    printf("error reading file.\n");
    exit(1);
  }

  char line[MAX_CHARS_PER_LINE];
  char digitsSequence[MAX_CHARS_PER_LINE];
  while (fgets(line, sizeof(line), filePtr)) {
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';  // remove \n putting \0 in the place

    wordToDigitsSequence(line, digitsSequence);
    insertWordOnTrie(root, digitsSequence, line);
  }

  fclose(filePtr);
  return;
}

TrieNode* getWordPayload(TrieNode* root, char* digitsSequence) {
  TrieNode* currNode = root;
  int lastCharIndex = strlen(digitsSequence) - 1;  // removing /0
  int currCharIndex = 0;
  while (currCharIndex <= lastCharIndex) {
    char currChar = digitsSequence[currCharIndex];
    int trieCharIndex = getTrieDigitIndex(currChar);
    if (currNode->next[trieCharIndex] == NULL) {
      printf("This word: %s isn't registered.\n", digitsSequence);
      return NULL;
    }
    else {
      currNode = currNode->next[trieCharIndex];
    }

    if (currCharIndex == lastCharIndex && currNode->isEndOfWord == 1) {
      printf("Found words:\n%s\n", currNode->wordsBuffer);
      return currNode;
    }
    currCharIndex++;
  }

  printf("Word for this digits sequence: %s isn't registered.\n", digitsSequence);
  return NULL;
}

void freeTrieDFS(TrieNode* node) {
  if (!node) return;
  for (int i = 0; i < ACCEPTED_SYMBOLS_AMOUNT; i++) {
    freeTrieDFS(node->next[i]);
  }
  free(node->wordsBuffer);
  free(node);
}

int main() {
  /*
    [x] 1 read 1 line from txt -> "casa"
    [x] 2 "casa" in digits -> wordToDigitsSequence(casa) -> "2272"
    [x] 3 now just store in the trie the path 2272 (mark end of word on last 2 and put the word there into the buffer separating words by \n)
  */

  TrieNode* root = createNode('\0');
  loadFileIntoTrie(root);

  int currOpt = -1;
  while (currOpt != 0) {
    char currentDigits[MAX_CHARS_PER_LINE];
    printf("\nType 0 to exit or digits from 2 to 9 to check the respective words: \n");
    scanf("%s", currentDigits);

    if (strlen(currentDigits) == 1 && currentDigits[0] == '0') {
      currOpt = 0;
      break;
    }

    int isValid = 1;
    int currStringIndex = 0;
    while (currentDigits[currStringIndex] != '\0') {
      int digit = currentDigits[currStringIndex] - '0';
      if (digit < 2 || digit > 9) {
        printf("Invalid digit\n");
        isValid = 0;
        break;
      }
      currStringIndex++;
    }

    if (isValid) {
      TrieNode* res = getWordPayload(root, currentDigits);
    }
  }

  freeTrieDFS(root);

  return 0;
}