#include <pebble.h>

Layer* connection_create_connected_layer();
void connection_destroy_connected_layer();

void connection_update(bool connected);