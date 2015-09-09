#include "Calendar.h"
#include "Layout.h"

static Layer* s_calendar_layer;

static void calendar_update(Layer* layer, GContext* context) 
{
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(context, COLOR_FALLBACK(GColorGreen, GColorBlack));
  uint16_t corner_radius = 0;
  graphics_fill_rect(
    context,
    bounds,
    corner_radius,
    GCornerNone
  );
}

// Create and return the calendar layer
Layer* calendar_create_layer()
{
    s_calendar_layer = layer_create(
      GRect(
        CALENDAR_LEFT,
        CALENDAR_TOP,
        CALENDAR_WIDTH,
        CALENDAR_HEIGHT
      )
    );
    
    layer_set_update_proc(s_calendar_layer, calendar_update);
    
    return s_calendar_layer;
}

void calendar_destroy_layer()
{
  layer_destroy(s_calendar_layer);
}