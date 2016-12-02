#include <pebble.h>
#include "persist_utils.h"

bool load_bool(const uint32_t key, bool default_value) {
  return persist_exists(key) ? persist_read_bool(key) : default_value;
}

int load_int(const uint32_t key, int default_value) {
  return persist_exists(key) ? persist_read_int(key) : default_value;
}

GColor load_color(const uint32_t key, GColor default_value) {
  return persist_exists(key) ? GColorFromHEX(persist_read_int(key)) : default_value;
}

void load_string(const uint32_t key, char *dest, const char *default_value, int len) {
  if (persist_exists(key)) {
    persist_read_string(key, dest, sizeof(dest));
  } else {
    strncpy(dest, default_value, len);
    //dest[sizeof(len) - 1] = '\0';
  }
}

void persist_bool_update(const uint32_t key, DictionaryIterator *iter) {
  Tuple *bool_t = dict_find(iter, key);
  if (bool_t) {
    persist_write_bool(key, bool_t->value->int32 == 1);
  }
}

void persist_int_update(const uint32_t key, DictionaryIterator *iter) {
  Tuple *int_t = dict_find(iter, key);
  if (int_t) {
    persist_write_int(key, int_t->value->int32);
  }
}

void persist_color_update(const uint32_t key, DictionaryIterator *iter) {
  Tuple *color_t = dict_find(iter, key);
  if (color_t) {
    persist_write_int(key, color_t->value->int32);
  }
}

void persist_string_update(const uint32_t key, DictionaryIterator *iter) {
  Tuple *string_t = dict_find(iter, key);
  if (string_t) {
    persist_write_string(key, string_t->value->cstring);
  }
}
