#pragma once
#include <pebble.h>

bool load_bool(const uint32_t key, bool default_value);

int load_int(const uint32_t key, int default_value);

GColor load_color(const uint32_t key, GColor default_value);

void load_string(const uint32_t key, char *dest, const char *default_value, int dest_length);

bool persist_bool_update(const uint32_t key, DictionaryIterator *iter);

bool persist_int_update(const uint32_t key, DictionaryIterator *iter);

bool persist_color_update(const uint32_t key, DictionaryIterator *iter);

bool persist_string_update(const uint32_t key, DictionaryIterator *iter);

bool persist_int_from_string_update(const uint32_t key, DictionaryIterator *iter);
