/*
Deleting the executable after running:
gcc 2D-array-and-pointers.c -o test && ./test && rm -rf ./test

Reference:
- https://www.log2base2.com/C/pointer/2d-array-and-pointers-in-c.html
- https://www.geeksforgeeks.org/how-to-declare-a-two-dimensional-array-of-pointers-in-c/

Important:
- When we define define a matrix[3][3] -> 2D array
-- matrix itself is a pointer to matrix[0] (TYPE: (int*)[3]) - pointer to array of 3 ints
-- *matrix is a pointer to matrix[0][0] (TYPE: int*) - pointer to an int
-- **matrix is the VALUE of matrix[0][0] (TYPE: int)
*/

#include <stdio.h>
#include <stdlib.h>

void exampleHandlingMatrixBasic();
void exampleHandlingPointerToPointersMatrix();

int main() {
  printf("\n--------exampleHandlingMatrixBasic--------\n");
  exampleHandlingMatrixBasic();

  printf("\n--------exampleHandlingPointerToPointersMatrix--------\n");
  exampleHandlingPointerToPointersMatrix();

  return 0;
}

void exampleHandlingMatrixBasic() {
  int matrix[3][3] = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
  };
  printf("sizeof(matrix): %zu\n", sizeof(int));  // 4 bytes (32bit)
  printf("sizeof(matrix): %zu\n", sizeof(matrix));  // 36 bytes
  printf("matrix items amount: %zu\n", sizeof(matrix) / sizeof(int));  // 9 items (correct)

  printf("----- \n");
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      printf("matrix[%d][%d]: %d\n", i, j, matrix[i][j]);
    }
  }
  printf("----- \n");

  // matrix is a pointer to an array of 3 ints (a row) -> matrix[0]
  printf("**matrix: %d\n", **matrix);  // 1
  int (*secondRowPtr)[3] = matrix + 1;  // skips 3 ints (first array)
  printf("*secondRowPtr[0]: %d\n", *secondRowPtr[0]);  // 4
  printf("**secondRowPtr: %d\n", **secondRowPtr);  // 4
  printf("----- \n");

  // *matrix is matrix[0], a pointer to the first row, so *(*matrix + 1) = matrix[0][1]
  printf("*(*matrix + 1): %d\n", *(*matrix + 1));  // 2

  // matrix + 1 is matrix[1] (second row), so **(matrix + 1) = matrix[1][0]
  printf("**(matrix + 1): %d\n", **(matrix + 1));  // 4

  printf("----- \n");

  printf("Or we could simply use pointer arithmetic to traverse the matrix:\n");
  int *currItem = *matrix;  // pointer to matrix[0][0]
  for(int i = 0; i <= 8; i++) {
    printf("matrix %d\n", *currItem);
    currItem += 1;
  }
  printf("----- \n");
}

void exampleHandlingPointerToPointersMatrix() {
  int rows = 3;
  int columns = 3;
  int **matrix = (int **)malloc(sizeof(int *) * rows);  // rows
  for(int i = 0; i < rows; i++) {
    matrix[i] = (int *)malloc(columns * sizeof(int));  // columns
    matrix[i][0] = 1;
    matrix[i][1] = 2;
  }

  printf("matrix[0][0]: %d -- ", matrix[0][0]);  // 1
  printf("**matrix: %d\n", **matrix);  // 1
  printf("matrix[0][1]: %d -- ", matrix[0][1]);  // 2
  printf("*(*matrix+1): %d\n", *(*matrix + 1));  // 2

  for(int i = 0; i < rows; i++) {
    free(matrix[i]);  // free rows
  }
  free(matrix);  // free array of pointers
}