int op_new_chan_handle();
void op_start();
void op_at(int chan_handle, int us, void (*fn)());
void op_bet(int chan_handle, int us1, int us2, void (*fn)());
void op_wait(int chan_handle, int us);
