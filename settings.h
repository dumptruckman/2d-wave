/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 23, 2017
*************************************************************/

#ifndef bool_t_DEFINED
  #define bool_t_DEFINED
  typedef enum { false, true } bool_t;
#endif

#ifndef N
  #define N 20
#endif

#ifndef timeDeltaMultiplier
  #define timeDeltaMultiplier 1
#endif

#ifndef fEnd
  #define fEnd 60
#endif

#ifndef sigma
  //#define sigma 0.01 //tight point
  //#define sigma 0.1 //wider point
  #define sigma 0.05 // in between!
#endif

#ifndef mu
  #define mu 0.5
#endif

// Uncomment following line to run in debug mode
//#define debug
#ifdef debug
  #ifndef gather
    #define gather(x) do { x; } while(0)
  #else
    #undef gather
    #define gather(x) do { x; } while(0)
  #endif
#else
  #ifndef gather
    #define gather(x)
  #endif
#endif

// Uncomment following line to write output to pgm files
//#define output
#ifdef output
  #ifndef gather
    #define gather(x) do { x; } while(0)
  #else
    #undef gather
    #define gather(x) do { x; } while(0)
  #endif
  #define outputCall(x) do { x; } while(0)
#else
  #ifndef gather
    #define gather(x)
  #endif
  #define outputCall(x)
#endif
