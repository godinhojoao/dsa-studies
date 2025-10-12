/*
Deleting the executable after running:
gcc hashtable-oplexicon.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CEL_PAYLOAD_SIZE 8  // if it changes, also change loadFileIntoHashTable sscanf
#define MAX_CHARS_PER_LINE 100

struct TableCell {
  char key[MAX_CHARS_PER_LINE - CEL_PAYLOAD_SIZE];
  char payload[CEL_PAYLOAD_SIZE];  // concatenate: polarity;class
  int currentlyUsed;
  int removed;
} typedef TableCell;

struct HashTable {
  struct TableCell* rows;
  int filledRows;
  int maxRows;
} typedef HashTable;

int isPrime(int n) {
  if(n < 2) return 0;
  if(n % 2 == 0) return n == 2;
  for(int i = 3; i * i <= n; i += 2)
    if(n % i == 0) return 0;
  return 1;
}

int nextPrime(int n) {
  if(n <= 2) return 2;
  if(n % 2 == 0) n++;
  while(!isPrime(n)) n += 2;
  return n;
}

int primaryHash(const char* s, int tableSize) {  // polynomialRollingHash
  // to read more about it: https://www.geeksforgeeks.org/dsa/string-hashing-using-polynomial-rolling-hash-function/
  int p = 53;
  int hashVal = 0;
  int pPow = p;
  for(int i = 0; s[i] != '\0'; i++) {
    hashVal = (hashVal + s[i] * pPow) % tableSize;
    pPow = (pPow * p) % tableSize;  // % to prevent overflow and keep values within tableSize
  }
  return hashVal;
}

int secondaryHash(char* word, int tableSize) {  // sumOfCharsCode
  int i = 0;
  int hash = 0;
  while(word[i] != '\0') {
    hash += word[i];
    i++;
  }
  hash %= tableSize;
  if(hash == 0) {
    return 1;
  }
  return hash;
}

int doubleHash(int hash1, int hash2, int attempt, int tableSize) {
  return (hash1 + (attempt * hash2)) % tableSize;
}

HashTable* createTable(int limit) {
  HashTable* table = malloc(sizeof(HashTable));
  table->maxRows = nextPrime(limit);  // ensure it is prime so we don't share common divisors on double hash (to avoid cycles and do not cover all table)
  table->filledRows = 0;
  table->rows = (TableCell*)malloc(sizeof(TableCell) * table->maxRows);
  for(int i = 0; i < table->maxRows; i++) {
    table->rows[i].removed = 0;
    table->rows[i].currentlyUsed = 0;
  }
  return table;
}

int isInUse(HashTable* table, int hash) {
  return table->rows[hash].currentlyUsed == 1 && table->rows[hash].removed == 0 ? 1 : 0;
}

int insertOnTable(HashTable** tablePtr, char* word, char* payload) {
  HashTable* table = *tablePtr;
  if(table->filledRows == table->maxRows) {
    HashTable* newTable = createTable(table->maxRows * 2);
    for(int i = 0; i < table->maxRows; i++) {
      if(isInUse(table, i)) {
        insertOnTable(&newTable, table->rows[i].key, table->rows[i].payload);
      }
    }

    free(table->rows);
    free(table);
    *tablePtr = newTable;
    table = *tablePtr;
  }

  int hash1 = primaryHash(word, table->maxRows);
  if(!isInUse(table, hash1)) {
    table->rows[hash1].currentlyUsed = 1;
    table->rows[hash1].removed = 0;
    strcpy(table->rows[hash1].payload, payload);
    strcpy(table->rows[hash1].key, word);
    table->filledRows += 1;
    return 1;
  }

  int hash2 = secondaryHash(word, table->maxRows);
  if(!isInUse(table, hash2)) {
    table->rows[hash2].currentlyUsed = 1;
    table->rows[hash2].removed = 0;
    strcpy(table->rows[hash2].payload, payload);
    strcpy(table->rows[hash2].key, word);
    table->filledRows += 1;
    return 1;
  }

  for(int attempt = 1; attempt <= table->maxRows; attempt++) {
    int hash3 = doubleHash(hash1, hash2, attempt, table->maxRows);
    if(!isInUse(table, hash3)) {
      table->rows[hash3].currentlyUsed = 1;
      table->rows[hash3].removed = 0;
      strcpy(table->rows[hash3].payload, payload);
      strcpy(table->rows[hash3].key, word);
      table->filledRows += 1;
      return 1;
    }
  }

  return -1;
}

TableCell findOnTable(HashTable* table, char* word) {
  TableCell empty = {0};  // 0 initialized instead of returning a wild pointer
  if(table->filledRows == 0) {
    return empty;
  }

  int hash1 = primaryHash(word, table->maxRows);
  if(isInUse(table, hash1) && strcmp(table->rows[hash1].key, word) == 0) {
    return table->rows[hash1];
  }

  int hash2 = secondaryHash(word, table->maxRows);
  if(isInUse(table, hash2) && strcmp(table->rows[hash2].key, word) == 0) {
    return table->rows[hash2];
  }

  for(int attempt = 0; attempt <= table->maxRows; attempt++) {
    int hash3 = doubleHash(hash1, hash2, attempt, table->maxRows);
    if(isInUse(table, hash3) && strcmp(table->rows[hash3].key, word) == 0) {
      return table->rows[hash3];
    }
  }

  return empty;
}

void normalizePtWord(char* s) {
  int currIndex = 0;

  for(size_t i = 0; s[i]; ++i) {
    unsigned char c = (unsigned char)s[i];

    if(c == 0xC3) {  // handle 2-byte UTF-8 accented letters
      unsigned char d = (unsigned char)s[i + 1];

      switch(d) {
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
          s[currIndex++] = (d == 0xA7 || d == 0x87) ? 'c' : 'a';
          break;

          // è é ê È É Ê
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0x88:
        case 0x89:
        case 0x8A:
          s[currIndex++] = 'e';
          break;

          // ì í î Ì Í Î
        case 0xAC:
        case 0xAD:
        case 0xAE:
        case 0x8C:
        case 0x8D:
        case 0x8E:
          s[currIndex++] = 'i';
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
          s[currIndex++] = 'o';
          break;

          // ù ú û Ù Ú Û
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0x99:
        case 0x9A:
        case 0x9B:
          s[currIndex++] = 'u';
          break;

        default:
          break;
      }

      i++;  // skip next byte
      continue;
    }

    s[currIndex++] = s[i];
  }

  s[currIndex] = '\0';
}

void loadFileIntoHashTable(HashTable** tablePtr) {
  HashTable* table = *tablePtr;
  FILE* filePtr;
  filePtr = fopen("OpLexicon.txt", "r");
  if(filePtr == NULL) {
    printf("error reading file.\n");
    exit(1);
  }

  char line[MAX_CHARS_PER_LINE];
  int count = 0;  // just to see file being loaded
  while(fgets(line, sizeof(line), filePtr)) {
    int len = strlen(line);
    if(len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';  // remove \n putting \0 in the place

    char word[MAX_CHARS_PER_LINE - CEL_PAYLOAD_SIZE], classAndPolarity[CEL_PAYLOAD_SIZE];

    if(sscanf(line, "%49[^,],%7[^\n]", word, classAndPolarity) == 2) {
      normalizePtWord(word);
      int insertRes = insertOnTable(tablePtr, word, classAndPolarity);
      if(insertRes != 1) {
        printf("Insert failed\n");
        exit(1);
      }
      count++;
    }
  }

  fclose(filePtr);
}

int removeWordFromTable(HashTable* table, char* word) {
  if(table->filledRows == 0) {
    printf("can't remove, table is empty\n");
    return -1;
  }

  int hash1 = primaryHash(word, table->maxRows);
  if(isInUse(table, hash1) && strcmp(table->rows[hash1].key, word) == 0) {
    table->rows[hash1].currentlyUsed = 0;
    table->rows[hash1].removed = 1;
    table->filledRows -= 1;
    return 1;
  }

  int hash2 = secondaryHash(word, table->maxRows);
  if(isInUse(table, hash2) && strcmp(table->rows[hash2].key, word) == 0) {
    table->rows[hash2].currentlyUsed = 0;
    table->rows[hash2].removed = 1;
    table->filledRows -= 1;
    return 1;
  }

  for(int attempt = 1; attempt <= table->maxRows; attempt++) {
    int hash3 = doubleHash(hash1, hash2, attempt, table->maxRows);
    if(isInUse(table, hash3) && strcmp(table->rows[hash3].key, word) == 0) {
      table->rows[hash3].currentlyUsed = 0;
      table->rows[hash3].removed = 1;
      table->filledRows -= 1;
      return 1;
    }
  }

  return -1;
}

int main() {
  HashTable* table = createTable(10);

  loadFileIntoHashTable(&table);

  char currUserInputWord[MAX_CHARS_PER_LINE] = {0};
  char currUserInputPayload[CEL_PAYLOAD_SIZE] = {0};
  int currOpt = -1;
  while(currOpt != 4) {
    printf("\nOptions:\n1 insert word\n2 search word\n3 remove word\n4 exit\n");
    scanf("%d", &currOpt);
    switch(currOpt) {
      case 1:
        printf("Enter a word without special chars: ");
        scanf("%s", currUserInputWord);
        printf("Enter class and polarity separated by comma without special chars: ");
        scanf("%s", currUserInputPayload);

        int insertRes = insertOnTable(&table, currUserInputWord, currUserInputPayload);
        printf(insertRes == 1 ? "Insert success\n" : "Insert failed\n");
        strcpy(currUserInputWord, "");
        break;
      case 2:
        printf("Enter a word without special chars: ");
        scanf("%s", currUserInputWord);

        TableCell res = findOnTable(table, currUserInputWord);
        if(res.currentlyUsed) {
          printf("Word key: %s\n", res.key);
          printf("Word payload: %s\n", res.payload);
        } else {
          printf("word not found\n");
        }
        strcpy(currUserInputWord, "");
        break;
      case 3:
        printf("Enter a word without special chars: ");
        scanf("%s", currUserInputWord);

        int removeRes = removeWordFromTable(table, currUserInputWord);
        printf(removeRes == 1 ? "Remove success\n" : "Remove failed\n");
        strcpy(currUserInputWord, "");
        break;
        break;
      default:
        printf("invalid option\n");
        break;
    }
  }
  free(table->rows);
  free(table);

  return 0;
}
