#include "jgb.h"

#include <avr/io.h>

#include <stdint.h>

void analog_setup(uint8_t mode, uint8_t scale) {
	switch(mode) {
		//AREF SETUP
		case ADC_AREF:
			ADCSRA = (scale << ADPS0);
			ADCSRB = (1 << AREFEN);
			ADCSRA = (1 << ADEN);
			break;
		//AVCC
		case ADC_AVCC:
			// AREF = AVcc
			ADMUX = (1<<REFS0);
			// ADC Enable and prescaler of 128
			// 16000000/128 = 125000
			ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
			break;
	}
}

uint16_t adc_read(uint8_t pin) {
	//Clear Lower 5 bits (MUX[4:0])
	ADMUX &= ~(0x1F << MUX0);
	ADMUX |= (pin << MUX0);
	
	uint16_t adc_value;
	
	//throw out first conversion
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1<<ADSC));
	adc_value = ADC;
	
	//2nd conversion gets saved
	ADCSRA |= (1 << ADSC);
	//Wait for complete conversion
	while(ADCSRA & (1<<ADSC));
	adc_value = ADC;
	
	return adc_value;
}

void dac_enable(void) {
	DACON = (1 << DAOE) | (1 << DAEN);
}

void dac_disable(void) {
	DACON = 0;
}

void dac_write(uint16_t level) {
	// The DACH write updates the d2a
	// DACH is only 2 bits wide
	// Max value 0x3FF
	DACL = level;
	DACH = (level >> 8);
}

void ac_enable(uint8_t comparator, uint8_t mode){
	switch(comparator){
		case AMP0:
			AC0CON = mode | AMP_enb;
			break;
		case AMP1:
			AC1CON = mode | AMP_enb;
			break;
		}
}

uint8_t ac_status(uint8_t comparator){
	return ACSR;
	switch(comparator){
		case AMP0:
			return ((ACSR & 0x01) != 0);
		case AMP1:
			return ((ACSR & 0x02) != 0);
		}
	return 0;
}

void open_relay(uint8_t relay_num){
	
	if(relay_num == RELAY1){
		PORTC &= ~(1 << 6);
		}else if (relay_num == RELAY2){
		PORTD &= ~(1 << 0);
		}else if (relay_num == RELAY3){
		PORTD &= ~(1 << 1);
	}
	
}

void close_relay(uint8_t relay_num){
	
	if(relay_num == RELAY1){
		PORTC |= (1 << 6);
		}else if (relay_num == RELAY2){
		PORTD |= (1 << 0);
		}else if (relay_num == RELAY3){
		PORTD |= (1 << 1);
	}
	
}



