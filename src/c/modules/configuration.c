#include <pebble.h>
#include "configuration.h"
#include "vibe_patterns.h"
#include "inttypes.h"

Configuration load_config(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading config");
  

  
  // Build & return Configuration...
  Configuration result = {
    // Display
    .main_bg_color = persist_exists(MESSAGE_KEY_MainBackgroundColor) 
      ? GColorFromHEX(persist_read_int(MESSAGE_KEY_MainBackgroundColor))
      : GColorBlack,
    .main_fg_color = persist_exists(MESSAGE_KEY_MainForegroundColor) 
      ? GColorFromHEX(persist_read_int(MESSAGE_KEY_MainForegroundColor))
      : GColorWhite,
    .comps_bg_color = persist_exists(MESSAGE_KEY_ComplicationsBackgroundColor) 
      ? GColorFromHEX(persist_read_int(MESSAGE_KEY_ComplicationsBackgroundColor))
      : PBL_IF_COLOR_ELSE(GColorRed, GColorWhite),
    .comps_fg_color = persist_exists(MESSAGE_KEY_ComplicationsForegroundColor) 
      ? GColorFromHEX(persist_read_int(MESSAGE_KEY_ComplicationsForegroundColor))
      : GColorBlack,
    .alert_bg_color = persist_exists(MESSAGE_KEY_AlertBackgroundColor) 
      ? GColorFromHEX(persist_read_int(MESSAGE_KEY_AlertBackgroundColor))
      : PBL_IF_COLOR_ELSE(GColorRed, GColorWhite),
    .alert_fg_color = persist_exists(MESSAGE_KEY_AlertForegroundColor) 
      ? GColorFromHEX(persist_read_int(MESSAGE_KEY_AlertForegroundColor))
      : PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack),
    .invert_layout = persist_exists(MESSAGE_KEY_EnableFlip) 
      ? persist_read_bool(MESSAGE_KEY_EnableFlip)
      : 0,
    // Alerts
    .alerts_enabled = persist_exists(MESSAGE_KEY_AlertsEnabled) 
      ? persist_read_bool(MESSAGE_KEY_AlertsEnabled)
      : 1,
    .alert_frequency_mins = persist_exists(MESSAGE_KEY_AlertFrequency) 
      ? persist_read_int(MESSAGE_KEY_AlertFrequency)
      : 15,
    
//    .alert_vibe_pattern = vibe_pattern,
    
    /*
    // FIXME: configure!
    uint32_t *alert_segments;
    if (persist_exists(MESSAGE_KEY_AlertVibePattern)) {
      char buffer[32];
      persist_read_string(MESSAGE_KEY_AlertVibePattern, buffer, sizeof(buffer));
      alert_segments = get_vibe_pattern_by_string(buffer);
    } else {
      alert_segments = get_vibe_pattern_by_string("very_long");
    }
    //uint32_t *alert_segments = get_vibe_pattern_by_string("short");
    */
    /*
    .alert_vibe_pattern = (VibePattern) {
      .durations = alert_segments,
      .num_segments = ARRAY_LENGTH(alert_segments)
    },
    */
    /*
    // FIXME: !!!!!!
    .alert_vibe_pattern = (VibePattern) {
      .durations = alert_segments_very_long,
      .num_segments = ARRAY_LENGTH(alert_segments_very_long)
    },
    */
    
    .alert_start_hour = persist_exists(MESSAGE_KEY_AlertStartHour) 
      ? persist_read_int(MESSAGE_KEY_AlertStartHour)
      : 9,
    .alert_end_hour = persist_exists(MESSAGE_KEY_AlertEndHour) 
      ? persist_read_int(MESSAGE_KEY_AlertEndHour)
      : 22,
    // Misc
    // FIXME: configurize
    //.date_format = "%a %b %e",
    .date_format = "%a %Y-%m-%d",
    
    .show_connection_status = persist_exists(MESSAGE_KEY_ShowConnectionStatus) 
      ? persist_read_bool(MESSAGE_KEY_ShowConnectionStatus)
      : 1,
    .show_battery_status = persist_exists(MESSAGE_KEY_ShowBatteryStatus) 
      ? persist_read_bool(MESSAGE_KEY_ShowBatteryStatus)
      : 1
  };
  
  
  
  
  
    // Post-processing...
  //   FIXME:  http://stackoverflow.com/questions/17250480/c-declaring-int-array-inside-struct
  if (persist_exists(MESSAGE_KEY_AlertVibePattern)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "User vibe!");
    char buffer[32];
    persist_read_string(MESSAGE_KEY_AlertVibePattern, buffer, sizeof(buffer));
    result.alert_vibe_pattern = get_vibe_pattern_by_string(buffer);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "... Received user vibe: %s", buffer);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Default vibe!");
    //result.alert_vibe_pattern = get_vibe_pattern_by_string("short");
    result.alert_vibe_pattern = get_vibe_pattern_by_string("very_long");
    APP_LOG(APP_LOG_LEVEL_DEBUG, "... Using default vibe.");
  }
  
  return result;
}

Configuration update_config(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating config");
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
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- AlertBackgroundColor: %" PRIu32, persist_read_int(MESSAGE_KEY_AlertBackgroundColor));
  }
  Tuple *alert_fg_color_t = dict_find(iter, MESSAGE_KEY_AlertForegroundColor);
  if (alert_fg_color_t) {
    persist_write_int(MESSAGE_KEY_AlertForegroundColor, alert_fg_color_t->value->int32);
  }
  Tuple *invert_layout_t = dict_find(iter, MESSAGE_KEY_EnableFlip);
  if (invert_layout_t) {
    persist_write_bool(MESSAGE_KEY_EnableFlip, invert_layout_t->value->int32 == 1);
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
  Tuple *alert_frequency_vibe_pattern_t = dict_find(iter, MESSAGE_KEY_AlertVibePattern);
  if (alert_frequency_vibe_pattern_t) {
    persist_write_string(MESSAGE_KEY_AlertVibePattern, alert_frequency_vibe_pattern_t->value->cstring);
  }
  Tuple *alert_start_hour_t = dict_find(iter, MESSAGE_KEY_AlertStartHour);
  if (alert_start_hour_t) {
    //persist_write_int(MESSAGE_KEY_AlertStartHour, alert_start_hour_t->value->int32);
    // FIXME: needs more rigorous error-checking...
    persist_write_int(MESSAGE_KEY_AlertStartHour, 
                      atoi(alert_start_hour_t->value->cstring));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- AlertStartHour: %" PRIu32, persist_read_int(MESSAGE_KEY_AlertStartHour));
  }
  Tuple *alert_end_hour_t = dict_find(iter, MESSAGE_KEY_AlertEndHour);
  if (alert_end_hour_t) {
    //persist_write_int(MESSAGE_KEY_AlertEndHour, alert_end_hour_t->value->int32);
    // FIXME: needs more rigorous error-checking...
    persist_write_int(MESSAGE_KEY_AlertEndHour, 
                      atoi(alert_end_hour_t->value->cstring));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- AlertEndHour: %" PRIu32, persist_read_int(MESSAGE_KEY_AlertEndHour));
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
  
  return load_config();
}