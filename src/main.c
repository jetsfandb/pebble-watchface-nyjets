#include <pebble.h>
#include "main.h"
#include "schedule.h"

static Window  *s_window_main;
static GBitmap *s_image_nyjets_logo;

static TextLayer   *s_layer_date; 
static BitmapLayer *s_layer_logo;
static TextLayer   *s_layer_time;
static TextLayer   *s_layer_gametime;

static struct schedule s_season_schedule[16];
  
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(s_window_main);  
  GRect bounds_main_window = layer_get_bounds(window_layer);
  s_image_nyjets_logo = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NYJETS_LOGO);
  
  GRect bounds_logo = bounds_main_window;
  s_layer_logo = bitmap_layer_create(bounds_logo);
  bitmap_layer_set_bitmap(s_layer_logo, s_image_nyjets_logo);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_layer_logo));

  GRect bounds_time = bounds_main_window;
  bounds_time.size.h = 30;
  s_layer_time = text_layer_create(bounds_time);
  text_layer_set_text_alignment(s_layer_time, GTextAlignmentCenter);
  text_layer_set_text_color(s_layer_time, GColorWhite);
  text_layer_set_font(s_layer_time, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_background_color(s_layer_time, GColorFromRGBA(0, 0, 0, 0));
  layer_add_child(bitmap_layer_get_layer(s_layer_logo), text_layer_get_layer(s_layer_time));

  GRect bounds_gametime = bounds_main_window;
  bounds_gametime.size.h = 103;
  bounds_gametime.origin.y = bounds_time.size.h + 4;
  s_layer_gametime = text_layer_create(bounds_gametime);
  text_layer_set_text_alignment(s_layer_gametime, GTextAlignmentCenter);
  text_layer_set_text_color(s_layer_gametime, GColorWhite);
  text_layer_set_font(s_layer_gametime, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));  
  text_layer_set_background_color(s_layer_gametime, GColorFromRGBA(0, 0, 0, 0));
  layer_add_child(bitmap_layer_get_layer(s_layer_logo), text_layer_get_layer(s_layer_gametime));

  GRect bounds_date = bounds_main_window;
  bounds_date.size.h = 30;
  bounds_date.origin.y = (bounds_main_window.size.h - bounds_date.size.h - 1);
  
  s_layer_date = text_layer_create(bounds_date);
  text_layer_set_text_alignment(s_layer_date, GTextAlignmentCenter);
  text_layer_set_text_color(s_layer_date, GColorWhite);
  text_layer_set_font(s_layer_date, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_background_color(s_layer_date, GColorFromRGBA(0, 0, 0, 0));
  layer_add_child(bitmap_layer_get_layer(s_layer_logo), text_layer_get_layer(s_layer_date));

}
static void main_window_unload(Window *window) {
  
    gbitmap_destroy(s_image_nyjets_logo);
    text_layer_destroy(s_layer_date);
    bitmap_layer_destroy(s_layer_logo);
    text_layer_destroy(s_layer_time);
    text_layer_destroy(s_layer_gametime);
}


static void update_time() {
    // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Day of Week
  static char s_dow[11];
  strftime(s_dow, sizeof(s_dow), "%A", tick_time);
  static char s_dow_short[4];
  strncpy(s_dow_short, s_dow, 3);
  
  // Month name
  static char s_month_name[10];
  strftime(s_month_name, sizeof(s_month_name), "%B", tick_time);
  static char s_month_name_short[4];
  strncpy(s_month_name_short, s_month_name, 3);  
  
  // Time
  static char s_time[9];
  strftime(s_time, sizeof(s_time), "%l:%M %p", tick_time);
  text_layer_set_background_color(s_layer_time, GColorFromRGBA(0, 0, 0, 0));
  text_layer_set_text(s_layer_time, s_time);

  // Date
  static char s_date[3];
  strftime(s_date, sizeof(s_date), "%e", tick_time);    
  static char s_date_str[13];
  snprintf(s_date_str, sizeof(s_date_str), "%s - %s %s", s_dow_short, s_month_name_short, s_date);  
  text_layer_set_text(s_layer_date, s_date_str);
  
  char date_yyyymmdd[9];
  char time_hhmm[5];
  strftime(date_yyyymmdd, sizeof(date_yyyymmdd), "%Y%m%d", tick_time);
  strftime(time_hhmm, sizeof(time_hhmm), "%H%M", tick_time);
  if(game_time(s_season_schedule, 16, date_yyyymmdd, time_hhmm)) {
    game_time_alert();
  }
  else {
    clear_game_time_alert();
  }
}

void game_time_alert() {
  static const uint32_t const segments[] = {250, 500, 250, 500, 250, 500, 250, 500, 500, 500, 500, 500, 500}; // J! - E! - T! - S! - JETS! - JETS! - JETS!
  VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
  };
  vibes_enqueue_custom_pattern(pat);  
  text_layer_set_background_color(s_layer_gametime, GColorFromRGBA(0, 0, 0, 255));
  text_layer_set_text(s_layer_gametime, "GAME TIME !!!!!\nJ! E! T! S!\nJETS! JETS! JETS!");
}

void clear_game_time_alert() {
  text_layer_set_background_color(s_layer_gametime, GColorFromRGBA(0, 0, 0, 0));
  text_layer_set_text(s_layer_gametime, ""); 
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  s_window_main = window_create();  
  window_set_window_handlers(s_window_main, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_window_main, true);
  update_time();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  set_schedule(s_season_schedule);
}

static void deinit() {
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}