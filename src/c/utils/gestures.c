#include <pebble.h>
#include "gestures.h"
#include "inttypes.h"


/* ====  Variables  ================================================================ */

static int     min_ms           = 0;
static int     max_ms           = 0;
static bool    silence_required = false;
static int32_t last_tap_ms      = 0;
void (*callback)(AccelAxisType, int32_t);


/* ====  Private functions  ========================================================= */

static int32_t get_current_ms(void) {
  time_t sec1;
  uint16_t ms1;
  time_ms(&sec1, &ms1);
  return (int32_t)1000*(int32_t)sec1 + (int32_t)ms1;
}

static void reset_counter(void) {
  last_tap_ms = 0;
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  int32_t curr_ms  = get_current_ms();
  int32_t elapsed_ms = curr_ms - last_tap_ms;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Tap? %" PRIu32, elapsed_ms);
  if (last_tap_ms == 0) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "  ... First tap");
    last_tap_ms = curr_ms;
  } else if (elapsed_ms < min_ms) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "  ... Too soon!");
    if (silence_required) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "  ... Resetting");
      last_tap_ms = curr_ms;
    }
  } else if (elapsed_ms > max_ms) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "  ... Too late!");
    last_tap_ms = curr_ms;
  } else {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Double-tap!");
    callback(axis, direction);
    reset_counter();
  }
}


/* ====  Public functions  ========================================================== */

/*
"Good question! There isn't an API to modify the sensitivity of the tap.

"You could, however, create something similar to the tap service using 
accel_data_service_subscribe() and looking at either the total force 
along all the axes, or possibly just the Z-axis."

https://developer.pebble.com/guides/events-and-services/accelerometer/
*/

// http://stackoverflow.com/questions/9410/how-do-you-pass-a-function-as-a-parameter-in-c
// http://www.cprogramming.com/tutorial/function-pointers.html
void double_tap_service_subscribe(
  int min_delay_ms, 
  int max_delay_ms,
  bool is_silence_required,
  void (*callback_func)(AccelAxisType, int32_t)
) {
  min_ms  = min_delay_ms;
  max_ms  = max_delay_ms;
  silence_required = is_silence_required;
  callback = callback_func;
  reset_counter();
  accel_tap_service_subscribe(accel_tap_handler);
}

void double_tap_service_unsubscribe(void) {
  accel_tap_service_unsubscribe();
}
