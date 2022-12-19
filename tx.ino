#include <Arduino.h>

#include "pins.h"
#include "op.h"

#define TX_PIN LED_BUILTIN

int tx_h;

void tx_init() {
  tx_h = op_new_chan_handle();
  pin_mode(TX_PIN, OUTPUT);
}

void tx_off() {
  digital_write(TX_PIN, LOW);

}

void tx_on() {
  digital_write(TX_PIN, HIGH);
}

void tx_tick() {
  op_at(tx_h, 0, tx_on);
  op_at(tx_h, 240, tx_off);
  op_at(tx_h, 240, tx_on);
  op_at(tx_h, 60, tx_off);
  op_at(tx_h, 60, tx_on);
  op_at(tx_h, 60, tx_off);
  op_at(tx_h, 60, tx_on);
  op_at(tx_h, 60, tx_off);
  op_at(tx_h, 60, tx_on);
  op_at(tx_h, 60, tx_off);
  op_wait(tx_h, 540);
}
