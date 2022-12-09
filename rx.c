#include <stdio.h>

#include "op.h"

int rx_state = 0;

void rx_off() { printf("rx_off\n"); }

void rx_on() { printf("rx_on\n"); }

void toggle_rx_at(int us) {
  rx_state = !rx_state;
  op_at(us, rx_state ? rx_off : rx_on);
}
