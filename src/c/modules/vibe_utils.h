#pragma once
#include <pebble.h>

void cancel_scheduled_vibes(void);

void cancel_vibes_and_scheduled_vibes(void);

void repeat_vibe_for_duration(VibePattern *pattern, int duration, int delay_between_repeats);

uint32_t calc_vibe_duration(VibePattern *pattern);
