#include <Arduino.h>

#include "pins.h"
#include "op.h"

#define RX_PIN A0

int rx_h;
byte buf[20000];
int buf_idx = 0;

int d;

void rx_init() {
  rx_h = op_new_chan_handle();
  pin_mode(RX_PIN, INPUT_PULLUP);
  analogReadResolution(8);
  ADC->ADC_MR |= 0x80; // these lines set free running mode on adc 7 (pin A0)
  ADC->ADC_CR=2;
  ADC->ADC_CHER=0x80;

  // debug! delete
  pinMode(3, OUTPUT);
  DAC = 0b10000001; // set timer/counter 1 to fast PWM mode, clear OC1A on compare match
  TCCR1B = 0b00011001; // set timer/counter 1 to fast PWM mode, set prescaler to 1/8
}

void rx_clear() {
  buf_idx = 0;
}

void rx_read() {
  buf[buf_idx++] = ADC->ADC_CDR[7];

  // debug! delete
  // OCR1A = ADC->ADC_CDR[7]; // set the compare match value for timer/counter 1
  analogWrite(3, ADC->ADC_CDR[7])
  // buf_idx++;
}

void rx_process() {
  // d++;
  // if (d==10) {
  Serial.print("buf len: ");
  Serial.println(buf_idx);
  // Serial.println(buf[0]);

  // d=0;
  // }
  buf_idx = 0;
}

void rx_tick() {
  op_at(rx_h, 0, rx_clear);
  op_bet_block(rx_h, 240, 240, rx_read);
  op_at(rx_h, 0, rx_process);
  op_bet_block(rx_h, 60, 60, rx_read);
  op_at(rx_h, 0, rx_process);
  op_bet_block(rx_h, 60, 60, rx_read);
  op_at(rx_h, 0, rx_process);
  op_bet_block(rx_h, 60, 60, rx_read);
  op_at(rx_h, 0, rx_process);
  op_bet_block(rx_h, 60, 540, rx_read);
  op_at(rx_h, 0, rx_process);
}

