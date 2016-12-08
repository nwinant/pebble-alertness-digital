#include <pebble.h>
#include "../utils/vibe_patterns.h"
#include "../utils/vibe_utils.h"
#include "configuration.h"
#include "alert_handler.h"
#include "inttypes.h"


/* ====  Debugging switches  ======================================================= */

#define DEV_EXCESSIVE_LOGGING 0
#define DEV_ALERT_AMNESIA 0


/* ====  Variables  ================================================================ */

static bool     alert_timer_running;
static uint8_t  alert_interval_remainder;
static int32_t  curr_time;
static int32_t  last_alert_time;


/* ====  Private functions  ========================================================= */

bool is_quiet_time_in_effect(void) {
  return config.alerts_use_quiet_time && quiet_time_is_active();
}

bool is_alert_potentially_active(void) {
  return config.alerts_enabled
    && is_alert_timer_running() 
    && (get_alert_interval_remainder() == 0)
    && !is_quiet_time_in_effect();
}


/* ====  External functions  ======================================================== */

bool is_alert_timer_running(void) {
  return alert_timer_running && !is_quiet_time_in_effect();
}

uint8_t get_alert_interval_remainder(void) {
  return alert_interval_remainder;
}

bool is_alert_currently_active(void) {
  return is_alert_potentially_active()
    && ((curr_time != last_alert_time) || DEV_ALERT_AMNESIA);
}

void update_alert_handler(struct tm *tick_time) {
  uint8_t curr_hour        = tick_time->tm_hour;
  uint8_t curr_min         = tick_time->tm_min;
  alert_interval_remainder = curr_min % config.alert1.frequency_mins;
  alert_timer_running      = 
    ((curr_hour >= config.alert1.start_hour)
     || ((config.alert1.start_hour - curr_hour == 1)
         && (60 - curr_min <= config.alert1.frequency_mins)))
    &&
    ((curr_hour < config.alert1.end_hour)
     || ((curr_hour = config.alert1.end_hour)
         && (curr_min == 0)));
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Is quiet time in effect? %i", is_quiet_time_in_effect());
  if (is_alert_potentially_active()) {
    cancel_vibes_and_scheduled_vibes();
    curr_time       = (tick_time->tm_hour * 10000) + (tick_time->tm_min * 100);
    last_alert_time = (persist_exists(MESSAGE_KEY_LastAlertTickTime)) ? persist_read_int(MESSAGE_KEY_LastAlertTickTime) : 0;
    if (DEV_EXCESSIVE_LOGGING) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Last alert: %" PRIi32, last_alert_time);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibe amnesia? %i", DEV_ALERT_AMNESIA);
    }
    if  (is_alert_currently_active()) {
      persist_write_int(MESSAGE_KEY_LastAlertTickTime, curr_time);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Alert: %" PRIi32, curr_time);
      repeat_vibe_for_duration(
        get_vibe_pattern_by_name(config.alert1.vibe_name),
        config.alert1.duration_sec * 1000,
        config.alert1.repeat_delay_ms);
    }
  }
}

void dismiss_current_alert(void) {
  cancel_vibes_and_scheduled_vibes();
}