#include <pebble.h>

/* ====================================================================================
 *
 * Properties
 *
 * ====================================================================================
 */

// Constants
/*
static const uint32_t alert_segments1[] = { 200, 100, 200, 500,
                                    1000, 500,
                                    200, 100, 200 };
*/
/*
static const uint32_t alert_segments_long[] = { 100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100 };
*/
/*
static const uint32_t alert_segments_extended[] = { 100, 200, 100, 400,
                                    800, 200,
                                    200, 200,
                                    800, 200,
                                    200, 200,
                                    //800, 200,
                                    800, 400,
                                    100, 200, 100 };
*/
static const uint32_t alert_segments_pulse[] = { 100, 200, 100, 400,
                                    500, 200, 500, 200,
                                    200, 100, 200, 100,
                                    500, 200, 500, 200,
                                    100, 200, 100 };

// Vars

static Window    *s_main_window;
static TextLayer *s_background_layer;
static TextLayer *s_complications_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_countdown_layer;
static TextLayer *s_connection_layer;
static TextLayer *s_battery_layer;
static uint8_t   alert_interval_remainder;
static uint8_t   curr_hour;
static bool      alert_active;
                             // TODO: track last alert!

// Clay config

static GColor      main_bg_color;
static GColor      main_fg_color;
static GColor      comps_bg_color;
static GColor      comps_fg_color;
static GColor      alert_bg_color;
static GColor      alert_fg_color;
static bool        alerts_enabled;
static uint8_t     alert_frequency_mins;
static uint8_t     alert_start_hour;
static uint8_t     alert_end_hour;
static bool        show_connection_status;
static bool        show_battery_status;
// TODO: configurize:
static VibePattern alert_vibe_pattern = {
  .durations = alert_segments_pulse,
  .num_segments = ARRAY_LENGTH(alert_segments_pulse)
};


/* ====================================================================================
 *
 * Display
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
    0, bounds.size.h - PBL_IF_ROUND_ELSE(38, 33),
    bounds.size.w, PBL_IF_ROUND_ELSE(38, 33)));
  text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_countdown_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_countdown_layer));
  
  // Connection layer
  s_connection_layer = text_layer_create(GRect(
    5, bounds.size.h - PBL_IF_ROUND_ELSE(23, 20),
    bounds.size.w - 10, PBL_IF_ROUND_ELSE(23, 20)));
  text_layer_set_font(s_connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_connection_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_connection_layer));
  
  // Battery layer
  s_battery_layer = text_layer_create(GRect(
    5, bounds.size.h - PBL_IF_ROUND_ELSE(23, 20),
    bounds.size.w - 10, PBL_IF_ROUND_ELSE(23, 20)));
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
  
  // Constant colors
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_background_color(s_countdown_layer, GColorClear);
  text_layer_set_background_color(s_connection_layer, GColorClear);
  text_layer_set_background_color(s_battery_layer, GColorClear);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_countdown_layer);
  text_layer_destroy(s_connection_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_complications_layer);
  text_layer_destroy(s_background_layer);
}

static void refresh_display_layout(void) {
  layer_set_hidden((Layer *)s_countdown_layer, !alerts_enabled);
  layer_set_hidden((Layer *)s_connection_layer, !show_connection_status);
  layer_set_hidden((Layer *)s_battery_layer, !show_battery_status);
  
  if (!alerts_enabled || (alert_interval_remainder > 0)) {
    text_layer_set_background_color(s_background_layer, main_bg_color);
    text_layer_set_background_color(s_complications_layer, comps_bg_color);
    text_layer_set_text_color(s_time_layer, main_fg_color);
    text_layer_set_text_color(s_date_layer, comps_fg_color);
    text_layer_set_text_color(s_countdown_layer, comps_fg_color);
    text_layer_set_text_color(s_connection_layer, comps_fg_color);
    text_layer_set_text_color(s_battery_layer, comps_fg_color);
  } else {
    text_layer_set_background_color(s_background_layer, alert_bg_color);
    text_layer_set_background_color(s_complications_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, alert_fg_color);
    text_layer_set_text_color(s_date_layer, alert_fg_color);
    text_layer_set_text_color(s_countdown_layer, alert_fg_color);
    text_layer_set_text_color(s_connection_layer, alert_fg_color);
    text_layer_set_text_color(s_battery_layer, alert_fg_color);
  }
}

static void refresh_display_data(struct tm *tick_time) {
  // Time
  static char time_buffer[8];
  strftime(time_buffer, sizeof(time_buffer), clock_is_24h_style() ? "%H:%M" : "%l:%M", tick_time);
  text_layer_set_text(s_time_layer, time_buffer);
  
  // Date
  static char date_buffer[11];
  strftime(date_buffer, sizeof(date_buffer), "%a %b %e", tick_time);
  text_layer_set_text(s_date_layer, date_buffer);
  
  // Countdown
  if (alerts_enabled) {
    static char countdown_buffer[3];
    //static char countdown_buffer[10]; // FIXME: remove!
    if (alert_active) {
      snprintf(countdown_buffer, sizeof(countdown_buffer), "%d", alert_frequency_mins - alert_interval_remainder);
      // FIXME: remove (parens) stuff
      //snprintf(countdown_buffer, sizeof(countdown_buffer), "%d (%d)", alert_frequency_mins - alert_interval_remainder, alert_frequency_mins);
    } else {
      snprintf(countdown_buffer, sizeof(countdown_buffer), "-");
    }
    text_layer_set_text(s_countdown_layer, countdown_buffer);
  }
}


/* ====================================================================================
 *
 * Timing
 *
 * ====================================================================================
 */
static void update_time(void) {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Update counters
  curr_hour = tick_time->tm_hour;
  alert_interval_remainder = tick_time->tm_min % alert_frequency_mins;
  alert_active = (curr_hour >= alert_start_hour) && (curr_hour < alert_end_hour);
  
  // Refresh the display
  refresh_display_layout();
  refresh_display_data(tick_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  if (alerts_enabled && alert_active && (alert_interval_remainder == 0)) {
    vibes_cancel();
    vibes_enqueue_custom_pattern(alert_vibe_pattern);
  }
}


/* ====================================================================================
 *
 * Configuration
 *
 * ====================================================================================
 */
static void load_config(void) {
  // Display
  main_bg_color = persist_exists(MESSAGE_KEY_MainBackgroundColor) 
    ? GColorFromHEX(persist_read_int(MESSAGE_KEY_MainBackgroundColor))
    : GColorBlack;
  main_fg_color = persist_exists(MESSAGE_KEY_MainForegroundColor) 
    ? GColorFromHEX(persist_read_int(MESSAGE_KEY_MainForegroundColor))
    : GColorWhite;
  comps_bg_color = persist_exists(MESSAGE_KEY_ComplicationsBackgroundColor) 
    ? GColorFromHEX(persist_read_int(MESSAGE_KEY_ComplicationsBackgroundColor))
    : PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
  comps_fg_color = persist_exists(MESSAGE_KEY_ComplicationsForegroundColor) 
    ? GColorFromHEX(persist_read_int(MESSAGE_KEY_ComplicationsForegroundColor))
    : GColorBlack;
  alert_bg_color = persist_exists(MESSAGE_KEY_AlertBackgroundColor) 
    ? GColorFromHEX(persist_read_int(MESSAGE_KEY_AlertBackgroundColor))
    : GColorWhite;
  alert_fg_color = persist_exists(MESSAGE_KEY_AlertForegroundColor) 
    ? GColorFromHEX(persist_read_int(MESSAGE_KEY_AlertForegroundColor))
    : PBL_IF_COLOR_ELSE(GColorDarkGreen, GColorBlack);
  
  // Alerts
  alerts_enabled = persist_exists(MESSAGE_KEY_AlertsEnabled) 
    ? persist_read_bool(MESSAGE_KEY_AlertsEnabled)
    : 1;
  alert_frequency_mins = persist_exists(MESSAGE_KEY_AlertFrequency) 
    ? persist_read_int(MESSAGE_KEY_AlertFrequency)
    : 15;
  alert_start_hour = persist_exists(MESSAGE_KEY_AlertStartHour) 
    ? persist_read_int(MESSAGE_KEY_AlertStartHour)
    : 9;
  alert_end_hour = persist_exists(MESSAGE_KEY_AlertEndHour) 
    ? persist_read_int(MESSAGE_KEY_AlertEndHour)
    : 22;
  
  // Misc
  show_connection_status = persist_exists(MESSAGE_KEY_ShowConnectionStatus) 
    ? persist_read_bool(MESSAGE_KEY_ShowConnectionStatus)
    : 1;
  show_battery_status = persist_exists(MESSAGE_KEY_ShowBatteryStatus) 
    ? persist_read_bool(MESSAGE_KEY_ShowBatteryStatus)
    : 1;
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Display prefs
  Tuple *main_bg_color_t = dict_find(iter, MESSAGE_KEY_MainBackgroundColor);
  if (main_bg_color_t) {
    persist_write_int(MESSAGE_KEY_MainBackgroundColor, main_bg_color_t->value->int32);
  }
  Tuple *main_fg_color_t = dict_find(iter, MESSAGE_KEY_MainForegroundColor);
  if (main_fg_color_t) {
    persist_write_int(MESSAGE_KEY_MainForegroundColor, main_fg_color_t->value->int32);
  }
  Tuple *comps_bg_color_t = dict_find(iter, MESSAGE_KEY_ComplicationsBackgroundColor);
  if (comps_bg_color_t) {
    persist_write_int(MESSAGE_KEY_ComplicationsBackgroundColor, comps_bg_color_t->value->int32);
  }
  Tuple *comps_fg_color_t = dict_find(iter, MESSAGE_KEY_ComplicationsForegroundColor);
  if (comps_fg_color_t) {
    persist_write_int(MESSAGE_KEY_ComplicationsForegroundColor, comps_fg_color_t->value->int32);
  }
  Tuple *alert_bg_color_t = dict_find(iter, MESSAGE_KEY_AlertBackgroundColor);
  if (alert_bg_color_t) {
    persist_write_int(MESSAGE_KEY_AlertBackgroundColor, alert_bg_color_t->value->int32);
  }
  Tuple *alert_fg_color_t = dict_find(iter, MESSAGE_KEY_AlertForegroundColor);
  if (alert_fg_color_t) {
    persist_write_int(MESSAGE_KEY_AlertForegroundColor, alert_fg_color_t->value->int32);
  }
  
  // Alert prefs
  Tuple *alerts_enabled_t = dict_find(iter, MESSAGE_KEY_AlertsEnabled);
  if (alerts_enabled_t) {
    persist_write_bool(MESSAGE_KEY_AlertsEnabled, alerts_enabled_t->value->int32 == 1);
  }
  Tuple *alert_frequency_mins_t = dict_find(iter, MESSAGE_KEY_AlertFrequency);
  if (alert_frequency_mins_t) {
    // FIXME: needs more rigorous error-checking...
    persist_write_int(MESSAGE_KEY_AlertFrequency, 
                      atoi(alert_frequency_mins_t->value->cstring));
  }
  Tuple *alert_start_hour_t = dict_find(iter, MESSAGE_KEY_AlertStartHour);
  if (alert_start_hour_t) {
    persist_write_int(MESSAGE_KEY_AlertStartHour, alert_start_hour_t->value->int32);
  }
  Tuple *alert_end_hour_t = dict_find(iter, MESSAGE_KEY_AlertEndHour);
  if (alert_end_hour_t) {
    persist_write_int(MESSAGE_KEY_AlertEndHour, alert_end_hour_t->value->int32);
  }
  
  // Misc
  Tuple *show_connection_status_t = dict_find(iter, MESSAGE_KEY_ShowConnectionStatus);
  if (show_connection_status_t) {
    persist_write_bool(MESSAGE_KEY_ShowConnectionStatus, show_connection_status_t->value->int32 == 1);
  }
  Tuple *show_battery_status_t = dict_find(iter, MESSAGE_KEY_ShowBatteryStatus);
  if (show_battery_status_t) {
    persist_write_bool(MESSAGE_KEY_ShowBatteryStatus, show_battery_status_t->value->int32 == 1);
  }
  
  // Refresh watch with changes
  load_config();
  update_time();
}


/* ====================================================================================
 *
 * Handlers
 *
 * ====================================================================================
 */
static void handle_bluetooth(bool connected) {
  text_layer_set_text(s_connection_layer, connected ? "..." : "  !");
}

static void handle_battery(BatteryChargeState charge_state) {
  // FIXME: should this be worked into refresh_display_data()?
  static char battery_text[] = "100%";
  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "...");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(s_battery_layer, battery_text);
}

static void register_config_handler(void) {
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
}

static void register_handlers(void) {
  register_config_handler();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handle_bluetooth
  });
  battery_state_service_subscribe(handle_battery);
  // Update battery & bluetooth
  handle_bluetooth(connection_service_peek_pebble_app_connection());
  handle_battery(battery_state_service_peek());
}

static void deregister_handlers(void) {
  tick_timer_service_unsubscribe();
  connection_service_unsubscribe();
  battery_state_service_unsubscribe();
  // FIXME: do we need to unsub the config handler?
}


/* ====================================================================================
 *
 * System hooks
 *
 * ====================================================================================
 */
static void init() {
  load_config();
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  register_handlers();
  
  // Display current time
  update_time();
}

static void deinit() {
  // Destroy Window
  deregister_handlers();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}