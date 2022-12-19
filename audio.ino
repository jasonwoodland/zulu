#include <Arduino.h>
#include <DueTimer.h>

#include "pins.h"
#include "op.h"

#define AUDIO_OUT_PIN 7





// timers TC0 TC1 TC2   channels 0-2 ids 0-2  3-5  6-8     AB 0 1
// use TC1 channel 0 
#define TONE_TIMER TC1
#define TONE_CHNL 0
#define TONE_IRQ TC3_IRQn

// TIMER_CLOCK4   84MHz/128 with 16 bit counter give 10 Hz to 656KHz
//  piano 27Hz to 4KHz

static uint8_t pinEnabled[PINS_COUNT];
static uint8_t TCChanEnabled = 0;
static boolean pin_state = false ;
static Tc *chTC = TONE_TIMER;
static uint32_t chNo = TONE_CHNL;

volatile static int32_t toggle_count;
static uint32_t tone_pin;

// frequency (in hertz) and duration (in milliseconds).

void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
{
		const uint32_t rc = VARIANT_MCK / 256 / frequency; 
		tone_pin = ulPin;
		toggle_count = 0;  // strange  wipe out previous duration
		if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
		 else toggle_count = -1;

		if (!TCChanEnabled) {
 			pmc_set_writeprotect(false);
			pmc_enable_periph_clk((uint32_t)TONE_IRQ);
			TC_Configure(chTC, chNo,
				TC_CMR_TCCLKS_TIMER_CLOCK4 |
				TC_CMR_WAVE |         // Waveform mode
				TC_CMR_WAVSEL_UP_RC ); // Counter running up and reset when equals to RC
	
			chTC->TC_CHANNEL[chNo].TC_IER=TC_IER_CPCS;  // RC compare interrupt
			chTC->TC_CHANNEL[chNo].TC_IDR=~TC_IER_CPCS;
			 NVIC_EnableIRQ(TONE_IRQ);
                         TCChanEnabled = 1;
		}
		if (!pinEnabled[ulPin]) {
			pinMode(ulPin, OUTPUT);
			pinEnabled[ulPin] = 1;
		}
		TC_Stop(chTC, chNo);
                TC_SetRC(chTC, chNo, rc);    // set frequency
		TC_Start(chTC, chNo);
}

void noTone(uint32_t ulPin)
{
	TC_Stop(chTC, chNo);  // stop timer
	digitalWrite(ulPin,LOW);  // no signal on pin
}

/* // timer ISR  TC1 ch 0 */
/* void TC3_Handler ( void ) { */
/* 	TC_GetStatus(TC1, 0); */
/* 	if (toggle_count != 0){ */
/* 		// toggle pin  TODO  better */
/* 		digitalWrite(tone_pin,pin_state= !pin_state); */
/* 		if (toggle_count > 0) toggle_count--; */
/* 	} else { */
/* 		noTone(tone_pin); */
/* 	} */
/* } */



int _freq;

void audio_init() {
  pin_mode(AUDIO_OUT_PIN, OUTPUT);
  Timer2.attachInterrupt(_toggle);
  Timer2.start();
}

void audio_tick() {
  tone(AUDIO_OUT_PIN, 440, 0);
  // analogWriteFrequency(AUDIO_OUT_PIN, 440);
}

void audio_startup_chime() {
  tone(AUDIO_OUT_PIN, 440, 100);
  tone(AUDIO_OUT_PIN, 880, 100);
  // analogWriteFrequency(AUDIO_OUT_PIN, 440);
  // delay(100);
  // analogWriteFrequency(AUDIO_OUT_PIN, 880);
  // delay(100);
}

void set_freq(int freq) {
  Timer2.setFrequency(freq);
}

void _toggle() {
  digitalWrite(AUDIO_OUT_PIN, !digitalRead(AUDIO_OUT_PIN));
}