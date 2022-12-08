#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define micros() (clock() / 10) // slow debug clock rate
/* #define micros() (clock() * 1000000 / CLOCKS_PER_SEC) */

void noop() {}

int op_cycle_start_tm = 0;
int op_inc_tm = 0;
int op_inc_idx = 0;
int op_idx = 0;

int rx_state = 0;

void op_start() {
  if (op_idx == op_inc_idx) {
    op_idx = 0;
    op_cycle_start_tm = micros();
    op_inc_tm = 0;
  }
  op_inc_idx = 0;
}

void op_at(int us, void (*fn)()) {
  /* printf("op_at      accul time: %d    inc_idx: %d    idx: %d\n", op_inc_tm + us, op_inc_idx, op_idx); */
  if (op_inc_idx == op_idx && micros() - op_cycle_start_tm >= op_inc_tm + us) {
    /* printf("fire\n"); */
    fn();
    op_inc_tm += us;
    op_idx++;
  }
  op_inc_idx++;
}

void op_wait(int us) {
  op_at(us, noop);
}

void rx_off() {
  printf("rx_off\n");
}

void rx_on() {
  printf("rx_on\n");
}

void toggle_rx_at(int us) {
  rx_state = !rx_state;
  op_at(us, rx_state ? rx_off : rx_on);
}

void main_cycle() {
  while (1) {
    usleep(100);
    /* printf("--- %lu\n", micros() - op_cycle_start_tm); */
    op_start();
    toggle_rx_at(0);
    toggle_rx_at(240);
    toggle_rx_at(240);
    toggle_rx_at(60);
    toggle_rx_at(60);
    toggle_rx_at(60);
    toggle_rx_at(60);
    toggle_rx_at(60);
    toggle_rx_at(60);
    toggle_rx_at(60);
    op_wait(540);
  }
}

int main() {
  main_cycle();
}
