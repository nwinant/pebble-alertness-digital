#pragma once
#include "configuration.h"

void update_alert_handler_config(Configuration new_config);

void update_alert_handler(struct tm *tick_time);

bool is_alert_active(void);

uint8_t get_alert_interval_remainder(void);
