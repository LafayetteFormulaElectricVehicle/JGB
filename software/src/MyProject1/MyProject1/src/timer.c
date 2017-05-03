#include "timer.h"

#include <avr/io.h>

void timer0_init(uint8_t timerScale) {
	//TCCR0A = (2 << COM0A0) | (3 << WGM00);
	TCCR0B = (timerScale << CS00);
	TCNT0 = 0;
}

void timer0_disable(void) {
	TCCR0B = (TIM_SCALE_0 << CS10);
	TCNT0 = 0;
}

void timer1_init(uint8_t timerScale) {
	//Set Timer Clock Source
	TCCR1B = (timerScale << CS10);
	TCNT1H = 0;
	TCNT1L = 0;
}

void timer1_disable(void) {
	TCCR1B = (TIM_SCALE_0 << CS10);
	TCNT1H = 0;
	TCNT1L = 0;
}

bool timer0_pwm_init(uint8_t ocPin, uint8_t pwmMode, uint8_t wgmMode) {
	switch(ocPin) {
		case PWM_OCxA:
			TCCR0A = (pwmMode << COM0A0) | (wgmMode << WGM00);
			return true;
		case PWM_OCxB:
			TCCR0B = (pwmMode << COM0A0) | (wgmMode << WGM00);
			return true;
		default:
			return false;
	}
	return false;
}

bool timer0_pwm_write(uint8_t ocPin, uint8_t value) {
	switch(ocPin) {
		case PWM_OCxA:
			OCR0A = value;
			return true;
		case PWM_OCxB:
			OCR0B = value;
			return true;
		default:
			return false;
	}
	return false;

}

void timer1_pwm_init(uint8_t ocPin, uint8_t pwmMode, uint8_t wgmMode) {
	
}

void timer1_pwm_write(uint8_t ocPin, uint8_t value) {
	
}


