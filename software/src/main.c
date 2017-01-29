#include "config.h"

//AVR-LIBC Headers 
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include <util/delay.h>

#include <stdio.h>

#include "jgb.h"
#include "uart.h"

//Device CAN_ID
#define CAN_ID  0x080
#define CAN_MSK ~(0x03) 

#define UART_NO_DATA 0x100

// Buffer for UART message
char buffer[30];

void setup(void) {
	analog_setup(ADC_AVCC, ADC_SCALE_128);
	dac_enable();
	uart_init(BAUD);
	
	fdevopen(&uart_putc, NULL);
	fdevopen(&uart_getc, NULL);
	
	sei();
}

void adc_test(void){
		sprintf(buffer, "ADC 1 reads %i",adc_read(ADC1)); 
		puts(buffer);
		sprintf(buffer, "ADC 2 reads %i",adc_read(ADC2)); 
		puts(buffer);
		sprintf(buffer, "ADC 3 reads %i",adc_read(ADC3)); 
		puts(buffer);
		sprintf(buffer, "ADC 4 reads %i",adc_read(ADC4)); 
		puts(buffer);
		_delay_ms(500);

}

void dac_test(void){
	dac_write(0x3FF);
	puts("Set DAC to 2^10-1 (1023)");
	strcpy(buffer, "Set DAC to 0x40");
	puts(buffer);
	dac_write(0x40);
	_delay_ms(500);
	strcpy(buffer, "Set DAC to 0x3FF");
	puts(buffer);
	dac_write(0x3FF);

}

int main(void) {
	//Setup Comment
	setup();

	while(1){
		adc_test();
	}
	
	return 0;
}
