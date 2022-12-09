#include <stdio.h>
#include <time.h>

#include "helpers.h"
#include "macros.h"

#define OP_MAX_CHANS 32

/* int op_cycle_start_tm = 0; */
/* int op_wait_for_tm = 0; */
/* int op_idx = 0; */
/* int op_wait_for_idx = 0; */

int dbg_cycle_cnt = 0;

struct channel {
  int cycle_start_tm;
  int wait_for_tm;
  int wait_for_idx;
  int idx;
};

struct channel chans[OP_MAX_CHANS];
int op_chan_count = 0;

int op_new_chan_handle() { return op_chan_count++; }

void op_start() {
  for (int chan_handle = 0; chan_handle < op_chan_count; chan_handle++) {
    struct channel *chan = &chans[chan_handle];
    if (chan->wait_for_idx == chan->idx) {
      /* #ifdef DEBUG */
      /* printf("cycles %d\n", dbg_cycle_cnt); */
      dbg_cycle_cnt = 0;
      /* #endif */

      chan->cycle_start_tm = micros();
      chan->wait_for_tm = 0;
      chan->wait_for_idx = 0;
    }

    chan->idx = 0;

    dbg_cycle_cnt++;
  }
}

void op_at(int chan_handle, int us, void (*fn)()) {
  struct channel *chan = &chans[chan_handle];
  if (chan->idx == chan->wait_for_idx &&
      micros() - chan->cycle_start_tm >= chan->wait_for_tm + us) {
    fn();
    chan->wait_for_tm += us;
    chan->wait_for_idx++;
  }
  chan->idx++;
}

void op_bet(int chan_handle, int us, int dur, void (*fn)()) {
  struct channel *chan = &chans[chan_handle];
  if (chan->idx == chan->wait_for_idx &&
      micros() - chan->cycle_start_tm >= chan->wait_for_tm + us &&
      micros() - chan->cycle_start_tm < chan->wait_for_tm + us + dur) {
    fn();
  }
  if (chan->idx == chan->wait_for_idx &&
      micros() - chan->cycle_start_tm >= chan->wait_for_tm + us + dur) {
    chan->wait_for_idx++;
  }
  chan->idx++;
}

void op_wait(int chan_handle, int us) { op_at(chan_handle, us, nop); }
