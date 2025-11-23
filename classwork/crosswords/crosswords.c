
/*
-> improvements
[ ] - use a hashtable for WordAuxForSearch and then change all places including: searchWordAndUpdateTable
support for directions:
[ ] - horizontal right left
[ ] - vertical bottom top
[ ] - diagonal all directions (top bottom, bottom top, left and right in each) -> 4 directions
[ ] - fix to avoid O(n^2) insertWordByPosition
*/

// gcc crosswords.c -o test && ./test && rm -rf ./test
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define POSSIBLE_DIRECTIONS_AMOUNT 2
#define HORIZONTAL_LEFT_RIGHT_DIRECTION 0
#define VERTICAL_TOP_BOTTOM_DIRECTION 1
#define RANDOM_CHAR_EMPTY_DIRECTION -1
int DIRECTIONS_DIFFERENCES_ARR[POSSIBLE_DIRECTIONS_AMOUNT][2] = {
    // (row, col)
    {0, 1},  // HORIZONTAL_LEFT_RIGHT_DIRECTION
    {1, 0},  // VERTICAL_TOP_BOTTOM_DIRECTION
};

typedef struct Cell {
  char val;
  int isVisible;
  int direction;  // 0 = horizontal, 1 = vertical, -1 = random char doesn't matter
  int isStartOfWord;
  int isEndOfWord;
} Cell;

typedef struct WordAuxForSearch {
  int startCol;
  int startRow;
  int direction;
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
  free(gameState->crosswords);
  free(gameState);
}

char getRandomChar() {
  int letter = rand() % 26;
  return 'a' + letter;
}

int insertWordByPosition(Game* gameState, int direction, WordAuxForSearch* wordAux) {
  int wordLength = strlen(wordAux->word);
  int dimension = gameState->dimension;
  int rowIncrement = DIRECTIONS_DIFFERENCES_ARR[direction][0];
  int colIncrement = DIRECTIONS_DIFFERENCES_ARR[direction][1];

  for(int startRow = 0; startRow < dimension; startRow++) {
    for(int startCol = 0; startCol < dimension; startCol++) {
      int lastCharOfWordIndex = wordLength - 1;
      int currentRow = startRow + (rowIncrement * lastCharOfWordIndex);
      int currentCol = startCol + (colIncrement * lastCharOfWordIndex);
      if(currentRow >= dimension || currentCol >= dimension) continue;  // avoid out of bound for curr supported directions

      // check if space is free
      int canPlaceWordHere = 1;
      for(int letterIndex = 0; letterIndex < wordLength; letterIndex++) {
        int currentRow = startRow + (letterIndex * rowIncrement);
        int currentCol = startCol + (letterIndex * colIncrement);
        if(gameState->crosswords[currentRow][currentCol].direction != -1) {  // direction -1 means filled
          canPlaceWordHere = 0;
          break;
        }
      }

      if(canPlaceWordHere) {
        // insert word
        wordAux->direction = direction;
        wordAux->startRow = startRow;
        wordAux->startCol = startCol;
        for(int letterIndex = 0; letterIndex < wordLength; letterIndex++) {
          int currentRow = startRow + (letterIndex * rowIncrement);
          int currentCol = startCol + (letterIndex * colIncrement);
          gameState->crosswords[currentRow][currentCol].val = wordAux->word[letterIndex];
          gameState->crosswords[currentRow][currentCol].direction = direction;
          gameState->crosswords[currentRow][currentCol].isVisible = 0;
          gameState->crosswords[currentRow][currentCol].isStartOfWord = (letterIndex == 0);
          gameState->crosswords[currentRow][currentCol].isEndOfWord = (letterIndex == wordLength - 1);
        }
        return 1;
      }
    }
  }

  return 0;
}

int insertWordRandomly(Game* gameState, WordAuxForSearch* wordAux) {
  int possibleDirectionsArr[POSSIBLE_DIRECTIONS_AMOUNT] = {0, 1};
  int insertedSuccessfully = 0;
  int i = 0;

  // testing all directions randomly one by one until one works
  while(i < POSSIBLE_DIRECTIONS_AMOUNT) {
    int lastValidDirectionPosition = POSSIBLE_DIRECTIONS_AMOUNT - 1 - i;
    int currDirection = rand() % (lastValidDirectionPosition + 1);
    // e.g 4 % 3 = 1, if we have 3 positions and our last valid index is 1, we want results from 0 to 1, so we use % 2 because: n % 2 <= 1
    if(insertWordByPosition(gameState, possibleDirectionsArr[currDirection], wordAux)) {
      insertedSuccessfully = 1;
      break;
    }
    int lastDirectionPosition = possibleDirectionsArr[lastValidDirectionPosition];
    possibleDirectionsArr[lastValidDirectionPosition] = possibleDirectionsArr[currDirection];
    possibleDirectionsArr[currDirection] = lastDirectionPosition;
    i++;
  }

  return insertedSuccessfully;
}

void createCrosswords(Game* gameState) {
  int dimension = gameState->dimension;

  gameState->crosswords = malloc(sizeof(Cell*) * dimension);
  for(int row = 0; row < dimension; row++) {
    gameState->crosswords[row] = malloc(sizeof(Cell) * dimension);

    for(int col = 0; col < dimension; col++) {
      gameState->crosswords[row][col].val = getRandomChar();
      gameState->crosswords[row][col].isVisible = 1;
      gameState->crosswords[row][col].direction = RANDOM_CHAR_EMPTY_DIRECTION;
      gameState->crosswords[row][col].isStartOfWord = 0;
      gameState->crosswords[row][col].isEndOfWord = 0;
    }
  }

  for(int wordIndex = 0; wordIndex < gameState->wordsAmount; wordIndex++) {
    insertWordRandomly(gameState, &gameState->wordsAuxForSearch[wordIndex]);
  }
}

void markAsFound(Game* gameState, WordAuxForSearch foundWord) {
  int rowIncrement = DIRECTIONS_DIFFERENCES_ARR[foundWord.direction][0];
  int colIncrement = DIRECTIONS_DIFFERENCES_ARR[foundWord.direction][1];

  for(int j = 0; j < strlen(foundWord.word); j++) {
    gameState->crosswords[foundWord.startRow + (j * rowIncrement)][foundWord.startCol + (j * colIncrement)].isVisible = 1;
  }
}

void searchWordAndUpdateTable(Game* gameState, char* word) {
  int foundWordIndex = -1;

  for(int i = 0; i < gameState->wordsAmount; i++) {
    if(strcmp(gameState->wordsAuxForSearch[i].word, word) == 0) {
      foundWordIndex = i;
      break;
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
  gameState->foundWords = 0;
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
