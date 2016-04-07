#include "Calendar.h"
#include "Debug.h"
#include "Layout.h"
#include "miniprintf.h"

static Layer* s_calendar_layer;
static char s_calendar_days[21][3];
static char s_week_days[7][6] = {
  "Mo",
  "Tu",
  "We",
  "Th",
  "Fr",
  "Sa",
  "Su"
};

  
static int16_t days_in_month(int16_t month, int16_t year)
{
  DEBUG_LOG(
    "Asked for days in year %d month %d.",
    year,
    month
  );
  // Index | Month | Days
  //-------+-------+------
  //   0   | Jan   | 31
  //   1   | Feb   | 28/29
  //   2   | Mar   | 31
  //   3   | Apr   | 30
  //   4   | May   | 31
  //   5   | Jun   | 30
  //   6   | Jul   | 31
  //   7   | Aug   | 31
  //   8   | Sep   | 30
  //   9   | Oct   | 31
  //  10   | Nov   | 30
  //  11   | Dec   | 31
  int16_t days = 31;
  if (
    month == 3 || // April
    month == 5 || // May
    month == 8 || // September
    month == 10   // November
  ) {
    days = 30;
  } else if (month == 1) {
    // It's February, oh joy!
    bool leap_year = year % 4 == 0;
    if (leap_year) {
      // It's not a leap year if the year is divisible by 100
      if (leap_year % 100 == 0) {
        leap_year = false;
        if (leap_year % 400 == 0) {
          // unless it's divisible by 400.
          leap_year = true;
        }
      }
    }
    days = leap_year ? 29 : 28;
  }
  return days;
}

static void calendar_update(Layer* layer, GContext* context) 
{
  GRect bounds = layer_get_bounds(layer);
  
  graphics_context_set_fill_color(context, COLOR_FALLBACK(GColorGreen, GColorBlack));
  graphics_context_set_stroke_color(context, COLOR_FALLBACK(GColorIndigo, GColorBlack));
  graphics_context_set_text_color(context, COLOR_FALLBACK(GColorDarkCandyAppleRed, GColorBlack));

  // uint16_t corner_radius = 0;
  // graphics_fill_rect(
  //   context,
  //   bounds,
  //   corner_radius,
  //   GCornerNone
  // );

  // Distances common to all of the procedures
  int16_t x_diff = 20;
  int16_t y_diff = 20;
  
  // Draw the horizontal lines
  int16_t left_x = bounds.origin.x;
  int16_t right_x = bounds.origin.x + bounds.size.w;
  int16_t start_y = bounds.origin.y + y_diff + 2;
  for (int i = 0; i < 3; ++i) {
    int16_t y = start_y + (i * y_diff);
    graphics_draw_line(
      context,
      GPoint(left_x, y),
      GPoint(right_x, y)
    );
  }
  
  // Draw the vertical lines
  int16_t top_y = start_y;
  int16_t bottom_y = bounds.origin.y + bounds.size.h;
  // Start from 1 as we don't want to draw the first line
  for (int i = 1; i < 7; ++i) {
    int16_t x = bounds.origin.x + (i * x_diff);
    graphics_draw_line(
      context,
      GPoint(x, top_y),
      GPoint(x, bottom_y)
    );
  }
  
  // Draw the days of the week
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  GTextAttributes* layout_cache = NULL;
  for (int16_t i = 0; i < 7; ++i) {
    int16_t x = bounds.origin.x + (i * x_diff);
    graphics_draw_text(
      context,
      s_week_days[i],
      font,
      GRect(
        x,
        bounds.origin.y,
        x_diff,
        bounds.origin.y + 20
      ),
      GTextOverflowModeFill,
      GTextAlignmentCenter,
      layout_cache
    );
  }
  
  // Now draw the calendar days.
  // First, fill s_calendar_days
  time_t temp = time(NULL);
  struct tm* now = localtime(&temp);
  
  // year is from 1900, so make up for it here.
  int16_t year = now->tm_year + 1900;
  
  int16_t day_start = now->tm_mday - (now->tm_wday - 1);
  if (now->tm_wday == 0) {
    // It's a Sunday, so go back a week.
    day_start -= 7;
  }
  DEBUG_LOG("Week starts on day: %d", day_start);
  int16_t days_in_this_month = days_in_month(
    now->tm_mon,
    year
  );
  int16_t days_last_month = days_in_month(
    (now->tm_mon - 1) % 12,
    now->tm_mon != 0 ? year : year - 1
  );
  int16_t days_next_month = days_in_month(
    (now->tm_mon + 1) % 12,
    now->tm_mon != 11 ? year : year + 1
  );
  font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  GFont bold_font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
  graphics_context_set_text_color(context, COLOR_FALLBACK(GColorDarkGreen, GColorBlack));
  for (int16_t i = 0; i < 21; ++i) {
    int16_t current_day = day_start;
    if (current_day < 1) {
      // This was last month
      current_day += days_last_month;
    } else if (current_day > days_in_this_month) {
      // This is next month
      current_day -= days_in_this_month;
    }
    mini_snprintf(
      s_calendar_days[i],
      3,
      "%02d",
      current_day
    );
    day_start += 1;
    
    // Now do the actual drawing
    int16_t x = bounds.origin.x + ((i % 7) * x_diff);
    int16_t y = start_y + ((i / 7) * y_diff);
    GFont current_font = current_day == now->tm_mday ? bold_font : font;
    graphics_draw_text(
      context,
      s_calendar_days[i],
      current_font,
      GRect(
        x,
        y,
        x_diff,
        y + 20
      ),
      GTextOverflowModeFill,
      GTextAlignmentCenter,
      layout_cache
    );
  }
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