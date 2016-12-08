#pragma once
#include <pebble.h>

/*
 AccelTapHandler:
 https://developer.pebble.com/docs/c/Foundation/Event_Service/AccelerometerService/#AccelTapHandler
 */
void double_tap_service_subscribe(
  int             min_delay_ms,
  int             max_delay_ms, 
  bool            is_silence_required,
  AccelTapHandler callback_func
);

void double_tap_service_unsubscribe(void);
