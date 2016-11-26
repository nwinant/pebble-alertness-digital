#pragma once
#include <pebble.h>


/* ====  Structs  ================================================================== */

#define DEFAULT_CONFIG_BUFF_MAX_LEN 32

typedef struct Configuration {
  bool        invert_layout;
  GColor      main_bg_color;
  GColor      main_fg_color;
  GColor      comps_bg_color;
  GColor      comps_fg_color;
  GColor      alert_bg_color;
  GColor      alert_fg_color;
  bool        alerts_enabled;
  bool        alerts_use_quiet_time;
  char        alert_vibe_name    [DEFAULT_CONFIG_BUFF_MAX_LEN];
  uint8_t     alert_frequency_mins;
  uint8_t     alert_start_hour;
  uint8_t     alert_end_hour;
  char        time_font_name     [DEFAULT_CONFIG_BUFF_MAX_LEN];
  char        date_font_name     [DEFAULT_CONFIG_BUFF_MAX_LEN];
  char        countdown_font_name[DEFAULT_CONFIG_BUFF_MAX_LEN];
  char        details_font_name  [DEFAULT_CONFIG_BUFF_MAX_LEN];
  char        date_format        [DEFAULT_CONFIG_BUFF_MAX_LEN];
  bool        show_connection_status;
  bool        show_battery_status;
  //VibePattern alert_vibe_pattern;
} Configuration;


/* ====  Variables  ================================================================ */

extern Configuration config;


/* ====  Functions  ================================================================ */

void load_config(void);

void update_config(DictionaryIterator *iter, void *context);
