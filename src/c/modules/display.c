#include <pebble.h>
#include "inttypes.h"
#include "configuration.h"
#include "alert_handler.h"


/* ====  Debugging switches  ======================================================= */

#define DEV_EXCESSIVE_LOGGING 0


/* ====  Variables  ================================================================ */

static Window        *s_main_window;
static TextLayer     *s_primary_layer;
static TextLayer     *s_complications_layer;
static TextLayer     *s_time_layer;
static TextLayer     *s_date_layer;
static TextLayer     *s_countdown_layer;
static TextLayer     *s_connection_layer;
static TextLayer     *s_battery_layer;

static GFont s_time_font;

struct TextLayer *get_connection_layer(void) {
  return s_connection_layer;
}

struct TextLayer *get_battery_layer(void) {
  return s_battery_layer;
}


/* ====  Create / Destroy  ========================================================= */

static void main_window_load(Window *window) {  
  // Prepare to calculate layout
  //uint32_t time_layer_size_y = 62;  
  uint32_t time_layer_size_y = 60;
  int32_t time_bounds_origin_y_offset = -10;  
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
  primary_bounds.size.h      = (bounds.size.h / 2) + (time_layer_size_y / 2);
  comp_bounds.size.h         = bounds.size.h - primary_bounds.size.h;
  //date_bounds.size.h         = 28;
  date_bounds.size.h         = 32;
  //countdown_bounds.size.h    = PBL_IF_ROUND_ELSE(38, 33);
  //countdown_bounds.size.h    = PBL_IF_ROUND_ELSE(36, 31);
  countdown_bounds.size.h    = PBL_IF_ROUND_ELSE(32, 27);
  connection_bounds.size.w   = connection_bounds.size.w - 10;
  connection_bounds.size.h   = PBL_IF_ROUND_ELSE(23, 20);
  connection_bounds.origin.x = 5;
  battery_bounds.size.w      = battery_bounds.size.w - 10;
  battery_bounds.size.h      = PBL_IF_ROUND_ELSE(23, 20);
  battery_bounds.origin.x    = 5;
  if (!config.invert_layout) {
    comp_bounds.origin.y       = 0;
    primary_bounds.origin.y    = bounds.size.h - primary_bounds.size.h;
    time_bounds.origin.y       = (primary_bounds.size.h / 2) - (time_layer_size_y / 2) + time_bounds_origin_y_offset; // Includes fudge factor to tweak layout
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
  if (DEV_EXCESSIVE_LOGGING) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Calculated bounds...");
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Window y:        %d", bounds.origin.y);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Window h:        %d", bounds.size.h);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Primary y:       %d", primary_bounds.origin.y);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Primary h:       %d", primary_bounds.size.h);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Complications y: %d", comp_bounds.origin.y);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Complications h: %d", comp_bounds.size.h);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Rendered bounds...");
  }
  
  // Primary layer
  s_primary_layer = text_layer_create(primary_bounds);
  layer_add_child(window_layer, text_layer_get_layer(s_primary_layer));
  GRect calc_primary_bounds = layer_get_bounds(text_layer_get_layer(s_primary_layer));
  if (DEV_EXCESSIVE_LOGGING) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Primary y:       %d", calc_primary_bounds.origin.y);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Primary h:       %d", calc_primary_bounds.size.h);
  }
  
  // Complications layer
  s_complications_layer = text_layer_create(comp_bounds);
  layer_add_child(window_layer, text_layer_get_layer(s_complications_layer));
  GRect calc_comp_bounds = layer_get_bounds(text_layer_get_layer(s_complications_layer));
  if (DEV_EXCESSIVE_LOGGING) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Complications y: %d", calc_comp_bounds.origin.y);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- Complications h: %d", calc_comp_bounds.size.h);
  }
  
  // Time layer
  s_time_layer = text_layer_create(time_bounds);
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
//  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_font(s_time_layer, s_time_font);

  
  
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(text_layer_get_layer(s_primary_layer), text_layer_get_layer(s_time_layer));
  
  // Date layer
  s_date_layer = text_layer_create(date_bounds);
  //text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(text_layer_get_layer(s_complications_layer), text_layer_get_layer(s_date_layer));
  
  // Countdown layer
  s_countdown_layer = text_layer_create(countdown_bounds);
  //text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  //text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
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
  fonts_unload_custom_font(s_time_font);
}

void init_display(Window *new_s_main_window) {
  // Create main Window element and assign to pointer
  s_main_window = new_s_main_window;
  
  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPEN_DIN_SCHRIFTEN_ENGSHRIFT_60));
  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPEN_DIN_SCHRIFTEN_ENGSHRIFT_48));
  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN1451_ALT_G_60));
  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_8_BIT_WONDER_48));
//    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_1451_STD_ENGSCHRIFT_60));
  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DINEN_72));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DINEN_62));

  
  
  
  
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}


/* ====  Refresh / update  ========================================================= */

void refresh_display_layout(void) {
  layer_set_hidden((Layer *)s_countdown_layer,  !config.alerts_enabled);
  layer_set_hidden((Layer *)s_connection_layer, !config.show_connection_status);
  layer_set_hidden((Layer *)s_battery_layer,    !config.show_battery_status);
  //if (!config.alerts_enabled || !is_alert_timer_running() || get_alert_interval_remainder() > 0) {
  if (!is_alert_currently_active()) {
    window_set_background_color(s_main_window, config.main_bg_color);
    text_layer_set_background_color(s_complications_layer, config.comps_bg_color);
    text_layer_set_text_color(s_time_layer,       config.main_fg_color);
    text_layer_set_text_color(s_date_layer,       config.comps_fg_color);
    text_layer_set_text_color(s_countdown_layer,  config.comps_fg_color);
    text_layer_set_text_color(s_connection_layer, config.comps_fg_color);
    text_layer_set_text_color(s_battery_layer,    config.comps_fg_color);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- AlertBackgroundColor: %" PRIu32, persist_read_int(MESSAGE_KEY_AlertBackgroundColor));
    
    window_set_background_color(s_main_window, config.alert_bg_color);
    text_layer_set_background_color(s_complications_layer, GColorClear);
    text_layer_set_text_color(s_time_layer,       config.alert_fg_color);
    text_layer_set_text_color(s_date_layer,       config.alert_fg_color);
    text_layer_set_text_color(s_countdown_layer,  config.alert_fg_color);
    text_layer_set_text_color(s_connection_layer, config.alert_fg_color);
    text_layer_set_text_color(s_battery_layer,    config.alert_fg_color);
  }
}

void refresh_display_data(struct tm *tick_time) {
  // Time
  static char time_buffer[8];
  strftime(time_buffer, sizeof(time_buffer), clock_is_24h_style() ? "%H:%M" : "%l:%M", tick_time);
  text_layer_set_text(s_time_layer, time_buffer);
  
  // Date
  //static char date_buffer[11];
  //strftime(date_buffer, sizeof(date_buffer), "%a %b %e", tick_time);
  static char date_buffer[15];
  strftime(date_buffer, sizeof(date_buffer), config.date_format, tick_time);
  text_layer_set_text(s_date_layer, date_buffer);
  
  // Countdown
  if (config.alerts_enabled) {
    static char countdown_buffer[3];
    //static char countdown_buffer[10]; // FIXME: remove!
    if (is_alert_timer_running()) {
      snprintf(countdown_buffer, sizeof(countdown_buffer), "%d", config.alert_frequency_mins - get_alert_interval_remainder());
      // FIXME: remove (parens) stuff
      //snprintf(countdown_buffer, sizeof(countdown_buffer), "%d (%d)", config.alert_frequency_mins - alert_interval_remainder, alert_frequency_mins);
    } else {
      //snprintf(countdown_buffer, sizeof(countdown_buffer), "-");
      snprintf(countdown_buffer, sizeof(countdown_buffer), " ");
    }
    text_layer_set_text(s_countdown_layer, countdown_buffer);
  }
}
