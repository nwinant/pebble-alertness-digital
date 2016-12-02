#pragma once
#include "configuration.h"

void update_alert_handler(struct tm *tick_time);

bool is_alert_timer_running(void);

uint8_t get_alert_interval_remainder(void);

bool is_alert_currently_active(void);

void dismiss_current_alert(void);

// FIXME: Add callbacks for alert status updates to refresh display - 2016-12-01
