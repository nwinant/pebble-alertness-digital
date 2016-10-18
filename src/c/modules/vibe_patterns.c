#include <pebble.h>
#include "vibe_patterns.h"


// CONST



// Structs



// Constants

/*
static const uint32_t alert_segments1[] = { 200, 100, 200, 500,
                                    1000, 500,
                                    200, 100, 200 };
*/
/*
static const uint32_t alert_segments_long[] = { 100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100, 400,
                                    800, 400,
                                    100, 200, 100 };
*/
/*
static const uint32_t alert_segments_extended[] = { 100, 200, 100, 400,
                                    800, 200,
                                    200, 200,
                                    800, 200,
                                    200, 200,
                                    //800, 200,
                                    800, 400,
                                    100, 200, 100 };
                                    
static uint32_t alert_segments_short[] = { 400, 200 };
static uint32_t alert_segments_very_long[] ={ 100, 200, 100, 400,
                                                 500, 200, 500, 200,
                                    200, 100, 200, 100,
                                    500, 200, 500, 200,
                                    100, 200, 100 };
*/




// http://stackoverflow.com/questions/12031302/convert-from-long-long-to-int-and-the-other-way-back-in-c

static uint32_t alert_segments_short[] = { 400, 200 };
static uint32_t alert_segments_very_long[] = { 100, 200, 100, 400,
                                    800, 200,
                                    200, 200,
                                    800, 200,
                                    200, 200,
                                    //800, 200,
                                    800, 400,
                                    100, 200, 100 };

typedef struct { char *key; uint32_t *val; } t_symstruct;

static t_symstruct lookuptable[] = {
  { "short", alert_segments_short },
  { "very_long", alert_segments_very_long },
  { "also", alert_segments_short }
};

#define NKEYS (int) (sizeof(lookuptable)/sizeof(t_symstruct))

VibePattern get_vibe_pattern_by_string(char *pattern_name) {
  // FIXME: remove
  if (1 == 1) {
  return (VibePattern) {
    .durations = alert_segments_very_long,
    .num_segments = ARRAY_LENGTH(alert_segments_very_long)
  };
  } else {
    
    // is the problem char key of variable length?
    
    
    
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibe pattern selected: %s", pattern_name);
  int i;
  
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "NKEYS: %d", NKEYS);
  
  for (i=0; i < NKEYS; i++) {
    t_symstruct *sym = lookuptable + i*sizeof(t_symstruct);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "? Trying %d", i);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Hmmmm... [%s]", sym->key);
    if (strcmp(sym->key, pattern_name) == 0) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Success?");
      return (VibePattern) {
        .durations = sym->val,
        .num_segments = ARRAY_LENGTH(sym->val)
      };
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(... Nope!)");
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "NOOOOOOOOOO!");
  // FIXME: What should this be?
  return (VibePattern) {
    .durations = alert_segments_very_long,
    .num_segments = ARRAY_LENGTH(alert_segments_very_long)
  };
  }
}



// Getters
/*
uint32_t *get_vibe_pattern_by_string(char *pattern_name) {
  if (strcmp("very_long", pattern_name)) {
    
  } else {
  return alert_segments_very_long;
  }
}
*/