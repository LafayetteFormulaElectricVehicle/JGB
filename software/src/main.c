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
	char c;
	char str [] = "Enter DAC count (0-1023)";
	char buffer [20];
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
		_delay_ms(500);

		strcpy(str, "Set DAC to 0x3FF");
		puts(str);
		dac_write(0x3FF);

		_delay_ms(500);
	}
	
	
	////point message object to first element of data buffer
	//tx_msg.pt_data = &tx_buffer[0];
	////standard CAN frame type (2.0A)
	//tx_msg.ctrl.ide = 0;
	////Number of bytes being sent (8 max)
	//tx_msg.dlc = CAN_BUFFER_SIZE;
	////populate ID field with ID Tag
	//tx_msg.id.std = CAN_ID;
	////assign this as a "Standard (2.0A) Reply" message object
	//tx_msg.cmd = CMD_TX_DATA;

	////wait for MOb to configure
    //while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);

    //// send a response
    //while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
    //_delay_ms(1000);
	
	return 0;
}
