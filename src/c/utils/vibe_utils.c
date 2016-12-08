#include <pebble.h>
#include "vibe_utils.h"


/* ====  Variables  ================================================================ */

static AppTimer    *vibe_timer         = NULL;
static VibePattern *vibe_pattern       = NULL;
static uint32_t     vibe_wait_duration;
static int          max_vibe_count;
static int          curr_vibe_count;


/* ====  Private functions  ======================================================== */

void duration_repeating_vibe_callback(void *callback_data) {
  vibes_cancel();
  curr_vibe_count++;
  vibes_enqueue_custom_pattern(*vibe_pattern);
  vibe_timer = NULL;
  if (curr_vibe_count < max_vibe_count) {
    vibe_timer = app_timer_register(vibe_wait_duration, duration_repeating_vibe_callback, NULL);
  }
}


/* ====  External functions  ======================================================== */

uint32_t calc_vibe_duration(VibePattern *pattern) {
  int result = 0;
  for (uint32_t i = 0; i < pattern->num_segments; i++) {
    result += pattern->durations[i];
  }
  return result;
}

void cancel_scheduled_vibes(void) {
  if (vibe_timer != NULL) {
    app_timer_cancel(vibe_timer);
    vibe_timer = NULL;
  }
}

void cancel_vibes_and_scheduled_vibes(void) {
  cancel_scheduled_vibes();
  vibes_cancel();
}

void repeat_vibe_for_duration(VibePattern *pattern, int duration, int delay_between_repeats) {
  cancel_scheduled_vibes();
  vibe_pattern       = pattern;
  vibe_wait_duration = calc_vibe_duration(vibe_pattern) + delay_between_repeats;
  max_vibe_count     = (duration + delay_between_repeats) / vibe_wait_duration;
  curr_vibe_count    = 0;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "max_vibe_count: %d, curr_vibe_count: %d", max_vibe_count, curr_vibe_count);
  duration_repeating_vibe_callback(NULL);
}