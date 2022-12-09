#include <stdio.h>

#include "op.h"

int tx_h;

void tx_off() { printf("tx_off\n"); }
void tx_on() { printf("tx_on\n"); }

void tx_init() { tx_h = op_new_chan_handle(); }

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
