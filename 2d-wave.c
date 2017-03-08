/*******************************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Mar 1, 2017
*
* Compiler options (alternately edit settings.h):
*   -DN=x                   The number of permutations to calculate.
*                             (i.e. Matrix of size NxN)
*                             Must be positive integer evenly divisible by comm
*                             size.
*   -DfEnd=x                The number of time steps to calculate.
*                             Must be positive integer.
*   -Dsigma=x               Defines the overall shape of the initial conditions.
*                             Examples:
*                               0.01  A tight point
*                               0.1   A wide point
*   -DtimeDeltaMultiplier=x A multiplier to increase or decrease the time step
*                             delta.
*                             Negative values and values over ~50 will be
*                             unstable.
*   -Ddebug                 A flag to indicate debug text should be printed to
*                             the console.
*   -Doutput                A flag to indicate pgm files should be generated for
*                             each time step calculation and an animated gif be
*                             be generated to show all calculations.
*   -DshowStatus            A flag that causes a visual status bar to display in
*                             the console to indicate the calculation progress.
*   -Dtiming                A flag that causes timing information to be printed
*                             to the console and timings.txt
*   -DoptimizationLevel=x   A string that will be printed with timing data to
*                             to track effect of compiler optimization levels.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "settings.h"
#include "mpihelp.h"
#include "ctools.h"
#include "pgmout.h"

// Define some ease of use "functions"
#define Sendrecv(send, dest, recv, source, tag, count) MPI_Sendrecv(send, \
    count, MPI_DOUBLE, dest, tag, recv, count, MPI_DOUBLE, source, tag, \
    MPI_COMM_WORLD, MPI_STATUS_IGNORE)
#define Send(send, count, dest, tag) MPI_Send(send, count, MPI_DOUBLE, dest, \
    tag, MPI_COMM_WORLD)
#define Recv(recv, count, source, tag) MPI_Recv(recv, count, MPI_DOUBLE, \
    source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
#define Gather(send, count, recv) MPI_Gather(send, count, MPI_DOUBLE, recv, \
    count, MPI_DOUBLE, MASTER, MPI_COMM_WORLD)
#define ReduceMax(elapsed, longest) MPI_Reduce(elapsed, longest, 1, \
    MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);

/**
 * Calculates the initial condition for the wave for a given position.
 */
double calculateInitialCondition(double x, double y);
/**
 * Calculates the input values for the initial condition, based on col index.
 */
double getInitialInput(int i);
/**
 * Allocates memory for 3 pointers to "matrix" pointers of the given size.
 */
double*** allocateBufferMemory(int numRows, int numCols);
/**
 * Allocates memory for a "matrix" pointer of the given size.
 */
double** allocateMatrix(int numRows, int numCols);
/**
 * The wave function.
 */
double f(int y_j, int x_i);
/**
 * Returns true if the given row index is on the border of the overall matrix.
 */
bool_t isBorder(int i);
/**
 * Handles printing debug text and/or file output for the given time step.
 */
void debugAndOutput(int fStep);
/**
 * Creates an animation gif from the existing pgm files.
 */
void createAnimation();

int myRank, commSize;

// Declare global variables.
// Theoretically reduces the function call time overhead.
double*** buffer;
double* topNeighbor;
double* bottomNeighbor;
int myNumRows, myNumCols;
double startTime, endTime, timeElapsed;


int main() {

  /*************************************************************
  * Setup the environment
  */
  Init(NULL, NULL);
  Size(&commSize);
  Rank(&myRank);

  // Any comm size is allowed as long as it divides N evenly.
  if (N % commSize != 0) {
    printf("N must be divisible by comm size!\n");
    Abort(1);
  }

  // Deletes all of the pgm files in the output directory.
  // Requires compiling with -Doutput
  outputCall(if (isMaster(myRank)) { cleanupOutput(); });

  // Starts the main task all at once.
  Barrier();

  /*************************************************************
  * Begin the main task
  */

  startTime = Time();

  // In this implementation, only the rows are divided amongst the procs.
  myNumRows = N / commSize;
  myNumCols = N;

  buffer = allocateBufferMemory(myNumRows, myNumCols);
  double** swap; // A swap pointer for changing out the matrices in the buffer.

  int i,j,k;

  // Peform the initial calculations, 2 matrices containing initial conditions.
  for (k = 0; k < 2; k++) {
    for (i = 0; i < myNumRows; i++) {
      for (j = 1; j < myNumCols - 1; j++) {
        // Border values are always 0.
        buffer[k][i][j] = isBorder(i) ? 0 : calculateInitialCondition(
            getInitialInput(i + (myRank * myNumRows)), getInitialInput(j));
      }
    }

    // Prints a visual status bar to show progress of the operation.
    // Requires compiling with -DshowStatus
    statusCall(
      if (!gatherEnabled && isMaster(myRank)) {
        printStatus(k, fEnd);
      }
    );

    // Executes the code that gathers all the data onto root proc in order to
    // print debug output or file output.
    // Requires compiling with -Doutput or -Ddebug
    gather(debugAndOutput(k));
  }

  // Determine the proc ranks of neighbors.
  int topNeighborRank = myRank == 0 ? MPI_PROC_NULL : myRank - 1;
  int bottomNeighborRank = myRank == commSize - 1 ? MPI_PROC_NULL : myRank + 1;
  // Storage for the neighbors calculations.
  topNeighbor = malloc(myNumCols * sizeof(double));
  bottomNeighbor = malloc(myNumCols * sizeof(double));

  // Performs calculations for time step >= 2.
  for (k = 2; k <= fEnd; k++) {
    // Send and receive all the required neighbor data.
    Sendrecv(buffer[1][myNumRows-1], bottomNeighborRank, topNeighbor,
        topNeighborRank, k, myNumCols);
    Sendrecv(buffer[1][0], topNeighborRank, bottomNeighbor,
        bottomNeighborRank, k, myNumCols);

    for (i = 0; i < myNumRows; i++) {
      for (j = 1; j < myNumCols - 1; j++) {
        // Border values are always 0
        buffer[2][i][j] = isBorder(i) ? 0 : f(i, j);
      }
    }

    // Prints a visual status bar to show progress of the operation.
    // Requires compiling with -DshowStatus
    statusCall(
      if (!gatherEnabled && isMaster(myRank)) {
        printStatus(k, fEnd);
      }
    );

    // Executes the code that gathers all the data onto root proc in order to
    // print debug output or file output.
    // Requires compiling with -Doutput or -Ddebug
    gather(debugAndOutput(k));

    // Shift the matrices up in the buffer.
    swap = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = buffer[2];
    buffer[2] = swap;
  }

  // Prints a new line after the completed status bar.
  // Requires compiling with -DshowStatus
  statusCall(if (isMaster(myRank)) { printf("\n"); });

  endTime = Time();

  // Calculate elapsed time and send the largest time value to the root proc.
  timeElapsed = endTime - startTime;
  double longestElapsedTime;
  ReduceMax(&timeElapsed, &longestElapsedTime);

  // Prints the elapsed time to the console and appends detailed timing data to
  // timings.txt.
  // Requires compiling with -Dtiming
  timingCall(if (isMaster(myRank)) {
    printf("Time elapsed: %g seconds\n", longestElapsedTime);
    writeTimingFile("timings.txt", N, fEnd, commSize, longestElapsedTime);
  });

  // Creates an animated gif of the wave function.
  // Requires compiling with -Doutput
  outputCall(createAnimation());

  /*************************************************************
  * Cleanup
  */
  Finalize();

  return 0;
}

void debugAndOutput(int fStep) {
  int i,j,k;
  k = fStep > 2 ? 2 : fStep;
  double** combined;
  if (isMaster(myRank)) {
    combined = allocateMatrix(N, N);
    for (i = 0; i < myNumRows; i++) {
      arrayCopy(buffer[k][i], combined[i], N);
    }
    for (j = 1; j < commSize; j++) {
      for (i = 0; i < myNumRows; i++) {
        Recv(combined[j * myNumRows + i], N, j, fStep);
      }
    }
    debugCall(printLnMatrix(combined, N, N));
    debugCall(printf("\n"));
    outputCall(
      if (writeHeader(fStep) == -1) {
        Abort(1);
      }
    );
    outputCall(
      if (writeMatrix(combined, N, N, fStep) == -1) {
        Abort(1);
      }
    );
    for (i = 0; i < N; i++) {
      free(combined[i]);
    }
    free(combined);
    statusCall(printStatus(fStep, fEnd));
  } else {
    for (j = 0; j < myNumRows; j++) {
      Send(buffer[k][j], N, MASTER, fStep);
    }
  }
}

void createAnimation() {
  if (isMaster(myRank)) {
    printf("Creating animation gif. This may take a while...\n");
    Barrier();
    createAnimationGif();
    if (commSize > 1) {
      taskComplete(1);
    }
  } else {
    Barrier();
    if (myRank == 1) {
      waitWithAnimation(MASTER);
    }
  }
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
	double max = (1.0/(2.0*M_PI*sigma*sigma))*exp(-0.5*( ((0.5-mu)/sigma)
      *((0.5-mu)/sigma) +  ((0.5-mu)/sigma)*((0.5-mu)/sigma)   ));
	double result = (1.0/(2.0*M_PI*sigma*sigma))*exp(-0.5*( ((x-mu)/sigma)
      *((x-mu)/sigma) +  ((y-mu)/sigma)*((y-mu)/sigma)   ))/max;
	return result;
}

double getInitialInput(int i) {
  return i == 0 ? 0 : i == N - 1 ? 1 : (double) i / (N - 1);
}

double*** allocateBufferMemory(int numRows, int numCols) {
  double*** result = malloc(3 * sizeof(double**));
  int k;
  for (k = 0; k < 3; k++) {
    result[k] = allocateMatrix(numRows, numCols);
  }
  return result;
}

double** allocateMatrix(int numRows, int numCols) {
  double** matrix = malloc(numRows * sizeof(double*));
  int i;
  for (i = 0; i < numRows; i++) {
    matrix[i] = calloc(numCols, sizeof(double));
  }
  return matrix;
}
