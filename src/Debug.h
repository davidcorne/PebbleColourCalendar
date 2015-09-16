// Uncomment this line to enable debugging.
// #define COLOUR_CAL_DEBUG 1
#include "generated/appinfo.h"

#ifdef COLOUR_CAL_DEBUG
  #pragma message "--- DEBUGGING ENABLED ---"
  static bool s_debug = true;
#else
  #pragma message "--- WARNING: DEBUGGING DISABLED ---"
  static bool s_debug = false;
#endif

#define DEBUG_LOG(...) \
  if (s_debug) { \
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__); \
  }

#define INFO_LOG(...) \
   app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__); 
