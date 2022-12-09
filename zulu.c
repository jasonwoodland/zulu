#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "macros.h"
#include "op.h"
#include "tx.h"

void main_cycle() {
  tx_init();
  for (;;) {
#ifdef DEBUG
    usleep(100);
#endif
    op_start();
    tx_tick();
  }
}

int main() { main_cycle(); }
