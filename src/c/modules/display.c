#include <pebble.h>
#include "inttypes.h"
#include "configuration.h"
#include "alert_handler.h"
#include "fonts.h"


/* ====  Debugging switches  ======================================================= */

#define DEV_EXCESSIVE_LOGGING 0


/* ====  Variables  ================================================================ */

static Window    *s_main_window;
static GRect     bounds;
static GPoint    center;
static int       radius;
static TextLayer *s_primary_layer;
static TextLayer *s_complications_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_countdown_layer;
static TextLayer *s_connection_layer;
static TextLayer *s_battery_layer;

struct TextLayer *get_connection_layer(void) {
  return s_connection_layer;
}

struct TextLayer *get_battery_layer(void) {
  return s_battery_layer;
}


/* ====  Create / Destroy  ========================================================= */

static void configure_text_layer(TextLayer *text_layer, GTextAlignment alignment, GColor bgcolor, TextLayer * parent_text_layer) {
  text_layer_set_text_alignment(text_layer, alignment);
  layer_add_child(text_layer_get_layer(parent_text_layer), text_layer_get_layer(text_layer));
  text_layer_set_background_color(text_layer, bgcolor);
}

void update_display_config(void) {
  unload_all_fonts();
  text_layer_set_font(s_time_layer,       get_font_by_name(config.time_font_name));
  text_layer_set_font(s_date_layer,       get_font_by_name(config.date_font_name));
  text_layer_set_font(s_countdown_layer,  get_font_by_name(config.countdown_font_name));
  text_layer_set_font(s_connection_layer, get_font_by_name(config.details_font_name));
  text_layer_set_font(s_battery_layer,    get_font_by_name(config.details_font_name));
}

static GPoint calc_round_point(int32_t angle) {
  GPoint result = {
    .x = (sin_lookup(angle)  * radius / TRIG_MAX_RATIO) + center.x,
    .y = (-cos_lookup(angle) * radius / TRIG_MAX_RATIO) + center.y
  };
  APP_LOG(APP_LOG_LEVEL_DEBUG, "~ ~ ~ THINGY: %d %d", result.x, result.y);
  return result;
}

static void main_window_load(Window *window) {
  // Prepare to calculate layout
  //uint32_t time_layer_size_y = 62;
  uint32_t time_layer_size_y = 60;
  int32_t time_bounds_origin_y_offset = -10;
  Layer *window_layer        = window_get_root_layer(window);
  bounds                     = layer_get_bounds(window_layer);
  bool is_tall               = bounds.size.h > 168;
  center = (GPoint) {
    .x = bounds.size.w / 2,
    .y = bounds.size.h / 2
  };
  radius = center.y;
  GRect primary_bounds       = bounds;
  GRect comp_bounds          = bounds;
  GRect time_bounds          = primary_bounds;
  GRect date_bounds          = comp_bounds;
  GRect countdown_bounds     = comp_bounds;
  GRect connection_bounds    = comp_bounds;
  GRect battery_bounds       = comp_bounds;
  
  // Calculate layout...
  
  primary_bounds.size.h      = (bounds.size.h / 2) + (time_layer_size_y / 2);
  primary_bounds.origin.y    = bounds.size.h - primary_bounds.size.h;
  time_bounds.origin.y       = (primary_bounds.size.h / 2) - (time_layer_size_y / 2) + time_bounds_origin_y_offset; // Includes fudge factor to tweak layout
  //comp_bounds.size.h         = bounds.size.h - primary_bounds.size.h;
  //comp_bounds.size.h         = bounds.size.h - primary_bounds.size.h + PBL_IF_ROUND_ELSE(15,0); // Includes fudge factor
  comp_bounds.size.h         = bounds.size.h - primary_bounds.size.h + ((is_tall) ? 15 : 0); // Includes fudge factor
  //comp_bounds.size.h         = (is_tall) ? bounds.size.h / 2 - 15 : bounds.size.h - primary_bounds.size.h;
  comp_bounds.origin.y       = 0;
  //date_bounds.size.h         = 28;
  date_bounds.size.h         = 32;
  date_bounds.origin.y       = comp_bounds.size.h - date_bounds.size.h;
  countdown_bounds.size.w    = 30;
  countdown_bounds.size.h    = 25;
  countdown_bounds.origin.x  = center.x - (countdown_bounds.size.w / 2);
  countdown_bounds.origin.y  = 0;
  //countdown_bounds.size.h    = PBL_IF_ROUND_ELSE(38, 33);
  //countdown_bounds.size.h    = PBL_IF_ROUND_ELSE(36, 31);
  //countdown_bounds.size.h    = 27;
  //connection_bounds.size.w   = connection_bounds.size.w - 10;
  connection_bounds.size.w   = 30;
  connection_bounds.size.h   = 20;
  connection_bounds.origin.x = 5;
  connection_bounds.origin.y = 0;
  battery_bounds.size.w      = 50;
  //battery_bounds.size.w      = battery_bounds.size.w - 10;
  battery_bounds.size.h      = 20;
  battery_bounds.origin.x    = primary_bounds.size.w - (battery_bounds.size.w + 5);
  battery_bounds.origin.y    = 0;
  
  #if defined(PBL_ROUND)
    //printf("This is a round display!");
    //int32_t min_width = connection_bounds.size.w + connection_bounds.size.w + battery_bounds.size.w;
    //printf("Min width: %d", min_width);
    //int32_t min_width = 100;
    //int32_t min_width = 25;
    // TODO: dynamically calculate optimal angle / width / placement for complications - 2016-1118
    int32_t angle = TRIG_MAX_ANGLE / 10;
    GPoint round_upper_left_point  = calc_round_point(TRIG_MAX_ANGLE - angle);
    GPoint round_upper_right_point = calc_round_point(angle);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "left:  %d %d", round_upper_left_point.x,  round_upper_left_point.y);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "right: %d %d", round_upper_right_point.x, round_upper_right_point.y);
    printf("DISTANCE %d", round_upper_right_point.x - round_upper_left_point.x);
    
    //countdown_bounds.size.h    = 32;
    countdown_bounds.origin.y  = round_upper_right_point.y - countdown_bounds.size.h + battery_bounds.size.h;
    //connection_bounds.size.w   = connection_bounds.size.w - 10;
    //connection_bounds.size.h   = 23;
    connection_bounds.origin.x = round_upper_left_point.x;
    connection_bounds.origin.y = round_upper_right_point.y;
    //battery_bounds.size.w      = round_upper_right_point.x;
    //battery_bounds.size.h      = 23;
    battery_bounds.origin.y    = round_upper_right_point.y;
    battery_bounds.origin.x    = round_upper_right_point.x - (battery_bounds.size.w);
  #endif
  
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
  text_layer_set_background_color(s_primary_layer, GColorClear);
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
  
  // Text layers
  s_time_layer       = text_layer_create(time_bounds);
  s_date_layer       = text_layer_create(date_bounds);
  s_countdown_layer  = text_layer_create(countdown_bounds);
  s_connection_layer = text_layer_create(connection_bounds);
  s_battery_layer    = text_layer_create(battery_bounds);
  
  configure_text_layer(s_time_layer,       GTextAlignmentCenter, GColorClear, s_primary_layer);
  configure_text_layer(s_date_layer,       GTextAlignmentCenter, GColorClear, s_complications_layer);
  configure_text_layer(s_countdown_layer,  GTextAlignmentCenter, GColorClear, s_complications_layer);
  configure_text_layer(s_connection_layer, GTextAlignmentLeft,   GColorClear, s_complications_layer);
  configure_text_layer(s_battery_layer,    GTextAlignmentRight,  GColorClear, s_complications_layer);
  
  // Things which may change on future config updates
  update_display_config();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_countdown_layer);
  text_layer_destroy(s_connection_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_complications_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_primary_layer);
  unload_all_fonts();
}

void init_display(Window *new_s_main_window) {
  s_main_window = new_s_main_window;
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
}


/* ====  Refresh / update  ========================================================= */

void refresh_display_layout(void) {
  layer_set_hidden((Layer *)s_countdown_layer,  !config.alerts_enabled);
  layer_set_hidden((Layer *)s_connection_layer, !config.show_connection_status);
  layer_set_hidden((Layer *)s_battery_layer,    !config.show_battery_status);
  if (!is_alert_currently_active()) {
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
