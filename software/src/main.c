#include "config.h"

//AVR-LIBC Headers 
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include <util/delay.h>

#include <stdio.h>

#include "can_lib.h"
#include "jgb.h"
#include "timer.h"
#include "uart.h"

//Device CAN_ID
#define CAN_ID  0x080
#define CAN_MSK ~(0x03) 

#define UART_NO_DATA 0x100

//uint8_t response_data[CAN_BUFFER_SIZE];
uint8_t can_buffer_tx[CAN_BUFFER_SIZE];
uint8_t can_buffer_rx[CAN_BUFFER_SIZE];

// Buffer for UART message
char buffer[30];

void setup(void) {
	analog_setup(ADC_AVCC, ADC_SCALE_128);

	uart_init(BAUD);
	
	can_init(0);
	
	fdevopen(&uart_putc, NULL);
	fdevopen(&uart_getc, NULL);
	
	sei();
}

int main(void) {
	st_cmd_t tx_msg;
	st_cmd_t rx_msg;
	//Setup Comment
	setup();

	uint16_t val = 0x0000;
	char str [] = "Enter DAC count (0-1023)";
	int buffer_index = 0;
	puts("Enabling DAC");
	dac_enable();
	puts("DAC Online");
	dac_write(0x3FF);
	puts("Set DAC to 2^10-1 (1023)");
	for(;;) {
		strcpy(str, "Set DAC to 0x40");
		puts(str);
		dac_write(0x40);
		sprintf(buffer, "ADC 1 reads %i",adc_read(ADC3)); 
		puts(buffer);
		_delay_ms(500);

		strcpy(str, "Set DAC to 0x3FF");
		sprintf(buffer, "ADC 1 reads %i",adc_read(ADC3)); 
		puts(buffer);
		dac_write(0x3FF);

		_delay_ms(500);
	}
	
	return 0;
}
