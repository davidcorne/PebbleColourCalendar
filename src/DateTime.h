#include <pebble.h>

//----- Time layer
TextLayer* datetime_create_time_layer();
void datetime_destroy_time_layer();

//----- Date layer
TextLayer* datetime_create_date_layer();
void datetime_destroy_date_layer();

void datetime_update();
