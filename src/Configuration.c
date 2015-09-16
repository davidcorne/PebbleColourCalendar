#include "Configuration.h"
#include "Debug.h"

// Keys
#define WEEKS_SHOWN_KEY 10

static Configuration* s_configuration;

void configuration_create()
{
    // Create the memory
    s_configuration = malloc(sizeof(Configuration));
    
    // Initialise from persistance
    if (persist_exists(WEEKS_SHOWN_KEY)) {
        s_configuration->weeks_shown = persist_read_int(WEEKS_SHOWN_KEY);
    } else {
        // Default value
        s_configuration->weeks_shown = WEEKS_SHOWN_NEXT_TWO;
    }

    DEBUG_LOG(
        "Configuration, weeks_shown: %d",
        s_configuration->weeks_shown
    );
}

void configuration_destroy()
{
    // Save the configuration to the persistance
    int status = persist_write_int(WEEKS_SHOWN_KEY, s_configuration->weeks_shown);
    DEBUG_LOG(
        "WEEKS_SHOWN_KEY written: %d",
        status
    );
    // Free the memory
    free(s_configuration);
}

Configuration* configuration_get()
{
    // Should really assert or if block here.
    return s_configuration;
}