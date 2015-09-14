#include "Battery.h"

#include "Colour.h"
#include "Debug.h"
#include "miniprintf.h"
#include "Layout.h"

static TextLayer* s_battery_label_layer;
static Layer* s_battery_meter_layer;
static BitmapLayer* s_charging_image_layer;

// Displayable state
static char s_battery_percent_buffer[] = "%d%%";
static int s_battery_level = 10;
static bool s_charging = false;
static GBitmap* s_charging_bitmap;

static void update_battery_label() 
{
  mini_snprintf(
    s_battery_percent_buffer, 
    sizeof(s_battery_percent_buffer), 
    "%d%%", 
    s_battery_level
  );
  DEBUG_LOG(
    "Battery label drawn with buffer %s",
    s_battery_percent_buffer
  );
  text_layer_set_text(s_battery_label_layer, s_battery_percent_buffer);
}

static void battery_meter_update(Layer* layer, GContext* context) 
{
  DEBUG_LOG(
    "Battery drawn with level %d",
    s_battery_level
  );
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(context, COLOR_FALLBACK(GColorGreen, GColorBlack));
  graphics_context_set_stroke_color(context, COLOR_FALLBACK(GColorBlack, GColorBlack));
  
  // Draw the battery meter.
  if (50 <= s_battery_level) {
    graphics_context_set_fill_color(context, COLOR_FALLBACK(GColorGreen, GColorBlack));
  } else if (30 <= s_battery_level && s_battery_level <= 50) {
    graphics_context_set_fill_color(context, COLOR_FALLBACK(GColorChromeYellow, GColorBlack));
  } else {
    graphics_context_set_fill_color(context, COLOR_FALLBACK(GColorRed, GColorBlack));
  }
  
  int full_battery_width = STATUS_BATTERY_WIDTH - STATUS_BATTERY_NUB_WIDTH;
  // -1 otherwise at 100% it will overwrite the nub.
  int rect_width = (full_battery_width / 10) * (s_battery_level / 10) - 1;
  // full_battery_width should be divisible by 10
  GRect battery_rect = GRect(
    bounds.origin.x + 1,
    bounds.origin.y + 1,
    rect_width,
    STATUS_BATTERY_HEIGHT - 2
  );
  uint16_t corner_radius = 0;
  graphics_fill_rect(
    context,
    battery_rect,
    corner_radius,
    GCornerNone
  );
  
  // Draw the battery box.
  graphics_draw_rect(
    context,
    GRect(
      bounds.origin.x,
      bounds.origin.y,
      STATUS_BATTERY_WIDTH - STATUS_BATTERY_NUB_WIDTH,
      STATUS_BATTERY_HEIGHT
    )
  );
  
  // Draw the battery nub.
  graphics_draw_rect(
    context,
    GRect(
      bounds.origin.x + STATUS_BATTERY_WIDTH - STATUS_BATTERY_NUB_WIDTH - 1, // Take off 1 so we are over the other line
      bounds.origin.y + ((STATUS_BATTERY_HEIGHT - STATUS_BATTERY_NUB_HEIGHT) / 2),
      STATUS_BATTERY_NUB_WIDTH,
      STATUS_BATTERY_NUB_HEIGHT
    )
  );
}

TextLayer* battery_create_label_layer()
{
  s_battery_label_layer = text_layer_create(
    GRect(
       STATUS_BATTERY_LABEL_LEFT,
       STATUS_BATTERY_LABEL_TOP,
       STATUS_BATTERY_LABEL_WIDTH,
       STATUS_BATTERY_LABEL_HEIGHT
    )
  );
  
  // Set the initial attributes on the text.
  text_layer_set_background_color(s_battery_label_layer, GColorClear);
  text_layer_set_text_color(s_battery_label_layer, colour_text_colour());
  text_layer_set_text(s_battery_label_layer, s_battery_percent_buffer);

  text_layer_set_font(
    s_battery_label_layer, 
    fonts_get_system_font(FONT_KEY_GOTHIC_14)
  );
  text_layer_set_text_alignment(s_battery_label_layer, GTextAlignmentRight);
  
  return s_battery_label_layer;
}

void battery_destroy_label_layer()
{
  text_layer_destroy(s_battery_label_layer);
}

Layer* battery_create_meter_layer()
{
  // Create battery meter Layer
  s_battery_meter_layer = layer_create(
    GRect(
       STATUS_BATTERY_LEFT,
       STATUS_BATTERY_TOP,
       STATUS_BATTERY_WIDTH,
       STATUS_BATTERY_HEIGHT
    )
  );
  layer_set_update_proc(s_battery_meter_layer, battery_meter_update);

  return s_battery_meter_layer;    
}

void battery_destroy_meter_layer()
{
  layer_destroy(s_battery_meter_layer);
}

BitmapLayer* battery_create_charging_layer()
{
  // Load the bitmap resource first.
  s_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CHARGING_IMAGE);
  s_charging_image_layer = bitmap_layer_create(
    GRect(
      BATTERY_CHARGING_LEFT,
      BATTERY_CHARGING_TOP,
      BATTERY_CHARGING_WIDTH,
      BATTERY_CHARGING_HEIGHT
    )
  );
  bitmap_layer_set_bitmap(s_charging_image_layer, s_charging_bitmap);
  return s_charging_image_layer;
}

void battery_destroy_charging_layer()
{
  bitmap_layer_destroy(s_charging_image_layer);
  gbitmap_destroy(s_charging_bitmap);
}

void charging_update()
{
  Layer* to_hide = 0;
  Layer* to_show = 0;
  if (s_charging) {
    // The watch is charging
    to_show = bitmap_layer_get_layer(s_charging_image_layer);
    to_hide = text_layer_get_layer(s_battery_label_layer);
  } else {
    to_show = text_layer_get_layer(s_battery_label_layer);
    to_hide = bitmap_layer_get_layer(s_charging_image_layer);
  }
  layer_set_hidden(to_hide, true);
  layer_set_hidden(to_show, false);
}

void battery_update(int battery_level, bool charging)
{
  // Record the charging state
  s_charging = charging;
  charging_update();
  
  // Record the new battery level
  s_battery_level = battery_level;
  layer_mark_dirty(s_battery_meter_layer);
  
  // Don't update the level if it's charging
  if (!s_charging) {
    update_battery_label();
  }
}
