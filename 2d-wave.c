/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 23, 2017
*************************************************************/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include "settings.h"
#include "mpihelp.h"
#include "ctools.h"

double initialCondition(double x, double y);

void main() {

  /*************************************************************
  * Setup the environment
  */

  int commSize;
  int myRank;

  Init(NULL, NULL);
  Size(&commSize);
  Rank(&myRank);

  if(commSize != 1 && commSize % 2 != 0) {
   printf("comm size must be divisible by 2!\n");
   Abort(1);
  }

  if (n % commSize != 0) {
    printf("n must be divisible by comm size!\n");
    Abort(1);
  }
  Barrier();

  /*************************************************************
  * Begin the real task
  */

  int myColLength;
  int myRowLength = myColLength = n / commSize;



  /*************************************************************
  * Cleanup
  */
  Finalize();
}

double initialCondition(double x, double y) {
	double max = (1.0/(2.0*M_PI*sigma*sigma))*exp(-0.5*( ((0.5-mu)/sigma)*((0.5-mu)/sigma) +  ((0.5-mu)/sigma)*((0.5-mu)/sigma)   ));
	double result = (1.0/(2.0*M_PI*sigma*sigma))*exp(-0.5*( ((x-mu)/sigma)*((x-mu)/sigma) +  ((y-mu)/sigma)*((y-mu)/sigma)   ))/max;
	return result;
}
