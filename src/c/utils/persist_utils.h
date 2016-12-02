#pragma once
#include <pebble.h>

bool load_bool(const uint32_t key, bool default_value);

int load_int(const uint32_t key, int default_value);

GColor load_color(const uint32_t key, GColor default_value);

void load_string(const uint32_t key, char *dest, const char *default_value, int len);

void persist_bool_update(const uint32_t key, DictionaryIterator *iter);

void persist_int_update(const uint32_t key, DictionaryIterator *iter);

void persist_color_update(const uint32_t key, DictionaryIterator *iter);

void persist_string_update(const uint32_t key, DictionaryIterator *iter);
