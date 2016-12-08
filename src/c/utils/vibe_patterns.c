#include <pebble.h>
#include "vibe_patterns.h"

/*
static const uint32_t alert_segments1[] = { 200, 100, 200, 500,
                                    1000, 500,
                                    200, 100, 200 };
static const uint32_t alert_segments_long[] = { 100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100 };
static uint32_t alert_segments_very_long[] ={ 100, 200, 100, 400,
                                                 500, 200, 500, 200,
                                    200, 100, 200, 100,
                                    500, 200, 500, 200,
                                    100, 200, 100 };
*/

/* ====  Structs  ================================================================== */

typedef struct NamedVibePattern {
  char           name[32];
  VibePattern    pattern;
} NamedVibePattern;


/* ====  Variables  ================================================================ */

static uint32_t alert_segments_short[] = { 400, 200 };
static uint32_t alert_segments_very_long[] = { 100, 200, 100, 400,
                                    800, 200,
                                    200, 200,
                                    800, 200,
                                    200, 200,
                                    //800, 200,
                                    800, 400,
                                    100, 200, 100 };
static uint32_t alert_segments_one_sec[] = { 300, 200, 300, 200 };

/* First pattern is default pattern */
static NamedVibePattern named_pattern[3] = {
  {
    "short",
    {
      .durations = alert_segments_short,
      .num_segments = ARRAY_LENGTH(alert_segments_short)
    }
  },
  {
    "very_long",
    {
      .durations = alert_segments_very_long,
      .num_segments = ARRAY_LENGTH(alert_segments_very_long)
    }
  },
  {
    "one_sec", 
    {
      .durations = alert_segments_one_sec,
      .num_segments = ARRAY_LENGTH(alert_segments_one_sec)
    }
  }
};


/* ====  Private functions  ======================================================== */

NamedVibePattern *get_named_pattern_by_name(const char *pattern_name) {
  NamedVibePattern *curr_pattern    = named_pattern;
  NamedVibePattern *last_pattern    = named_pattern + sizeof(named_pattern)/sizeof(curr_pattern[0]);
  while (curr_pattern < last_pattern) {
    if (strcmp(pattern_name, curr_pattern->name) == 0) {
      return curr_pattern;
    }
    curr_pattern++;
  }
  return NULL;
}


/* ====  External functions  ======================================================== */

VibePattern *get_vibe_pattern_by_name(const char *pattern_name) {
  NamedVibePattern *selected_pattern = get_named_pattern_by_name(pattern_name);
  if (selected_pattern) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Retrieved vibe pattern '%s'", pattern_name);
    return &selected_pattern->pattern;
  }
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Couldn't find vibe pattern '%s', using default", pattern_name);
  return &named_pattern->pattern;
}
