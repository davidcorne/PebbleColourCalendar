#include "Connection.h"
#include "Layout.h"

BitmapLayer* s_connected_layer;

GBitmap* s_bluetooth_bitmap;
GBitmap* s_airplane_bitmap;

bool s_connected = false;

Layer* connection_create_connected_layer()
{
    s_bluetooth_bitmap = 
      gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_IMAGE);
    s_airplane_bitmap = 
      gbitmap_create_with_resource(RESOURCE_ID_AIRPLANE_IMAGE);
    s_connected_layer = bitmap_layer_create(
      GRect(
          CONNECTION_LEFT,
          CONNECTION_TOP,
          CONNECTION_WIDTH,
          CONNECTION_HEIGHT
      )  
    );
    return bitmap_layer_get_layer(s_connected_layer);
}

void connection_destroy_connected_layer()
{
    bitmap_layer_destroy(s_connected_layer);
    gbitmap_destroy(s_bluetooth_bitmap);
    gbitmap_destroy(s_airplane_bitmap);
}

void connection_update(bool connected)
{
    if (connected) {
      bitmap_layer_set_bitmap(s_connected_layer, s_bluetooth_bitmap);    
    } else {
      bitmap_layer_set_bitmap(s_connected_layer, s_airplane_bitmap);
    }
}