#include <pebble.h>

TextLayer* battery_create_label_layer();
void battery_destroy_label_layer();

Layer* battery_create_meter_layer();
void battery_destroy_meter_layer();

void battery_update(int battery_level);