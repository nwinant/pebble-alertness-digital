#include <pebble.h>
#include "../utils/vibe_patterns.h"
#include "../utils/persist_utils.h"
#include "configuration.h"
#include "inttypes.h"

/*
 *  TODO: Ideally, this will be replaced (in whole or in part) with Enamel:
 *        https://github.com/gregoiresage/enamel   - nwinant, 2016-11-13
 *
 */

/* ====  Variables  ================================================================ */

static const AlertConfiguration default_alert = {
  .vibe_name        = "one_sec",
  .duration_sec     = 58,
  .repeat_delay_ms  = 100,
  .frequency_mins   = 15,
  .start_hour       = 9,
  .end_hour         = 22
  //.frequency_mins   = 1,
  //.start_hour       = 0,
  //.end_hour         = 24
};

static Configuration      defaults;
static bool               initialized = false;

Configuration config;


/* ====  Private functions  ========================================================= */

AlertConfiguration load_alert_config(void) {
  AlertConfiguration result = {
    .duration_sec    = default_alert.duration_sec,    // FIXME: make configurable!
    .repeat_delay_ms = default_alert.repeat_delay_ms, // FIXME: make configurable!
    .frequency_mins  = load_int(MESSAGE_KEY_AlertFrequency, default_alert.frequency_mins),
    .start_hour      = load_int(MESSAGE_KEY_AlertStartHour, default_alert.start_hour),
    .end_hour        = load_int(MESSAGE_KEY_AlertEndHour,   default_alert.end_hour)
  };
  // String defaults which are not yet user-configurable
  strncpy(result.vibe_name, default_alert.vibe_name, sizeof(default_alert.vibe_name));
  return result;
}

void load_defaults(void) {
  if (!initialized) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading defaults");
    defaults = (Configuration) {
      .alerts_enabled         = 1,
      .alerts_use_quiet_time  = true,
      .alert_dismiss_on_tap   = true,
      .alert1                 = default_alert,
      .main_bg_color          = GColorBlack,
      .main_fg_color          = GColorWhite,
      .comps_bg_color         = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite),
      .comps_fg_color         = PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack),
      .alert_bg_color         = PBL_IF_COLOR_ELSE(GColorFromHEX(0x550000), GColorWhite),
      .alert_fg_color         = PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack),
      .show_connection_status = 1,
      .show_battery_status    = 1,
      .time_font_name         = "time_comfortaa",
      .date_font_name         = "date_gothic",
      .countdown_font_name    = "countdown_gothic",
      .details_font_name      = "details_gothic",
      .date_format            = "%a %m-%d",
      .double_tap_min_ms      = 500,
      .double_tap_max_ms      = 3000,
      .is_silence_required    = false
    };
    initialized = true;
  }
}


/* ====  External functions  ======================================================== */

void load_config(void) {
  load_defaults();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading config");
  Configuration result = {
    // Alerts
    .alerts_use_quiet_time = defaults.alerts_use_quiet_time, // FIXME: make config-driven
    .alerts_enabled        = load_bool(MESSAGE_KEY_AlertsEnabled, defaults.alerts_enabled),
    .alert_dismiss_on_tap  = defaults.alert_dismiss_on_tap, // FIXME: make configurable!
    .alert1                = load_alert_config(),
    
    // Display
    .main_bg_color  = load_color(MESSAGE_KEY_MainBackgroundColor,          defaults.main_bg_color),
    .main_fg_color  = load_color(MESSAGE_KEY_MainForegroundColor,          defaults.main_fg_color),
    .comps_bg_color = load_color(MESSAGE_KEY_ComplicationsBackgroundColor, defaults.comps_bg_color),
    .comps_fg_color = load_color(MESSAGE_KEY_ComplicationsForegroundColor, defaults.comps_fg_color),
    .alert_bg_color = load_color(MESSAGE_KEY_AlertBackgroundColor,         defaults.alert_bg_color),
    .alert_fg_color = load_color(MESSAGE_KEY_AlertForegroundColor,         defaults.alert_fg_color),
    
    // Misc
    .show_connection_status = load_bool(MESSAGE_KEY_ShowConnectionStatus, defaults.show_connection_status),
    .show_battery_status    = load_bool(MESSAGE_KEY_ShowBatteryStatus,    defaults.show_battery_status),
    .double_tap_min_ms = defaults.double_tap_min_ms,    // FIXME: make configurable!
    .double_tap_max_ms = defaults.double_tap_max_ms,    // FIXME: make configurable!
    .is_silence_required = defaults.is_silence_required // FIXME: make configurable!
  };
  
  // Load String-based properties
  load_string(MESSAGE_KEY_TimeFont,   result.time_font_name, defaults.time_font_name, DEFAULT_CONFIG_BUFF_MAX_LEN);
  load_string(MESSAGE_KEY_DateFormat, result.date_format,    defaults.date_format,    DEFAULT_CONFIG_BUFF_MAX_LEN);
  
  // String defaults which are not yet user-configurable
  strncpy(result.date_font_name,      defaults.date_font_name,      sizeof(defaults.date_font_name));
  strncpy(result.countdown_font_name, defaults.countdown_font_name, sizeof(defaults.countdown_font_name));
  strncpy(result.details_font_name,   defaults.details_font_name,   sizeof(defaults.details_font_name));
  
  // Aaaaaaaaand replace the global config:
  config = result;
}

void update_config(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating config");
  
  // Display
  persist_color_update(MESSAGE_KEY_MainBackgroundColor,          iter);
  persist_color_update(MESSAGE_KEY_MainForegroundColor,          iter);
  persist_color_update(MESSAGE_KEY_ComplicationsBackgroundColor, iter);
  persist_color_update(MESSAGE_KEY_ComplicationsForegroundColor, iter);
  persist_color_update(MESSAGE_KEY_AlertBackgroundColor,         iter);
  persist_color_update(MESSAGE_KEY_AlertForegroundColor,         iter);
  
  // Alert prefs
  persist_bool_update(MESSAGE_KEY_AlertsEnabled, iter);
  //alerts_use_quiet_time   // FIXME: make configurable!
  //.alert_vibe_name        // FIXME: make configurable!
  //.alert_duration_sec     // FIXME: make configurable!
  //.alert_repeat_delay_ms  // FIXME: make configurable!
  persist_int_from_string_update(MESSAGE_KEY_AlertFrequency,   iter);
  persist_string_update         (MESSAGE_KEY_AlertVibePattern, iter);
  if (persist_int_from_string_update(MESSAGE_KEY_AlertStartHour, iter)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- AlertStartHour: %" PRIu32, persist_read_int(MESSAGE_KEY_AlertStartHour));
  }
  if (persist_int_from_string_update(MESSAGE_KEY_AlertEndHour, iter)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "- AlertEndHour: %" PRIu32, persist_read_int(MESSAGE_KEY_AlertEndHour));
  }
  //.alert_dismiss_on_tap  // TODO: make configurable! - 2016-12-01
  persist_string_update(MESSAGE_KEY_TimeFont, iter);
  
  // Misc
  //.date_font_name       // FIXME: make configurable!
  //.countdown_font_name  // FIXME: make configurable!
  //.details_font_name    // FIXME: make configurable!
  persist_string_update(MESSAGE_KEY_DateFormat,           iter);
  persist_bool_update  (MESSAGE_KEY_ShowConnectionStatus, iter);
  persist_bool_update  (MESSAGE_KEY_ShowBatteryStatus,    iter);
  //.double_tap_min_ms = defaults.double_tap_min_ms, // FIXME: make configurable!
  //.double_tap_max_ms = defaults.double_tap_max_ms  // FIXME: make configurable!
  //.is_silence_required = defaults.is_silence_required // FIXME: make configurable!
  
  // Now, refresh the actual config...
  load_config();
}