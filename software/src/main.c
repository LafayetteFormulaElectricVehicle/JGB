#include "config.h"

//AVR-LIBC Headers 
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include <util/delay.h>

#include <stdio.h>

#include "can_lib.h"
#include "adc.h"
#include "timer.h"
#include "uart.h"

//Device CAN_ID
#define CAN_ID  0x080
#define CAN_MSK ~(0x03) 

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
	char str [] = "Hello World?\n";
	for(;;) {
		puts(str);
		//adc_1
		//uart_println(adc_read(2));
		////adc_2
		//uart_println(adc_read(3));
		////adc_3
		//uart_println(adc_read(6));
		////adc_4
		//uart_println(adc_read(7));
		////amp_0
		//uart_println(adc_read(15));
		////amp_1
		//uart_println(adc_read(16));
		
		_delay_ms(2000);
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
