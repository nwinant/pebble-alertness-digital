#pragma once
#include <pebble.h>


/* ====  Structs  ================================================================== */

typedef struct Configuration {
  bool        invert_layout;
  GColor      main_bg_color;
  GColor      main_fg_color;
  GColor      comps_bg_color;
  GColor      comps_fg_color;
  GColor      alert_bg_color;
  GColor      alert_fg_color;
  bool        alerts_enabled;
  uint8_t     alert_frequency_mins;
  uint8_t     alert_start_hour;
  uint8_t     alert_end_hour;
  char        *date_format;
  bool        show_connection_status;
  bool        show_battery_status;
  VibePattern alert_vibe_pattern;
} Configuration;


/* ====  Variables  ================================================================ */

extern Configuration config;


/* ====  Functions  ================================================================ */

void load_config(void);

void update_config(DictionaryIterator *iter, void *context);
