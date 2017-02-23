/*************************************************************
* Author: Jeremy Wood
* Date: Feb 22, 2017
* Last Modified: Feb 22, 2017
*************************************************************/

#include <string.h>

#ifndef bool_t_DEFINED
  #define bool_t_DEFINED
  typedef enum { false, true } bool_t;
#endif

#ifndef xyCount
  #define xyCount 10
#endif

#ifndef timeDeltaMultiplier
  #define timeDeltaMultiplier 1
#endif

#ifndef timeMax
  #define timeMax 2
#endif

#ifndef debug
  #define debug false
#endif

static int const DEFAULT_XY_COUNT = xyCount;
static int const DEFAULT_TIME_DELTA_MULTI = timeDeltaMultiplier;
static int const DEFAULT_TIME_MAX = timeMax;
static int const DEFAULT_DEBUG = debug;

#undef xyCount
#undef timeDeltaMultiplier
#undef timeMax
#undef debug

typedef struct Settings {
  int xyCount; // The number of x and y elements
  int timeDeltaMultiplier; // modifies the time delta
  int timeMax; // "f_end"
  bool_t debug;
} settings_t;

typedef enum _SettingFlag {
  NONE,
  XY_COUNT,
  TIME_DELTA_MULTI,
  TIME_MAX
} _SettingFlag;

void processArgs(int argsc, char* argsv[], settings_t* settings);
void printSettings(settings_t* settings);

void processArgs(int argsc, char* argsv[], settings_t* settings) {
  // Initialize with defaults
  settings->xyCount = DEFAULT_XY_COUNT;
  settings->timeDeltaMultiplier = DEFAULT_TIME_DELTA_MULTI;
  settings->timeMax = DEFAULT_TIME_MAX;
  settings->debug = DEFAULT_DEBUG;

  // Determine if any settings supplied in command line args
  _SettingFlag flag = NONE;
  int i;
  for (i = 1; i < argsc; i++) {
    switch (flag) {
      case XY_COUNT:
        settings->xyCount = atoi(argsv[i]);
        flag = NONE;
        break;
      case TIME_DELTA_MULTI:
        settings->timeDeltaMultiplier = atoi(argsv[i]);
        flag = NONE;
        break;
      case TIME_MAX:
        settings->timeMax = atoi(argsv[i]);
        flag = NONE;
        break;
      case NONE:
        if (strcmp(argsv[i], "-n") == 0) {
          flag = XY_COUNT;
        } else if (strcmp(argsv[i], "-t") == 0) {
          flag = TIME_DELTA_MULTI;
        } else if (strcmp(argsv[i], "-e") == 0 || strcmp(argsv[i], "-end") == 0) {
          flag = TIME_MAX;
        } else if (strcmp(argsv[i], "--debug") == 0) {
          settings->debug = true;
        }
        break;
    }
  }
}

void printSettings(settings_t* settings) {
  printf("XY Count              = %d\n", settings->xyCount);
  printf("Max Time              = %d\n", settings->timeMax);
  printf("Time Delta Multiplier = %d\n", settings->timeDeltaMultiplier);
}
