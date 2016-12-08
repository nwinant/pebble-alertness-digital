#pragma once
#include <pebble.h>


/* ====  Structs  ================================================================== */

#define DEFAULT_CONFIG_BUFF_MAX_LEN 32

typedef struct AlertConfiguration {
  char               vibe_name    [DEFAULT_CONFIG_BUFF_MAX_LEN];
  int                duration_sec;
  int                repeat_delay_ms;
  uint8_t            frequency_mins;
  uint8_t            start_hour;
  uint8_t            end_hour;
} AlertConfiguration;

typedef struct Configuration {
  bool               alerts_enabled;
  bool               alerts_use_quiet_time;
  bool               alert_dismiss_on_tap;
  AlertConfiguration alert1;
  GColor             main_bg_color;
  GColor             main_fg_color;
  GColor             comps_bg_color;
  GColor             comps_fg_color;
  GColor             alert_bg_color;
  GColor             alert_fg_color;
  char               time_font_name     [DEFAULT_CONFIG_BUFF_MAX_LEN];
  char               date_font_name     [DEFAULT_CONFIG_BUFF_MAX_LEN];
  char               countdown_font_name[DEFAULT_CONFIG_BUFF_MAX_LEN];
  char               details_font_name  [DEFAULT_CONFIG_BUFF_MAX_LEN];
  char               date_format        [DEFAULT_CONFIG_BUFF_MAX_LEN];
  bool               show_connection_status;
  bool               show_battery_status;
  int                double_tap_min_ms;
  int                double_tap_max_ms;
  bool               is_silence_required;
} Configuration;


/* ====  Variables  ================================================================ */

extern Configuration config;


/* ====  Functions  ================================================================ */

void load_config(void);

void update_config(DictionaryIterator *iter, void *context);
