
/*
-----> missing features
[ ] - insert in any direction randomly
[ ] - show random cells
[ ] - get words randomly, not it in order
*/

// gcc crosswords.c -o test && ./test && rm -rf ./test
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Cell {
  char val;
  int isVisible;
  int direction;  // 0=horizontal, 1 = vertical, 2 = diagonal, -1 = random doesn't matter
  int isStartOfWord;
  int isEndOfWord;
  int wordIndex;
} Cell;

typedef struct WordAuxForSearch {
  int startCol;
  int startRow;
  char* word;
} WordAuxForSearch;

typedef struct Game {
  int wordsAmount;
  int foundWords;
  int dimension;
  Cell** crosswords;
  WordAuxForSearch* wordsAuxForSearch;
} Game;

void freeGameState(Game* gameState) {
  for(int i = 0; i < gameState->wordsAmount; i++) {
    free(gameState->wordsAuxForSearch[i].word);
  }
  free(gameState->wordsAuxForSearch);
  for(int i = 0; i < gameState->dimension; i++) {
    free(gameState->crosswords[i]);
  }
  free(gameState);
}

char getRandomChar() {
  int letter = rand() % 26;
  return 'a' + letter;
}

void createCrosswords(Game* gameState) {
  int dimension = gameState->dimension;

  gameState->crosswords = malloc(sizeof(Cell*) * dimension);

  for(int i = 0; i < dimension; i++) {
    gameState->crosswords[i] = malloc(sizeof(Cell) * dimension);

    int j = 0;

    if(i < gameState->wordsAmount) {  // n-1 words
      while(j < strlen(gameState->wordsAuxForSearch[i].word) && j < dimension) {
        gameState->crosswords[i][j].val = gameState->wordsAuxForSearch[i].word[j];
        gameState->crosswords[i][j].isVisible = 0;  // make it randomly (we need x visible decide x based on n)
        gameState->crosswords[i][j].direction = 0;
        gameState->crosswords[i][j].isStartOfWord = j == 0;
        gameState->crosswords[i][j].isEndOfWord = j + 1 == dimension;
        if(j == 0) {
          gameState->wordsAuxForSearch[i].startCol = i;
          gameState->wordsAuxForSearch[i].startRow = j;
        }
        j++;
      }
    }

    while(j < dimension) {  // fill remaining empty slots with a random char
      gameState->crosswords[i][j].val = getRandomChar();
      gameState->crosswords[i][j].direction = -1;  // random = invalid direction
      gameState->crosswords[i][j].isVisible = 1;  // make it randomly (we need x visible decide x based on n)
      gameState->crosswords[i][j].isStartOfWord = 0;
      gameState->crosswords[i][j].isEndOfWord = 0;
      j++;
    }
  }
}

void markAsFound(Game* gameState, WordAuxForSearch foundWord) {
  // TODO: use direction here (now its only horizontal so i++) --> now it has a fixed col
  for(int j = 0; j < strlen(foundWord.word); j++) {
    gameState->crosswords[foundWord.startCol][foundWord.startRow + j].isVisible = 1;
  }
}

void searchWordAndUpdateTable(Game* gameState, char* word) {
  int foundWordIndex = -1;

  for(int i = 0; i < gameState->wordsAmount; i++) {
    // Could use a hashtable here for exact match - maybe other time..
    if(strcmp(gameState->wordsAuxForSearch[i].word, word) == 0) {
      foundWordIndex = i;
      continue;
    }
  }

  if(foundWordIndex == -1) {
    printf("Word: %s not found.\n", word);
  } else {
    printf("Found word: %s.\n", word);
    gameState->foundWords += 1;
    markAsFound(gameState, gameState->wordsAuxForSearch[foundWordIndex]);
  }
}

void showTable(Game* gameState, int forceAllVisible) {
  int dimension = gameState->dimension;
  printf("\nCrosswords current state\n");
  for(int i = 0; i < dimension; i++) {
    for(int j = 0; j < dimension; j++) {
      Cell currCell = gameState->crosswords[i][j];
      if(currCell.isVisible || forceAllVisible) {
        printf("| _%c_ ", gameState->crosswords[i][j].val);
      } else {
        printf("| _*_ ");
      }
      printf(j + 1 == dimension ? "|" : "");
    }
    printf("\n");
  }
  printf("\n");
}

void menu(Game* gameState) {
  int giveUp = 0;
  int playing = 1;

  showTable(gameState, 0);

  while(playing && !giveUp && gameState->foundWords != gameState->wordsAmount) {
    char word[30];
    printf("Press 'f' to give up and see all words, or enter a word: ");
    scanf("%s", word);
    printf("\n");

    giveUp = word[0] == 'f' && (word[1] == ' ' || word[1] == '\0' || word[1] == '\n');

    if(!giveUp) {
      searchWordAndUpdateTable(gameState, word);
    }

    showTable(gameState, giveUp);
  }

  if(!giveUp) {
    printf("Congratulations you're a winner!!!\n");
  }
}

int main() {
  srand(time(NULL));

  Game* gameState = malloc(sizeof(Game));
  // int n = 3;
  int n = 0;
  while(n < 3) {
    printf("give me the size of crosswords (3 <= n <= 10): ");
    scanf("%d", &n);
    printf("\n");

    if(n < 3) {
      printf("I've said (3 <= n <= 10)\n");
    }
  }
  gameState->dimension = n;
  gameState->wordsAmount = gameState->dimension - 1;
  gameState->wordsAuxForSearch = malloc(sizeof(WordAuxForSearch) * gameState->wordsAmount);  // n-1 words because we need space to add random chars

  FILE* filePtr = fopen("words.txt", "r");
  if(!filePtr) {
    perror("fopen");
    return 1;
  }
  char currWordBuffer[30];

  int insertedWordIndex = 0;
  while(fgets(currWordBuffer, sizeof(currWordBuffer), filePtr) != NULL && insertedWordIndex < gameState->wordsAmount) {
    size_t length = strlen(currWordBuffer);
    if(length > 0 && currWordBuffer[length - 1] == '\n') currWordBuffer[length - 1] = '\0';  // remove newline
    if(strlen(currWordBuffer) < gameState->dimension + 1) {  // +1 for \0
      gameState->wordsAuxForSearch[insertedWordIndex].word = malloc(strlen(currWordBuffer) + 1);  // +1 for \0
      strcpy(gameState->wordsAuxForSearch[insertedWordIndex].word, currWordBuffer);
      insertedWordIndex++;
    }
  }
  fclose(filePtr);

  createCrosswords(gameState);
  menu(gameState);

  freeGameState(gameState);

  return 0;
}
