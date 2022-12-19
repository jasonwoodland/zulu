#include <stdio.h>
#include <DueTimer.h>

#include "helpers.h"
#include "macros.h"

#define OP_MAX_CHANS 32
#define OP_MAX_OP_PER_CHAN 32


/* int op_cycle_start_tm = 0; */
/* int op_wait_for_tm = 0; */
/* int op_idx = 0; */
/* int op_wait_for_idx = 0; */

int dbg_cycle_cnt = 0;
int op_micros;

struct channel {
  int cycle_start_tm;
  int wait_for_tm;
  int wait_for_idx;
  int idx;
};

struct channel chans[OP_MAX_CHANS];
int op_chan_count = 0;

// DueTimer op_timer;
int op_bet_block_flag = 0;

void op_init() {
  Timer1.attachInterrupt(op_bet_block_clear);
}

int op_fn_dur[OP_MAX_CHANS][OP_MAX_OP_PER_CHAN];

int op_new_chan_handle() { return op_chan_count++; }

void op_start() {
  op_micros = micros() / 10;
  for (int chan_handle = 0; chan_handle < op_chan_count; chan_handle++) {
    struct channel *chan = &chans[chan_handle];
    if (chan->wait_for_idx == chan->idx) {
      chan->cycle_start_tm = op_micros;
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
      op_micros - chan->cycle_start_tm >= chan->wait_for_tm + us) {
    fn();
    chan->wait_for_tm += us;
    chan->wait_for_idx++;
  }
  chan->idx++;
}

void op_bet(int chan_handle, int us, int dur, void (*fn)()) {
  struct channel *chan = &chans[chan_handle];
  if (chan->idx == chan->wait_for_idx &&
      op_micros - chan->cycle_start_tm >= chan->wait_for_tm + us &&
      op_micros - chan->cycle_start_tm < chan->wait_for_tm + us + dur) {
    fn();
  }
  if (chan->idx == chan->wait_for_idx &&
      op_micros - chan->cycle_start_tm >= chan->wait_for_tm + us + dur) {
    chan->wait_for_tm += us + dur;
    chan->wait_for_idx++;
  }
  chan->idx++;
}

void op_bet_block_clear() {
  op_bet_block_flag = 0;
  // Serial.println("x");
}

void op_bet_block(int chan_handle, int us, int dur, void (*fn)()) {
  struct channel *chan = &chans[chan_handle];
  if (chan->idx == chan->wait_for_idx &&
      op_micros - chan->cycle_start_tm >= chan->wait_for_tm + us &&
      op_micros - chan->cycle_start_tm < chan->wait_for_tm + us + dur) {
    op_bet_block_flag = 1;
    Timer1.start(dur);
    while (op_bet_block_flag) {
      fn();
    }
    Timer1.stop();
    chan->wait_for_tm += us + dur;
    chan->wait_for_idx++;
  }
  chan->idx++;
}

void op_wait(int chan_handle, int us) {
  op_at(chan_handle, us, nop);
}

