/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 23, 2017
*************************************************************/

#ifndef bool_t_DEFINED
  #define bool_t_DEFINED
  typedef enum { false, true } bool_t;
#endif

#ifdef debug
  #define debugCall(x) do { x; } while(0)
  #define debugPrint(format, args...) do { printf(format, ## args); } while(0)
#else
  #define debugCall(x)
  #define debugPrint(format, args...)
#endif

void printArray(double* array, int length);
void printlnArray(double* array, int length);
void printMatrix(double** matrix, int numRows, int numCols);
void printLnMatrix(double** matrix, int numRows, int numCols);

void printArray(double* array, int length) {
  printf("[");
  int i;
  for (i = 0; i < length; i++) {
    if (i != 0) {
      printf("  ");
    }
    printf("%f", array[i]);
  }
  printf("]");
}

void printlnArray(double* array, int length) {
  printArray(array, length);
  printf("\n");
}

void printMatrix(double** matrix, int numRows, int numCols) {
  printf("[");
  int i;
  for (i = 0; i < numRows; i++) {
    if (i != 0) {
      printf(" ");
    }
    printArray(matrix[i], numCols);
    if (i != numRows - 1) {
      printf("\n");
    }
  }
  printf("]");
}

void printLnMatrix(double** matrix, int numRows, int numCols) {
  printMatrix(matrix, numRows, numCols);
  printf("\n");
}
