/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 22, 2017
*************************************************************/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include "settings.h"
#include "mpihelp.h"
#include "ctools.h"

void main(int argsc, char* argsv[]) {

  /*************************************************************
  * Setup the environment
  */

  int commSize;
  int myRank;

  Init(NULL, NULL);
  Size(&commSize);
  Rank(&myRank);

  if(commSize % 2 != 0) {
   printf("comm size must be divisible by 2!\n");
   Abort(1);
  }

  settings_t actualSettings;
  settings_t* settings = &actualSettings;
  processArgs(argsc, argsv, settings);
  if (settings->debug && isMaster(myRank)) {
    printSettings(settings);
  }

  if (settings->xyCount % commSize != 0) {
    printf("n must be divisible by comm size!\n");
    Abort(1);
  }
  Barrier();

  /*************************************************************
  * Begin the real task
  */

  int myColLength;
  int myRowLength = myColLength = settings->xyCount / commSize;



  /*************************************************************
  * Cleanup
  */
  Finalize();
}
