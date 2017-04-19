/*
 * GccApplication2.c
 *
 * Created: 1/28/2017 5:10:55 PM
 * Author : Raji Birru
 */ 

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "jgb.h"
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdio.h>
#include "timer.h"

#include <string.h>

//#define sei
st_cmd_t tx_msg;
st_cmd_t rx_msg;

//#define CAN_BUFFER_SIZE 8
#define CAN_ID  0x80


uint8_t can_buffer_tx[CAN_BUFFER_SIZE];
uint8_t can_buffer_rx[CAN_BUFFER_SIZE];



int main(void)
{
	uart_init(9600);
	
	//fdevopen(&uart_putc, NULL);
	//fdevopen((int (*)(char,  struct __file *))&uart_getc, NULL);
	
	fdevopen(&uart_putc, &uart_getc);
	can_init(0);
	CANGIE |= (1 << ENIT);
	sei();
	
	can_buffer_rx[0] = 0x00; //Byte 0
	can_buffer_rx[1] = 0x00; //Software throttle
	can_buffer_rx[2] = 0x00;
	can_buffer_rx[3] = 0x00;
	can_buffer_rx[4] = 0x00;
	can_buffer_rx[5] = 0x00;
	can_buffer_rx[6] = 0x00;
	can_buffer_rx[7] = 0x00;
	
	/**
	Can_config_rx();
	Can_config_rx_buffer();
	can_get_data(0x200);
	
	
	rx_conf.pt_data = &can_buffer_rx[0];
	rx_conf.ctrl.ide = 0;
	rx_conf.dlc = 8;
	rx_conf.id.std = CAN_ID;
	rx_conf.cmd = CMD_RX_MASKED;

	//Setup receive message
	rx_rtr.pt_data = &can_buffer_rx[7];
	rx_rtr.ctrl.ide = 0;
	rx_rtr.ctrl.rtr = 1;
	rx_rtr.dlc = 8;
	rx_rtr.id.std = CAN_ID+1;
	rx_rtr.cmd = CMD_RX_REMOTE_MASKED;
	*/
	
	char temp_buff [8];
	char temp_two [30];
	char buffer[128];
	int i = 0;
	uint8_t curr;
	
	/**
	for(;;) {
		puts("Type Something:");
		fgets(&buffer[0], 8, stdin);
		puts(&buffer[0]);
		_delay_ms(2000);
	}*/
	
	
	for(;;) {
		curr = uart_getc(NULL);
		if(i < 8){
			printf("Last char in rx_buffer: ");
			printf("%c\n", curr);
		}
		else if (i == 8) {
			printf("\n 8 ENTERED \n");
			i++;
		}
		else if (i > 8){
			i++;
			printf("Now sending over can bus. \n");
			break;
		}
		
		if(curr != NULL){
			printf("\n GOT ONE \n");
			can_buffer_tx[i] = curr;
			i = i+1;
		}				
		
		_delay_ms(2000);
	}
	
	
	while (1)
	{	
		
		
		/************************************************************************/
		/*                         CANBUS SEND SECTION                          */
		/************************************************************************/
		
		//can_buffer_tx[0] = 0xFF; //Byte 0
		//can_buffer_tx[1] = 0x55; //Physical throttle
		//can_buffer_tx[2] = 0x33; //Software throttle
		//can_buffer_tx[3] = 0x0F; //Current
		//can_buffer_tx[4] = 0xF0; //Current
		//can_buffer_tx[5] = 0x77; //Voltage
		//can_buffer_tx[6] = 0xaa; //Voltage
		//can_buffer_tx[7] = 0x9c; //Reserved
										
		// point message object to first element of data buffer
		tx_msg.pt_data = &can_buffer_tx[0];
		// standard CAN frame type (2.0A)
		tx_msg.ctrl.ide = 0;
		// Number of bytes being sent (8 max)
		tx_msg.dlc = CAN_BUFFER_SIZE;
		// populate ID field with ID Tag
		tx_msg.id.std = CAN_ID;
		// assign this as a "Standard (2.0A) Reply" message object
		tx_msg.cmd = CMD_TX_DATA;
		
		// wait for MOb to configure
		while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);

		// wait for a transmit request to come in, and send a response
		while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
		
		printf("Last char sent: ");
		printf("%c\n", can_buffer_tx[7]);
		
		
		/************************************************************************/
		/*                      CANBUS RECEIVE SECTION                          */
		/************************************************************************/
		
		/**
		rx_msg.pt_data = &can_buffer_rx[0];
		rx_msg.ctrl.ide = 0;
		rx_msg.dlc = CAN_BUFFER_SIZE;
		rx_msg.id.std = CAN_ID;
		rx_msg.cmd = CMD_RX_MASKED;
		
		while(can_cmd(&rx_msg) != CAN_CMD_ACCEPTED);
		while(can_get_status(&rx_msg) == CAN_STATUS_NOT_COMPLETED);
		
		printf("Chars received: ");
		for (int k = 0; k < 8; k++){			
			printf("%c\n", can_buffer_rx[k]);
		}
		*/
		_delay_ms(5000);
		
	}
}
	
	