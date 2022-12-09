#include <stdio.h>
#include <time.h>

#include "helpers.h"
#include "macros.h"

int op_cycle_start_tm = 0;
int op_inc_tm = 0;
int op_inc_idx = 0;
int op_idx = 0;

int dbg_cycle_cnt = 0;

void op_start() {
  if (op_idx == op_inc_idx) {
    /* #ifdef DEBUG */
    printf("cycles %d\n", dbg_cycle_cnt);
    dbg_cycle_cnt = 0;
    /* #endif */

    op_idx = 0;
    op_cycle_start_tm = micros();
    op_inc_tm = 0;
  }

  op_inc_idx = 0;

  dbg_cycle_cnt++;
}

void op_at(int us, void (*fn)()) {
  /* printf("op_at	accul time: %d	inc_idx: %d	idx: %d\n", op_inc_tm +
   * us, op_inc_idx, op_idx); */
  if (op_inc_idx == op_idx && micros() - op_cycle_start_tm >= op_inc_tm + us) {
    fn();
    op_inc_tm += us;
    op_idx++;
  }
  op_inc_idx++;
}

void op_wait(int us) { op_at(us, nop); }
