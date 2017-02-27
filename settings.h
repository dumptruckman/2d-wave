/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 23, 2017
*************************************************************/

#ifndef bool_t_DEFINED
  #define bool_t_DEFINED
  typedef enum { false, true } bool_t;
#endif

// Number of permutations
#ifndef N
  #define N 100
#endif

// Values greater than 1 speed up the animation
#ifndef timeDeltaMultiplier
  #define timeDeltaMultiplier 10
#endif

// Number of time steps
#ifndef fEnd
  #define fEnd 100
#endif

#ifndef sigma
  #define sigma 0.01 //tight point
  //#define sigma 0.1 //wider point
  //#define sigma 0.05 // in between!
#endif

#ifndef mu
  #define mu 0.5
#endif

// Uncomment following line to run in debug mode
//#define debug
#ifdef debug
  #ifndef gatherEnabled
    #define gatherEnabled true
  #endif
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
  #ifndef gatherEnabled
    #define gatherEnabled true
  #endif
  #ifndef gather
    #define gather(x) do { x; } while(0)
  #else
    #undef gather
    #define gather(x) do { x; } while(0)
  #endif
  #define outputCall(x) do { x; } while(0)
  #ifndef imageSize
    #define imageSize "400x400"
  #endif
#else
  #ifndef gather
    #define gather(x)
  #endif
  #define outputCall(x)
#endif

#ifdef showStatus
  #ifndef gatherEnabled
    #define gatherEnabled false
  #endif
  #define statusCall(x) do { x; } while(0)
#else
  #ifndef gatherEnabled
    #define gatherEnabled false
  #endif
  #define statusCall(x)
#endif
