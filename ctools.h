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
  #define debugPrint(format, args...) do { printf(format, ## args); } while(0)
#else
  #define debugPrint(format, args...)
#endif

void printArray(double* array, int length);
void printlnArray(double* array, int length);

void printArray(double* array, int length) {
  printf("[");
  int i;
  for (i = 0; i < length; i++) {
    if (i != 0) {
      printf("  ");
    }
    printf("%g", array[i]);
  }
  printf("]");
}

void printlnArray(double* array, int length) {
  printArray(array, length);
  printf("\n");
}
