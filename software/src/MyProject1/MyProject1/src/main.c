/*
 * GccApplication2.c
 *
 * Created: 1/28/2017 5:10:55 PM
 * Author : birrur
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
st_cmd_t rx_conf, rx_rtr;

//#define CAN_BUFFER_SIZE 8
#define CAN_ID  0x80


uint8_t can_buffer_tx[CAN_BUFFER_SIZE];
uint8_t can_buffer_rx[CAN_BUFFER_SIZE];



int main(void)
{
	uart_init(9600);
	fdevopen(&uart_putc, NULL);
	fdevopen((int (*)(char,  struct __file *))&uart_getc, NULL);
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
	
	//Can_config_rx();
	//Can_config_rx_buffer();
	//can_get_data(0x200);
	
	rx_conf.pt_data = &can_buffer_rx[0];
	rx_conf.ctrl.ide = 0;
	rx_conf.dlc = 8;//
	//rx_conf.ctrl.rtr = 0;
	rx_conf.id.std = CAN_ID+1;//
	//rx_conf.cmd = CMD_RX_DATA;
	rx_conf.cmd = CMD_RX_MASKED;

	//Setup receive message
	rx_rtr.pt_data = &can_buffer_rx[7];
	rx_rtr.ctrl.ide = 0;
	rx_rtr.ctrl.rtr = 1;
	rx_rtr.dlc = 8;
	rx_rtr.id.std = CAN_ID+1;
	rx_rtr.cmd = CMD_RX_REMOTE_MASKED;
	
	char temp_buff [8];
	char temp_two [30];
	char buffer[128];
	int len;
	
	//fflush(stdin);
	//puts("Who are you?");
	//fflush(stdin);
	//fgets(temp_buff, 8, stdin);
	//printf("Good to meet you, %s. \n", temp_buff);
	
	//for(;;) {
	//	printf("Last char in rx_buffer: ");
	//	printf("%c\n", uart_getc(NULL));
	//	_delay_ms(500);
	//}
	
	while (1)
	{
		can_buffer_tx[0] = 0xFF; //Byte 0
		can_buffer_tx[1] = 0x55; //Physical throttle
		can_buffer_tx[2] = 0x33; //Software throttle
		can_buffer_tx[3] = 0x0F; //Current
		can_buffer_tx[4] = 0xF0; //Current
		can_buffer_tx[5] = 0x77; //Voltage
		can_buffer_tx[6] = 0xaa; //Voltage
		can_buffer_tx[7] = 0x9c; //Reserved
		
		tx_msg.pt_data = &can_buffer_tx[0];
		tx_msg.ctrl.ide = 0;
		tx_msg.dlc = 7;
		tx_msg.id.std = 0x011;
		tx_msg.cmd = CMD_TX_DATA;
		
		/**
		uint8_t can_msg;
		puts("Message: ");
		for (int i = 0; i <8; i++){			
			can_msg = can_buffer_tx[i];
			sprintf(temp_buff, "%x", can_msg);
			puts(temp_buff);
		}*/
		
		//fgets(temp_buff, 8, stdin);
		//puts(temp_buff);
		//len = strlen(temp_buff);
		//puts(len);
		
		/**		
		uint8_t can_msg;
		fgets(temp_buff, 8, stdin);
		can_msg = uart_getc(temp_buff);
		sprintf(temp_buff, "%x", can_msg);*/
		
		for(;;) {
			puts("Type Something:");
			gets(&buffer[0]);
			puts(&buffer[0]);
			_delay_ms(500);
			_delay_ms(500);
		}
		
		_delay_ms(500);
		_delay_ms(500);
		_delay_ms(500);
		_delay_ms(500);
	
	}
}
	
	