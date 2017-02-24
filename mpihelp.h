/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 23, 2017
*************************************************************/

#include <mpi.h>

#ifndef bool_t_DEFINED
  #define bool_t_DEFINED
  typedef enum { false, true } bool_t;
#endif

#define Init(x, y) MPI_Init(x, y)
#define Abort(x) MPI_Abort(MPI_COMM_WORLD, x)
#define Rank(x) MPI_Comm_rank(MPI_COMM_WORLD, x)
#define Size(x) MPI_Comm_size(MPI_COMM_WORLD, x)
#define Barrier() MPI_Barrier(MPI_COMM_WORLD)
#define Finalize() MPI_Finalize()

static int const MASTER = 0; // root proc rank

#ifdef debug
  #define debugPrintRoot(rank,format,args...) do { if (rank == MASTER) { printf(format, ## args); } } while(0)
#else
  #define debugPrintRoot(rank,format,args...)
#endif

bool_t isMaster(int rank);

bool_t isMaster(int rank) {
  return rank == MASTER;
}
