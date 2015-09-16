#include <pebble.h>
#include "Colour.h"
#include "Calendar.h"
#include "DateTime.h"
#include "Debug.h"
#include "Battery.h"
#include "Connection.h"

static Window* s_main_window;

static void main_window_load(Window *window) 
{
  DEBUG_LOG("load main window");
  TextLayer* time_layer = datetime_create_time_layer();
  // Add it as a child layer to the Window's root layer
  layer_add_child(
    window_get_root_layer(window),
    text_layer_get_layer(time_layer)
  );

  TextLayer* date_layer = datetime_create_date_layer();
  layer_add_child(
    window_get_root_layer(window),
    text_layer_get_layer(date_layer)
  );
  // draw them
  datetime_update();
  
  // Add the connected icon
  layer_add_child(
    window_get_root_layer(window),
    connection_create_connected_layer()
  );
  
  // Add charging image to the window
  BitmapLayer* charging_image_layer = battery_create_charging_layer();
  layer_add_child(
    window_get_root_layer(window),
    bitmap_layer_get_layer(charging_image_layer)
  );
  
  // Add battery meter to window
  Layer* meter_layer = battery_create_meter_layer();
  layer_add_child(window_get_root_layer(window), meter_layer);

  // Add battery label layer to window
   // Add to Window
  TextLayer* label_layer = battery_create_label_layer();
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(label_layer));

  layer_add_child(window_get_root_layer(window), calendar_create_layer());
  
  // // Update the battery
  // BatteryChargeState battery_state = battery_state_service_peek();
  // battery_update(battery_state.charge_percent, battery_state.is_charging);
  
  // // Update the connection status
  // connection_update(bluetooth_connection_service_peek());
}

static void tick_handler(struct tm* tick_time, TimeUnits units_changed) 
{
  datetime_update();
}

static void battery_callback(BatteryChargeState state) 
{
  DEBUG_LOG("Battery_callback");
  battery_update(state.charge_percent, state.is_charging);
}

static void connection_callback(bool connected)
{
  DEBUG_LOG("connection_callback");
  connection_update(connected);
}

static void main_window_unload(Window *window) 
{
  datetime_destroy_time_layer();
  datetime_destroy_date_layer();
  connection_destroy_connected_layer();
  battery_destroy_meter_layer();
  battery_destroy_label_layer();
  battery_destroy_charging_layer();
  calendar_destroy_layer();
}

static void init()
{
  INFO_LOG(
    "\tDebug level:      %d",
    s_debug
    );
  INFO_LOG(
    "\tVersion label:    %s",
    VERSION_LABEL
  );
  INFO_LOG(
    "\tCompany name:     %s",
    COMPANY_NAME
  );
  INFO_LOG(
    "\tApplication name: %s",
    APPLICATION_NAME
  );
  INFO_LOG(
    "\tUUID:             %s",
    APPLICATION_UUID
  );
  DEBUG_LOG("init");
  // Create the main window instance
  s_main_window = window_create();
  
  window_set_window_handlers(
    s_main_window, 
    (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload
    }
  );
  
  // Push the window on the stack
  bool animated = true;
  window_stack_push(s_main_window, animated);
  
  // Listen for time changes
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Listen for battery changes
  battery_state_service_subscribe(battery_callback);
  
  // Ensure battery level is displayed from the start
  battery_callback(battery_state_service_peek());
  
  // Listen for connection changes
  bluetooth_connection_service_subscribe(connection_callback);
  
  // Ensure it's correct
  connection_callback(bluetooth_connection_service_peek());
}

static void deinit()
{
  // Destroy the main window
  window_destroy(s_main_window);
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}