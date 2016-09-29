#include <pebble.h>
#define ALERT_INTERVAL_MINS 15


/* ====================================================================================
 *
 * Properties
 *
 * ====================================================================================
 */

//90-31=59
//84-31=53
  
// Constants
//static const uint32_t time_layer_origin_y=PBL_IF_ROUND_ELSE(58, 52);
//static const uint32_t time_layer_size_y=55;
//static const uint32_t time_layer_origin_y=PBL_IF_ROUND_ELSE(51, 45);
//static const uint32_t time_layer_size_y=62;
static const uint8_t alarm_start_hour=9;
static const uint8_t alarm_end_hour=22;
/*
static const uint32_t alarm_segments1[] = { 200, 100, 200, 500,
                                    1000, 500,
                                    200, 100, 200 };
*/
/*
static const uint32_t alarm_segments_long[] = { 100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100 };
*/
/*
static const uint32_t alarm_segments_extended[] = { 100, 200, 100, 400,
                                    800, 200,
                                    200, 200,
                                    800, 200,
                                    200, 200,
                                    //800, 200,
                                    800, 400,
                                    100, 200, 100 };
*/
static const uint32_t alarm_segments_pulse[] = { 100, 200, 100, 400,
                                    500, 200, 500, 200,
                                    200, 100, 200, 100,
                                    500, 200, 500, 200,
                                    100, 200, 100 };
  
static const VibePattern alert_vibe_pattern = {
  .durations = alarm_segments_pulse,
  .num_segments = ARRAY_LENGTH(alarm_segments_pulse)
};

// Vars

static Window *s_main_window;
static TextLayer *s_background_layer;
static TextLayer *s_complications_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_countdown_layer;

static uint8_t alert_interval_remainder;
static uint8_t curr_hour;
static bool alarm_active;


/* ====================================================================================
 *
 * Hooks
 *
 * ====================================================================================
 */
static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Background layer
  s_background_layer = text_layer_create(GRect(
    0, 0, 
    bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer));
  
  // Time layer
  uint32_t time_layer_offset_y = -8; // Fudge factor to tweak layout
  uint32_t time_layer_size_y = 62;
  uint32_t time_layer_origin_y=bounds.size.h / 2 - (time_layer_size_y / 2) + time_layer_offset_y;
  s_time_layer = text_layer_create(GRect(
    0, time_layer_origin_y,
    bounds.size.w, time_layer_size_y));
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  // Complications layer
  s_complications_layer = text_layer_create(GRect(
    0, time_layer_origin_y + time_layer_size_y,
    bounds.size.w, bounds.size.h - (time_layer_origin_y + time_layer_size_y)));
  layer_add_child(window_layer, text_layer_get_layer(s_complications_layer));
  
  // Date layer
  s_date_layer = text_layer_create(GRect(
    0, time_layer_origin_y + time_layer_size_y,
    bounds.size.w, 28));
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  // Countdown layer
  s_countdown_layer = text_layer_create(GRect(
    0, bounds.size.h - PBL_IF_ROUND_ELSE(38, 32),
    bounds.size.w, PBL_IF_ROUND_ELSE(38, 32)));
  text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_countdown_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_countdown_layer));
  
  // Constant colors
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_background_color(s_countdown_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_countdown_layer, GColorBlack);
  
  
  
  /*
  //    ...    FIXME: remove!
  vibes_cancel();
  vibes_enqueue_custom_pattern(alert_vibe_pattern);
  */
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_countdown_layer);
  text_layer_destroy(s_complications_layer);
  text_layer_destroy(s_background_layer);
}

static void update_complications(struct tm *tick_time) {
  static char date_buffer[11];
  strftime(date_buffer, sizeof(date_buffer), "%a %b %e", tick_time);
  text_layer_set_text(s_date_layer, date_buffer);
  
  static char countdown_buffer[3];
  if (alarm_active) {
    snprintf(countdown_buffer, sizeof(countdown_buffer), "%d", ALERT_INTERVAL_MINS - alert_interval_remainder);
  } else {
    snprintf(countdown_buffer, sizeof(countdown_buffer), "--");
  }
  text_layer_set_text(s_countdown_layer, countdown_buffer);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Update counters
  curr_hour = tick_time->tm_hour;
  alert_interval_remainder = tick_time->tm_min %ALERT_INTERVAL_MINS;
  alarm_active = (curr_hour >= alarm_start_hour) && (curr_hour < alarm_end_hour);
  
  // Write the current hours and minutes into a buffer
  static char time_buffer[8];
  strftime(time_buffer, sizeof(time_buffer), clock_is_24h_style() ? "%H:%M" : "%l:%M", tick_time);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "update_time: %s", time_buffer); // FIXME: remove!
  
  
  // Set colors
  if (alert_interval_remainder > 0) {
    text_layer_set_background_color(s_background_layer, GColorBlack);
    
    text_layer_set_background_color(s_complications_layer, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
    text_layer_set_text_color(s_time_layer, GColorCeleste);
  } else {
    text_layer_set_background_color(s_background_layer, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
    text_layer_set_background_color(s_complications_layer, GColorWhite);
    text_layer_set_text_color(s_time_layer, PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, time_buffer);
  update_complications(tick_time);
  /*
  // FIXME: remove!
  static char countdown_buffer[3];
  snprintf(countdown_buffer, sizeof(countdown_buffer), "??");
  text_layer_set_text(s_countdown_layer, countdown_buffer);
  */
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  APP_LOG(APP_LOG_LEVEL_INFO, "-->"); // FIXME: remove!
  APP_LOG(APP_LOG_LEVEL_INFO, "tick_handler: %d", units_changed); // FIXME: remove!
  
  update_time();
  
  APP_LOG(APP_LOG_LEVEL_INFO, "alarm_active? %d", alarm_active); // FIXME: remove!
  APP_LOG(APP_LOG_LEVEL_INFO, "alert_interval_remainder? %u", alert_interval_remainder); // FIXME: remove!
  
  if (alarm_active && (alert_interval_remainder == 0)) {
    APP_LOG(APP_LOG_LEVEL_INFO, "ALARM!"); // FIXME: remove!
    vibes_cancel();
    vibes_enqueue_custom_pattern(alert_vibe_pattern);
  }
  
  /*
  // FIXME: remove!
  static char countdown_buffer[3];
  snprintf(countdown_buffer, sizeof(countdown_buffer), "!!");
  text_layer_set_text(s_countdown_layer, countdown_buffer);
  */
  APP_LOG(APP_LOG_LEVEL_INFO, " --"); // FIXME: remove!
}


/* ====================================================================================
 *
 * Hooks
 *
 * ====================================================================================
 */

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  //tick_timer_service_subscribe(MINUTE_UNIT | HOUR_UNIT | DAY_UNIT, tick_handler);

  // Display current time
  update_time();
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}