/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 23, 2017
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "settings.h"
#include "mpihelp.h"
#include "ctools.h"

#define Sendrecv(send, dest, recv, source, tag, count) MPI_Sendrecv(send, count, MPI_DOUBLE, dest, tag, recv, count, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE)

double calculateInitialCondition(double x, double y);
double getInitialInput(int i);
double*** allocateBufferMemory(int numRows, int numCols);
double f(int y_j, int x_i);
bool_t isBorder(int i);

int myRank, commSize;

double*** buffer;
double* topNeighbor;
double* bottomNeighbor;
int myNumRows, myNumCols;

void main() {

  /*************************************************************
  * Setup the environment
  */
  Init(NULL, NULL);
  Size(&commSize);
  Rank(&myRank);

  if(commSize != 1 && commSize % 2 != 0) {
   printf("comm size must be divisible by 2!\n");
   Abort(1);
  }

  if (N % commSize != 0) {
    printf("N must be divisible by comm size!\n");
    Abort(1);
  }
  Barrier();

  /*************************************************************
  * Begin the real task
  */

  myNumRows = N / commSize;
  myNumCols = N;

  buffer = allocateBufferMemory(myNumRows, myNumCols);
  double** swap;

  topNeighbor = malloc(myNumCols * sizeof(double));
  bottomNeighbor = malloc(myNumCols * sizeof(double));

  int i,j,k;
  for (k = 0; k < 2; k++) {
    for (i = 1; i < myNumRows - 1; i++) {
      for (j = 1; j < myNumCols - 1; j++) {
        buffer[k][i][j] = calculateInitialCondition(getInitialInput(i), getInitialInput(j));
      }
    }
    debugCall(printLnMatrix(buffer[k], myNumRows, myNumCols));
    debugCall(printf("\n"));
  }

  int topNeighborRank = myRank == 0 ? MPI_PROC_NULL : myRank - 1;
  int bottomNeighborRank = myRank == commSize - 1 ? MPI_PROC_NULL : myRank + 1;

  for (k = 2; k <= fEnd; k++) {
    Sendrecv(buffer[1][myNumRows-1], bottomNeighborRank, topNeighbor, topNeighborRank, k, myNumCols);
    Sendrecv(buffer[1][0], topNeighborRank, bottomNeighbor, bottomNeighborRank, k, myNumCols);
    for (i = 0; i < myNumRows; i++) {
      for (j = 1; j < myNumCols - 1; j++) {
        buffer[2][i][j] = isBorder(i) ? 0 : f(i, j);
      }
    }
    debugCall(printLnMatrix(buffer[2], myNumRows, myNumCols));
    debugCall(printf("\n"));
    swap = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = buffer[2];
    buffer[2] = swap;
  }

  /*************************************************************
  * Cleanup
  */
  Finalize();
}

double f(int y_j, int x_i) {
  return timeDeltaMultiplier * 0.01 * (
    buffer[1][y_j][x_i-1] + buffer[1][y_j][x_i+1]
    + (y_j == 0 ? topNeighbor[x_i] : buffer[1][y_j-1][x_i])
    + (y_j == myNumRows - 1 ? bottomNeighbor[x_i] : buffer[1][y_j+1][x_i])
    - 4 * buffer[1][y_j][x_i]
  ) + 2 * buffer[1][y_j][x_i] - buffer[0][y_j][x_i];
}

bool_t isBorder(int i) {
  if ((i == 0 && myRank == MASTER)
      || (myRank == commSize - 1 && i == myNumRows - 1)) {
    return true;
  }
  return false;
}

double calculateInitialCondition(double x, double y) {
	double max = (1.0/(2.0*M_PI*sigma*sigma))*exp(-0.5*( ((0.5-mu)/sigma)*((0.5-mu)/sigma) +  ((0.5-mu)/sigma)*((0.5-mu)/sigma)   ));
	double result = (1.0/(2.0*M_PI*sigma*sigma))*exp(-0.5*( ((x-mu)/sigma)*((x-mu)/sigma) +  ((y-mu)/sigma)*((y-mu)/sigma)   ))/max;
	return result;
}

double getInitialInput(int i) {
  return i == 0 ? 0 : i == N - 1 ? 1 : (double) i / (N - 1);
}

double*** allocateBufferMemory(int numRows, int numCols) {
  double*** result = malloc(3 * sizeof(double**));
  int k;
  for (k = 0; k < 3; k++) {
    double** matrix = malloc(numRows * sizeof(double*));
    result[k] = matrix;
    int i, j;
    for (i = 0; i < numRows; i++) {
      matrix[i] = calloc(numCols, sizeof(double));
    }
  }
  return result;
}
