#include "DateTime.h"
#include "Layout.h"
#include "Colour.h"

static TextLayer* s_time_layer;
static TextLayer* s_date_layer;

static char s_time_buffer[9];
// [abbreviated-day] [01-31] [abbreviated-month]
// 3 + 1 + 2 + 1 + 3 + 1
static char s_date_buffer[11];
static char s_12_hour_format[] = "%l:%M %p";
static char s_24_hour_format[] = "%H:%M";

TextLayer* datetime_create_time_layer()
{
  s_time_layer = text_layer_create(
    GRect(
      TIME_DISPLAY_LEFT,
      TIME_DISPLAY_TOP,
      TIME_DISPLAY_WIDTH,
      TIME_DISPLAY_HEIGHT
    )
  );
  
  // Set the initial attributes on the text.
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, colour_time_colour());

  // Improve the layout to be more like a watchface
  text_layer_set_font(
    s_time_layer, 
    fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK)
  );
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  return s_time_layer;    
}

void datetime_destroy_time_layer()
{
    text_layer_destroy(s_time_layer);
}

TextLayer* datetime_create_date_layer()
{
    s_date_layer = text_layer_create(
    GRect(
      DATE_DISPLAY_LEFT,
      DATE_DISPLAY_TOP,
      DATE_DISPLAY_WIDTH,
      DATE_DISPLAY_HEIGHT
    )
  );
  
  // Set the initial attributes on the text.
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, colour_time_colour());

  // Improve the layout to be more like a watchface
  text_layer_set_font(
    s_date_layer, 
    fonts_get_system_font(FONT_KEY_GOTHIC_24)
  );
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
  
  return s_date_layer;
}

void datetime_destroy_date_layer()
{
    text_layer_destroy(s_date_layer);
}

void datetime_update()
{
  time_t temp = time(NULL);
  struct tm* now = localtime(&temp);
  
  strftime(
    s_time_buffer, 
    sizeof(s_time_buffer), 
    clock_is_24h_style() ? s_24_hour_format : s_12_hour_format,
    now
  );
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %d %b", now);
  
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
}
