#pragma once
#include <pebble.h>

void double_tap_service_subscribe(
  int min_delay_ms,
  int max_delay_ms, 
  bool is_silence_required,
  void (*callback_func)(AccelAxisType, int32_t)
);

void double_tap_service_unsubscribe(void);
