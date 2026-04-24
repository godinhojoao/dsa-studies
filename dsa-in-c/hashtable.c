/*
Deleting the executable after running:
gcc hashtable.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CEL_PAYLOAD_SIZE 50
#define MAX_CHARS_PER_LINE 100

typedef struct {
  char key[MAX_CHARS_PER_LINE - CEL_PAYLOAD_SIZE];
  char payload[CEL_PAYLOAD_SIZE];
  int currentlyUsed;
  int removed;
} TableCell;

typedef struct {
  TableCell* rows;
  int filledRows;
  int maxRows;
} HashTable;

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

int primaryHash(const char* s, int tableSize) {
  int p = 53;
  int hashVal = 0;
  int pPow = p;
  for(int i = 0; s[i] != '\0'; i++) {
    hashVal = (hashVal + s[i] * pPow) % tableSize;
    pPow = (pPow * p) % tableSize;
  }
  return hashVal;
}

int secondaryHash(char* word, int tableSize) {
  int i = 0;
  int hash = 0;
  while(word[i] != '\0') {
    hash += word[i];
    i++;
  }
  hash %= tableSize;
  if(hash == 0) return 1;
  return hash;
}

int doubleHash(int hash1, int hash2, int attempt, int tableSize) {
  return (hash1 + (attempt * hash2)) % tableSize;
}

HashTable* createTable(int limit) {
  HashTable* table = malloc(sizeof(HashTable));
  table->maxRows = nextPrime(limit);
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
      if(isInUse(table, i)) insertOnTable(&newTable, table->rows[i].key, table->rows[i].payload);
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
  TableCell empty = {0};
  if(table->filledRows == 0) return empty;

  int hash1 = primaryHash(word, table->maxRows);
  if(isInUse(table, hash1) && strcmp(table->rows[hash1].key, word) == 0)
    return table->rows[hash1];

  int hash2 = secondaryHash(word, table->maxRows);
  if(isInUse(table, hash2) && strcmp(table->rows[hash2].key, word) == 0)
    return table->rows[hash2];

  for(int attempt = 0; attempt <= table->maxRows; attempt++) {
    int hash3 = doubleHash(hash1, hash2, attempt, table->maxRows);
    if(isInUse(table, hash3) && strcmp(table->rows[hash3].key, word) == 0)
      return table->rows[hash3];
  }
  return empty;
}

int removeWordFromTable(HashTable* table, char* word) {
  if(table->filledRows == 0) return -1;

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
  char word[MAX_CHARS_PER_LINE] = {0};
  char payload[CEL_PAYLOAD_SIZE] = {0};
  int opt = -1;

  while(opt != 4) {
    printf("\nOptions:\n1 insert\n2 search\n3 remove\n4 exit\n");
    scanf("%d", &opt);
    switch(opt) {
      case 1:
        printf("Enter word: ");
        scanf("%s", word);
        printf("Enter payload: ");
        scanf("%s", payload);
        printf(insertOnTable(&table, word, payload) == 1 ? "Inserted\n" : "Insert failed\n");
        break;
      case 2: {
        printf("Enter word: ");
        scanf("%s", word);
        TableCell res = findOnTable(table, word);
        if(res.currentlyUsed)
          printf("Found: %s => %s\n", res.key, res.payload);
        else
          printf("Not found\n");
        break;
      }
      case 3:
        printf("Enter word: ");
        scanf("%s", word);
        printf(removeWordFromTable(table, word) == 1 ? "Removed\n" : "Remove failed\n");
        break;
      case 4:
        break;
      default:
        printf("Invalid option\n");
        break;
    }
  }

  free(table->rows);
  free(table);
  return 0;
}
