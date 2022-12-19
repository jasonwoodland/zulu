#include "macros.h"
#include "op.h"
#include "tx.h"
#include "rx.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  op_init();
  tx_init();
  rx_init();
  startup_led_flash();
  audio_startup_chime();
}

void loop() {
  op_start();
  tx_tick();
  rx_tick();
  audio_tick();
}

void startup_led_flash() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}
