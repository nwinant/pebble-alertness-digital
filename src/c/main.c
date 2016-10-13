#include <pebble.h>
#include "modules/vibe_patterns.h"
#include "modules/configuration.h"

/* ====================================================================================
 *
 * Properties
 *
 * ====================================================================================
 */
static Window        *s_main_window;
static TextLayer     *s_primary_layer;
static TextLayer     *s_complications_layer;
static TextLayer     *s_time_layer;
static TextLayer     *s_date_layer;
static TextLayer     *s_countdown_layer;
static TextLayer     *s_connection_layer;
static TextLayer     *s_battery_layer;
static uint8_t       alert_interval_remainder;
static uint8_t       curr_hour;
static bool          alert_active;
static Configuration config;
                          // TODO: track last alert!


/* ====================================================================================
 *
 * Display
 *
 * ====================================================================================
 */
static void main_window_load(Window *window) {
  // Prepare to calculate layout
  uint32_t time_layer_size_y = 62;  
  Layer *window_layer        = window_get_root_layer(window);
  GRect bounds               = layer_get_bounds(window_layer);
  GRect primary_bounds       = bounds;
  GRect comp_bounds          = bounds;
  GRect time_bounds          = primary_bounds;
  GRect date_bounds          = comp_bounds;
  GRect countdown_bounds     = comp_bounds;
  GRect connection_bounds    = comp_bounds;
  GRect battery_bounds       = comp_bounds;
  
  // Calculate layout
  primary_bounds.size.h      = (bounds.size.h / 2) + (time_layer_size_y / 2) + -8; // Includes fudge factor to tweak layout
  comp_bounds.size.h         = bounds.size.h - primary_bounds.size.h;
  date_bounds.size.h         = 28;
  countdown_bounds.size.h    = PBL_IF_ROUND_ELSE(38, 33);
  connection_bounds.size.w   = connection_bounds.size.w - 10;
  connection_bounds.size.h   = PBL_IF_ROUND_ELSE(23, 20);
  connection_bounds.origin.x = 5;
  battery_bounds.size.w      = battery_bounds.size.w - 10;
  battery_bounds.size.h      = PBL_IF_ROUND_ELSE(23, 20);
  battery_bounds.origin.x    = 5;
  if (config.invert_layout) {
    comp_bounds.origin.y       = 0;
    primary_bounds.origin.y    = bounds.size.h - primary_bounds.size.h;
    time_bounds.origin.y       = (primary_bounds.size.h / 2) - (time_layer_size_y / 2);
    date_bounds.origin.y       = comp_bounds.size.h - date_bounds.size.h;
    countdown_bounds.origin.y  = 0;
    connection_bounds.origin.y = 0;
    battery_bounds.origin.y    = 0;
  } else {
    primary_bounds.origin.y    = 0;
    comp_bounds.origin.y       = primary_bounds.size.h;
    time_bounds.origin.y       = primary_bounds.size.h - time_layer_size_y;
    date_bounds.origin.y       = 0;
    countdown_bounds.origin.y  = comp_bounds.size.h - countdown_bounds.size.h;
    connection_bounds.origin.y = comp_bounds.size.h - connection_bounds.size.h;
    battery_bounds.origin.y    = comp_bounds.size.h - battery_bounds.size.h;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Calculated bounds...");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Window y:        %d", bounds.origin.y);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Window h:        %d", bounds.size.h);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Primary y:       %d", primary_bounds.origin.y);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Primary h:       %d", primary_bounds.size.h);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Complications y: %d", comp_bounds.origin.y);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Complications h: %d", comp_bounds.size.h);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Rendered bounds...");
  
  // Primary layer
  s_primary_layer = text_layer_create(primary_bounds);
  layer_add_child(window_layer, text_layer_get_layer(s_primary_layer));
  GRect calc_primary_bounds = layer_get_bounds(text_layer_get_layer(s_primary_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Primary y:       %d", calc_primary_bounds.origin.y);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Primary h:       %d", calc_primary_bounds.size.h);
  
  // Complications layer
  s_complications_layer = text_layer_create(comp_bounds);
  layer_add_child(window_layer, text_layer_get_layer(s_complications_layer));
  GRect calc_comp_bounds = layer_get_bounds(text_layer_get_layer(s_complications_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Complications y: %d", calc_comp_bounds.origin.y);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Complications h: %d", calc_comp_bounds.size.h);
  
  // Time layer
  s_time_layer = text_layer_create(time_bounds);
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(text_layer_get_layer(s_primary_layer), text_layer_get_layer(s_time_layer));
  
  // Date layer
  s_date_layer = text_layer_create(date_bounds);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(text_layer_get_layer(s_complications_layer), text_layer_get_layer(s_date_layer));
  
  // Countdown layer
  s_countdown_layer = text_layer_create(countdown_bounds);
  text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_countdown_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_countdown_layer));
  layer_add_child(text_layer_get_layer(s_complications_layer), text_layer_get_layer(s_countdown_layer));
  
  // Connection layer
  s_connection_layer = text_layer_create(connection_bounds);
  text_layer_set_font(s_connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_connection_layer, GTextAlignmentLeft);
  layer_add_child(text_layer_get_layer(s_complications_layer), text_layer_get_layer(s_connection_layer));
  
  // Battery layer
  s_battery_layer = text_layer_create(battery_bounds);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  layer_add_child(text_layer_get_layer(s_complications_layer), text_layer_get_layer(s_battery_layer));
  
  // Constant colors
  text_layer_set_background_color(s_primary_layer, GColorClear);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_background_color(s_countdown_layer, GColorClear);
  text_layer_set_background_color(s_connection_layer, GColorClear);
  text_layer_set_background_color(s_battery_layer, GColorClear);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_countdown_layer);
  text_layer_destroy(s_connection_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_complications_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_primary_layer);
}

static void refresh_display_layout(void) {
  layer_set_hidden((Layer *)s_countdown_layer,  !config.alerts_enabled);
  layer_set_hidden((Layer *)s_connection_layer, !config.show_connection_status);
  layer_set_hidden((Layer *)s_battery_layer,    !config.show_battery_status);
  if (!config.alerts_enabled || (alert_interval_remainder > 0)) {
    window_set_background_color(s_main_window, config.main_bg_color);
    text_layer_set_background_color(s_complications_layer, config.comps_bg_color);
    text_layer_set_text_color(s_time_layer,       config.main_fg_color);
    text_layer_set_text_color(s_date_layer,       config.comps_fg_color);
    text_layer_set_text_color(s_countdown_layer,  config.comps_fg_color);
    text_layer_set_text_color(s_connection_layer, config.comps_fg_color);
    text_layer_set_text_color(s_battery_layer,    config.comps_fg_color);
  } else {
    window_set_background_color(s_main_window, config.alert_bg_color);
    text_layer_set_background_color(s_complications_layer, GColorClear);
    text_layer_set_text_color(s_time_layer,       config.alert_fg_color);
    text_layer_set_text_color(s_date_layer,       config.alert_fg_color);
    text_layer_set_text_color(s_countdown_layer,  config.alert_fg_color);
    text_layer_set_text_color(s_connection_layer, config.alert_fg_color);
    text_layer_set_text_color(s_battery_layer,    config.alert_fg_color);
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
  if (config.alerts_enabled) {
    static char countdown_buffer[3];
    //static char countdown_buffer[10]; // FIXME: remove!
    if (alert_active) {
      snprintf(countdown_buffer, sizeof(countdown_buffer), "%d", config.alert_frequency_mins - alert_interval_remainder);
      // FIXME: remove (parens) stuff
      //snprintf(countdown_buffer, sizeof(countdown_buffer), "%d (%d)", config.alert_frequency_mins - alert_interval_remainder, alert_frequency_mins);
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating time");
  
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Update counters
  curr_hour                = tick_time->tm_hour;
  alert_interval_remainder = tick_time->tm_min % config.alert_frequency_mins;
  alert_active             = (curr_hour >= config.alert_start_hour) && (curr_hour < config.alert_end_hour);
  
  // Refresh the display
  refresh_display_layout();
  refresh_display_data(tick_time);
}


/* ====================================================================================
 *
 * Handlers
 *
 * ====================================================================================
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  if (config.alerts_enabled && alert_active && (alert_interval_remainder == 0)) {
    vibes_cancel();
    vibes_enqueue_custom_pattern(config.alert_vibe_pattern);
  }
}

static void config_update_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "New config incoming...");
  config = update_config(iter, context);
  update_time();  // Refresh watch with changes
}

static void bluetooth_handler(bool connected) {
  text_layer_set_text(s_connection_layer, connected ? "..." : "  !");
}

static void battery_handler(BatteryChargeState charge_state) {
  // FIXME: should this be worked into refresh_display_data()?
  static char battery_text[] = "100%";
  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "...");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(s_battery_layer, battery_text);
}

static void register_handlers(void) {
  // Configuration
  app_message_register_inbox_received(config_update_handler);
  app_message_open(128, 128);
  // Ticks
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Battery & bluetooth
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_handler
  });
  battery_state_service_subscribe(battery_handler);
  bluetooth_handler(connection_service_peek_pebble_app_connection());
  battery_handler(battery_state_service_peek());
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
  config = load_config();
  
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