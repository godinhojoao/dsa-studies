/*
https://github.com/marlovss/OpLexicon

Deleting the executable after running:
gcc trie-oplexicon.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ACCEPTED_SYMBOLS_AMOUNT 36

struct TrieNode {
  char val;
  struct TrieNode* next[ACCEPTED_SYMBOLS_AMOUNT];
  int polarity;
  int isEndOfWord;
  char class[4];
} typedef TrieNode;

// start - this was completely searched on Google - can't know the hex of each signal ã, õ, etc.
void normalizePtWord(char *s) {
  size_t w = 0;
  for (size_t i = 0; s[i]; ++i) {
    unsigned char c = (unsigned char)s[i];

    if (c == 0xC3) {                 // common 2-byte UTF-8 accents
      unsigned char d = (unsigned char)s[i+1];
      char out = 0;

      // lowercase
      if (d == 0xA0 || d == 0xA1 || d == 0xA2 || d == 0xA3) out = 'a'; // à á â ã
      else if (d == 0xA7) out = 'c';                                   // ç
      else if (d == 0xA8 || d == 0xA9 || d == 0xAA) out = 'e';         // è é ê
      else if (d == 0xAC || d == 0xAD || d == 0xAE) out = 'i';         // ì í î
      else if (d == 0xB2 || d == 0xB3 || d == 0xB4 || d == 0xB5) out = 'o'; // ò ó ô õ
      else if (d == 0xB9 || d == 0xBA || d == 0xBB) out = 'u';         // ù ú û

      // uppercase
      else if (d == 0x80 || d == 0x81 || d == 0x82 || d == 0x83) out = 'a'; // À Á Â Ã
      else if (d == 0x87) out = 'c';                                        // Ç
      else if (d == 0x88 || d == 0x89 || d == 0x8A) out = 'e';              // È É Ê
      else if (d == 0x8C || d == 0x8D || d == 0x8E) out = 'i';              // Ì Í Î
      else if (d == 0x92 || d == 0x93 || d == 0x94 || d == 0x95) out = 'o'; // Ò Ó Ô Õ
      else if (d == 0x99 || d == 0x9A || d == 0x9B) out = 'u';              // Ù Ú Û

      if (out) { s[w++] = out; i++; }   // consume 2nd byte
      else { i++; }                     // skip unknown C3 sequence
      continue;
    }

    if (c < 0x80) {                     // ASCII
      char t = (char)tolower(c);
      // keep only allowed chars
      if ((t >= 'a' && t <= 'z') ||
          t == '-' || t == ',' || t == '.' || t == '!' ||
          t == '_' || t == ':' || t == ';' || t == '?' ||
          t == '\'' || t == '"') {
        s[w++] = t;
      }
      // else: skip (e.g., spaces)
      continue;
    }

    // skip all other multibyte sequences cleanly
  }
  s[w] = '\0';
}
int getTrieCharIndex(char val) {
  if (val >= 'a' && val <= 'z') return val - 'a';
  switch (val) {
    case '-': return 26; case ',': return 27; case '.': return 28;
    case '!': return 29; case '_': return 30; case ':': return 31;
    case ';': return 32; case '?': return 33; case '\'': return 34; case '"': return 35;
    default: return -1; // skip unsupported
  }
}
// end - this was completely searched on Google - can't know the hex of each signal ã, õ, etc.

TrieNode* createNode(char val) {
  TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
  for(int i = 0; i < ACCEPTED_SYMBOLS_AMOUNT; i++) {
    newNode->next[i] = NULL;
  }
  newNode->val = val;
  newNode->isEndOfWord = 0;
  newNode->polarity = 0;
  return newNode;
}

void insertWord(TrieNode* root, char* word, int polarity, char class[4]) {
  if(polarity != 1 && polarity != 0 && polarity != -1) {
    printf("Invalid polarity, valid polarities: (-1,0,1)\n");
    return;
  }
  struct TrieNode* currNode = root;

  int lastCharIndex = strlen(word) - 1;  // removing /0
  int currCharIndex = 0;
  while(currCharIndex <= lastCharIndex) {
    char currChar = word[currCharIndex];
    int trieCharIndex = getTrieCharIndex(currChar);
    if(currNode->next[trieCharIndex] == NULL) {
      currNode->next[trieCharIndex] = createNode(currChar);
    }

    if(currCharIndex == lastCharIndex) {
      currNode->next[trieCharIndex]->isEndOfWord = 1;
      currNode->next[trieCharIndex]->polarity = polarity;
      strcpy(currNode->next[trieCharIndex]->class, class);
      currNode->next[trieCharIndex]->class[3] = '\0'; // safety
    }
    currCharIndex++;
    currNode = currNode->next[trieCharIndex];
  }
}

void loadFileIntoTrie(TrieNode* root) {
  FILE* filePtr;
  filePtr = fopen("OpLexicon.txt", "r");
  if(filePtr == NULL) {
    printf("error reading file.\n");
    exit(1);
  }

  char line[100]; //maxCharsPerLine = 100
  while(fgets(line, sizeof(line), filePtr)) {
    int len = strlen(line);
    if(len > 0 && line[len - 1] == '\n') line[len - 1] = '\0'; // remove \n putting \0 in the place

    char word[50], class[4];
    int polarity;

    if (sscanf(line, "%49[^,],%3[^,],%d", word, class, &polarity) == 3) {
      normalizePtWord(word);
      insertWord(root, word, polarity, class);
    }
  }

  fclose(filePtr);
  return;
}

TrieNode* getWordPayload(TrieNode* root, char* word) {
  TrieNode* currNode = root;
  int lastCharIndex = strlen(word) - 1;  // removing /0
  int currCharIndex = 0;
  while(currCharIndex <= lastCharIndex) {
    char currChar = word[currCharIndex];
    int trieCharIndex = getTrieCharIndex(currChar);
    if(currNode->next[trieCharIndex] == NULL) {
      printf("This word: %s isn't registered.\n", word);
      return NULL;
    } else {
      currNode = currNode->next[trieCharIndex];
    }

    if(currCharIndex == lastCharIndex && currNode->isEndOfWord == 1) {
      printf("Found word: \"%s\", polarity: %d, class: %s\n", word, currNode->polarity, currNode->class);
      return currNode;
    }
    currCharIndex++;
  }

  printf("This word: %s isn't registered.\n", word);
  return NULL;
}

int editWordPolarity(TrieNode* root, char* word, int polarity) {  // -1 = failed, 1 = success
  if(polarity != 1 && polarity != 0 && polarity != -1) {
    printf("Invalid polarity, valid polarities: (-1,0,1)\n");
    return -1;
  }
  TrieNode* currNode = root;
  int lastCharIndex = strlen(word) - 1;  // removing /0
  int currCharIndex = 0;
  while(currCharIndex <= lastCharIndex) {
    char currChar = word[currCharIndex];
    int trieCharIndex = getTrieCharIndex(currChar);
    if(currNode->next[trieCharIndex] == NULL) {
      return -1;
    } else {
      currNode = currNode->next[trieCharIndex];
    }

    if(currCharIndex == lastCharIndex && currNode->isEndOfWord == 1) {
      currNode->polarity = polarity;
      return 1;
    }
    currCharIndex++;
  }

  return -1;
}

void saveAndFreeTrieDFS(TrieNode* currNode, char* currentWord, int depth, FILE* filePtr) {
  if(currNode->isEndOfWord) {
    currentWord[depth] = '\0';
    fprintf(filePtr, "%s,%s,%d\n", currentWord, currNode->class, currNode->polarity);
  }

  for(int i = 0; i < ACCEPTED_SYMBOLS_AMOUNT; i++) {
    if(currNode->next[i]) {
      currentWord[depth] = currNode->next[i]->val;
      saveAndFreeTrieDFS(currNode->next[i], currentWord, depth + 1, filePtr);
    }
  }

  free(currNode);
}

void saveTrieOnFileAndFree(TrieNode* root) {
  FILE* filePtr;
  filePtr = fopen("trie-result.txt", "w");
  if(filePtr == NULL) {
    printf("error creating file.\n");
    exit(1);
  }

  char* word = malloc(sizeof(char) * 50);  // biggest word size: 50
  saveAndFreeTrieDFS(root, word, 0, filePtr);

  free(word);
  fclose(filePtr);
}

void menu(TrieNode* root) {
  int currOpt = 0;
  while(currOpt != 4) {
    printf("\n--- MENU: Select one operation ---\n");
    printf("1 - Search word\n");
    printf("2 - Insert word\n");
    printf("3 - Update word polarity\n");
    printf("4 - Exit and save updated data\n");
    printf("Current operation: ");
    scanf("%d", &currOpt);

    printf("\n");

    switch(currOpt) {
      case 1:
        printf("---- Searching word ----\n");

        char* wordToFind = malloc(sizeof(char) * 50);
        printf("Enter word: ");
        scanf(" %49[^\n]", wordToFind);
        normalizePtWord(wordToFind);

        TrieNode* wordPayload = getWordPayload(root, wordToFind);
        free(wordToFind);
        break;
      case 2:
        printf("---- Adding word ----\n");
        char* word = malloc(sizeof(char) * 50);
        int wordPolarity = 0;
        printf("Enter word (adjective): ");
        scanf(" %49[^\n]", word);
        normalizePtWord(word);

        printf("\nEnter word polarity: ");
        scanf("%d", &wordPolarity);

        insertWord(root, word, wordPolarity, "adj");

        free(word);
        break;
      case 3:
        printf("---- Updating word polarity ----\n");

        char* wordToUpdate = malloc(sizeof(char) * 50);
        printf("Enter word: ");
        scanf(" %49[^\n]", wordToUpdate);
        normalizePtWord(wordToUpdate);
        int polarity = 0;
        printf("Enter word polarity: ");
        scanf("%d", &wordPolarity);

        int updateResult = editWordPolarity(root, wordToUpdate, wordPolarity);
        printf("updateResult: %s\n", updateResult == 1 ? "success" : "fail");

        free(wordToUpdate);
        break;
      case 4:
        saveTrieOnFileAndFree(root);
        break;
      default:
        printf("Invalid option, try again\n");
        currOpt = 0;
        break;
    }
  }
}

int main() {
  TrieNode* root = createNode('\0');

  loadFileIntoTrie(root);

  menu(root);
  return 0;
}