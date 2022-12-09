#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "macros.h"
#include "op.h"
#include "rx.h"

void main_cycle() {
  for (;;) {
#ifdef DEBUG
    usleep(100);
#endif
    usleep(10);
    op_start();
    op_at(0, rx_on);
    op_at(240, rx_off);
    op_at(240, rx_on);
    op_at(60, rx_off);
    op_at(60, rx_on);
    op_at(60, rx_off);
    op_at(60, rx_on);
    op_at(60, rx_off);
    op_at(60, rx_on);
    op_at(60, rx_off);
    op_wait(540);
  }
}

int main() { main_cycle(); }
