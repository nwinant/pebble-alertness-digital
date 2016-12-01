#include <pebble.h>
#include "inttypes.h"
#include "modules/configuration.h"
#include "modules/alert_handler.h"
#include "modules/display.h"


/* ====  Variables  ================================================================ */

static Window *s_main_window;


/* ====  Timing  =================================================================== */

static void update_time(void) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating time");
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Update & refresh
  update_alert_handler(tick_time);
  refresh_display_layout();
  refresh_display_data(tick_time);
}


/* ====  Handlers  ================================================================= */

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void config_update_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "New config incoming...");
  update_config(iter, context);
  update_display_config();
  update_time();  // Refresh watch with changes
}

static void bluetooth_handler(bool connected) {
  //text_layer_set_text(s_connection_layer, connected ? "..." : "  !");
  text_layer_set_text(get_connection_layer(), connected ? "" : "  !");
}

static void battery_handler(BatteryChargeState charge_state) {
  // FIXME: should this be worked into refresh_display_data()?
  static char battery_text[] = "100%";
  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "...");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(get_battery_layer(), battery_text);
}

static void register_handlers(void) {
  // Configuration
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Registering config handler...");
  app_message_register_inbox_received(config_update_handler);
  
  // TODO: what are the ideal values?
  app_message_open(256, 256);
  //app_message_open(128, 128);
  //app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  // Max size consumes 8200 bytes of heap memory (PTS), potentially more in the future!
  
  // Ticks
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Battery & bluetooth
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_handler
  });
  battery_state_service_subscribe(battery_handler);
  bluetooth_handler(connection_service_peek_pebble_app_connection());
  battery_handler(battery_state_service_peek());
}

static void deregister_handlers(void) {
  tick_timer_service_unsubscribe();
  connection_service_unsubscribe();
  battery_state_service_unsubscribe();
  // FIXME: do we need to unsub the config handler?
}


/* ====  System hooks  ============================================================= */

static void init() {
  load_config();
  s_main_window = window_create();
  init_display(s_main_window);
  register_handlers();
  update_time();
}

static void deinit() {
  deregister_handlers();
  vibes_cancel();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}