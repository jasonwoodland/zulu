#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "macros.h"
#include "op.h"
#include "tx.h"

void main_cycle() {
  /* int rx_h = op_new_chan_handle(); */
  int tx_h = op_new_chan_handle();
  /* int snd_h = op_chan(); */
  for (;;) {
#ifdef DEBUG
    usleep(100);
#endif
    op_start();
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
    /* op_at(tx_h, 0, foo); */
    /* op_wait(tx_h, 100); */
    /* op_bet(rx_h, 400, 200, bar); */
    /* op_wait(rx_h, 1000); */
  }
}

int main() { main_cycle(); }
