/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Mar 1, 2017
*************************************************************/

#include <mpi.h>

#ifndef bool_t_DEFINED
  #define bool_t_DEFINED
  typedef enum { false, true } bool_t;
#endif

#ifndef optimizationLevel
  #define optimizationLevel "O?"
#endif

#define Init(x, y) MPI_Init(x, y)
#define Abort(x) MPI_Abort(MPI_COMM_WORLD, x)
#define Rank(x) MPI_Comm_rank(MPI_COMM_WORLD, x)
#define Size(x) MPI_Comm_size(MPI_COMM_WORLD, x)
#define Barrier() MPI_Barrier(MPI_COMM_WORLD)
#define Finalize() MPI_Finalize()
#define Time() MPI_Wtime()

static int const MASTER = 0; // root proc rank

#ifdef debug
  #define debugPrintRoot(rank,format,args...) do { if (rank == MASTER) { printf(format, ## args); } } while(0)
#else
  #define debugPrintRoot(rank,format,args...)
#endif

#ifdef timing
  #define timingCall(x) do { x; } while(0)
#else
  #define timingCall(x)
#endif

bool_t isMaster(int rank);

bool_t isMaster(int rank) {
  return rank == MASTER;
}

char _notifyProxy;
void taskComplete(int notifyRank) {
  MPI_Send(&_notifyProxy, 1, MPI_CHAR, notifyRank, 0, MPI_COMM_WORLD);
}

void waitWithAnimation(int waitingForRank) {
  double lastTime = Time();
  char frame = 0;
  int waiting = 0;
  while (waiting == 0) {
    MPI_Iprobe(waitingForRank, 0, MPI_COMM_WORLD, &waiting, MPI_STATUS_IGNORE);
    if (waiting != 0) {
      MPI_Recv(&_notifyProxy, 1, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      double currentTime = Time();
      if (currentTime - lastTime > 1) {
        lastTime = currentTime;
        switch (frame) {
          case 0:
            printf("\r\\");
            break;
          case 1:
            printf("\r|");
            break;
          case 2:
            printf("\r/");
            break;
          case 3:
            printf("\r-");
            break;
        }
        frame++;
        if (frame > 3) {
          frame = 0;
        }
      }
    }
  }
}

int writeTimingFile(char* fileName, int n, int fEndStep, int commSize, double elapsedTime) {
  FILE *fp;
	fp = fopen(fileName, "a");
	if (fp == NULL) {
		printf("sorry can't open %s.\n", fileName);
		return -1;
	}
  fprintf(fp, "2d-wave\t%s\t%d\t%d\t%d\t%g\n", optimizationLevel, n, fEndStep, commSize, elapsedTime);
  fflush(fp);
  fclose(fp);
  return 0;
}
