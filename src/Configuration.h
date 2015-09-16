#include <pebble.h>

typedef enum {
  WEEKS_SHOWN_NEXT_TWO,
  WEEKS_SHOWN_NEXT_ONE,
  WEEKS_SHOWN_NEXT_NONE
} ConfigurationWeeksShown;

typedef struct {
    ConfigurationWeeksShown weeks_shown;
} Configuration;

void configuration_create();
void configuration_destroy();

Configuration* configuration_get();
