#include <pebble.h>
#include "fonts.h"
#include "inttypes.h"


/* ====  Structs  ================================================================== */

//#define MISSING_FONT 0

#define FONT_NAME_MAX_LEN 32
typedef struct CustomFontStruct { 
  char        name[FONT_NAME_MAX_LEN];
  uint32_t    resource_id;
  GFont       instance;
} CustomFontStruct;

typedef struct SystemFontStruct { 
  char        name[FONT_NAME_MAX_LEN];
  char        *font_key;
} SystemFontStruct;


/* ====  Variables  ================================================================ */

/*
http://www.dafont.com/comfortaa.font
*/
static CustomFontStruct custom_font[2] = {
  {"time_comfortaa",        RESOURCE_ID_FONT_COMFORTAA_BOLD_SUBSET_62, NULL},
  {"time_dinen",            RESOURCE_ID_FONT_DINEN_SUBSET_62,          NULL}
};

static SystemFontStruct system_font[5] = {
  {"time_bitham",           FONT_KEY_BITHAM_42_BOLD},
  {"time_roboto",           FONT_KEY_ROBOTO_BOLD_SUBSET_49},
  {"date_gothic",           FONT_KEY_GOTHIC_28_BOLD},
  //{"date_gothic_med",     FONT_KEY_GOTHIC_24_BOLD},
  //{"countdown_bitham",    FONT_KEY_BITHAM_30_BLACK},
  //{"countdown_gothic_lg", FONT_KEY_GOTHIC_28},
  {"countdown_gothic",      FONT_KEY_GOTHIC_24_BOLD},
  {"details_gothic",        FONT_KEY_GOTHIC_18_BOLD}
};


/* ====  Private functions  ======================================================== */

void log_font_struct(char *action, CustomFontStruct *font) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s font: '%s' %" PRIu32 " %i", 
            action, font->name, font->resource_id, (font->instance != NULL));
}

void possibly_load_custom_font_struct(CustomFontStruct *font) {
  if (font->instance == NULL) {
    font->instance = fonts_load_custom_font(resource_get_handle(font->resource_id));
    log_font_struct("Loaded", font);
  }
}

void possibly_unload_custom_font_struct(CustomFontStruct *font) {
  if (font->instance != NULL) {
    fonts_unload_custom_font(font->instance);
    font->instance = NULL;
    log_font_struct("Unloaded", font);
  }
}

CustomFontStruct *get_custom_font_struct_by_name(char *font_name) {
  CustomFontStruct *curr_font = custom_font;
  CustomFontStruct *last_font = custom_font + sizeof(custom_font)/sizeof(custom_font[0]);
  while (curr_font < last_font) {    
    if (strcmp(font_name, curr_font->name) == 0) {
      return curr_font;
    }
    curr_font++;    
  }
  return NULL;
}

SystemFontStruct *get_system_font_struct_by_name(char *font_name) {
  SystemFontStruct *curr_font = system_font;
  SystemFontStruct *last_font = system_font + sizeof(system_font)/sizeof(system_font[0]);
  while (curr_font < last_font) {    
    if (strcmp(font_name, curr_font->name) == 0) {
      return curr_font;
    }
    curr_font++;    
  }
  return NULL;
}


/* ====  External functions  ======================================================== */

GFont get_font_by_name(char *font_name) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Trying to get font: '%s'", font_name);
  CustomFontStruct *cust_font = get_custom_font_struct_by_name(font_name);
  if (cust_font != NULL) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Retrieving custom font: '%s'", font_name);
    possibly_load_custom_font_struct(cust_font);
    return cust_font->instance;
  }
  SystemFontStruct *sys_font = get_system_font_struct_by_name(font_name);
  if (sys_font != NULL) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Retrieving system font: '%s'", font_name);
    return fonts_get_system_font(sys_font->font_key);
  }
  return NULL;
}

void unload_all_fonts(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Unloading all fonts...");
  CustomFontStruct *curr_font = custom_font;
  CustomFontStruct *last_font = custom_font + sizeof(custom_font)/sizeof(custom_font[0]);
  while (curr_font < last_font) {
    possibly_unload_custom_font_struct(curr_font);
    curr_font++;    
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "... All fonts unloaded");
}