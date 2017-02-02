/*
 * GccApplication2.c
 *
 * Created: 1/28/2017 5:10:55 PM
 * Author : townleym
 */ 
//# define F_CPU 1000000UL
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "jgb.h"
#include <avr/interrupt.h>

//new inclusions 1/31/17:
#include <avr/cpufunc.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdio.h>
#include "timer.h"

//#define sei
st_cmd_t tx_msg;
st_cmd_t rx_conf, rx_rtr;

//#define CAN_BUFFER_SIZE 8
#define CAN_ID  0x200


uint8_t can_buffer_tx[CAN_BUFFER_SIZE];
uint8_t can_buffer_rx[CAN_BUFFER_SIZE];



int main(void)
{
	//_delay_ms(5);
	/* Replace with your application code */
	//DDRC = (1 << 6);
	//DDRD = (1 << 0) | (1 << 1);
	//DDRD = (1 << 1);
	
	//set_gpio_mode(GPIO0, OUTPUT);
	//set_gpio_mode(GPIO1, OUTPUT);
	//set_gpio_mode(GPIO2, OUTPUT);
	//set_gpio(GPIO0);
	uart_init(9600);
	fdevopen(&uart_putc, NULL);
	fdevopen((int (*)(char,  struct __file *))&uart_getc, NULL);
	//fdevopen(&uart_getc, NULL);
	can_init(0);
	CANGIE |= (1 << ENBOFF);
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
	rx_conf.dlc = 2;//
	rx_conf.ctrl.rtr = 0;
	rx_conf.id.std = CAN_ID+1;//
	//rx_conf.cmd = CMD_RX_DATA;
	rx_conf.cmd = CMD_RX_DATA_MASKED;

	//Setup receive message
	rx_rtr.pt_data = &can_buffer_rx[7];
	rx_rtr.ctrl.ide = 0;
	rx_rtr.ctrl.rtr = 1;
	rx_rtr.dlc = 0;
	rx_rtr.id.std = CAN_ID+1;
	rx_rtr.cmd = CMD_RX_REMOTE_MASKED;
	
	while (1)
	{
		
		//puts("Hello World!");
		
		//set_gpio(GPIO1);
		
		//open_relay(RELAY1);
		
		//To be changed for updated communications protocol:
		can_buffer_tx[0] = 0xFF; //Byte 0
		can_buffer_tx[1] = 0xFF; //Physical throttle
		can_buffer_tx[2] = 0xFF; //Software throttle
		can_buffer_tx[3] = 0xFF; //Current
		can_buffer_tx[4] = 0xFF; //Current
		can_buffer_tx[5] = 0xFF; //Voltage
		can_buffer_tx[6] = 0xFF; //Voltage
		can_buffer_tx[7] = 0xFF; //Reserved
		
		//can_buffer_rx[0] = 0x00; //Byte 0
		//can_buffer_rx[1] = 0x00; //Software throttle
		//can_buffer_rx[2] = 0x00;
		//can_buffer_rx[3] = 0x00;
		//can_buffer_rx[4] = 0x00;
		//can_buffer_rx[5] = 0x00;
		//can_buffer_rx[6] = 0x00;
		//can_buffer_rx[7] = 0x00;
		
		
		tx_msg.pt_data = &can_buffer_tx[0];
		tx_msg.ctrl.ide = 0;
		tx_msg.dlc = 7;
		tx_msg.id.std = CAN_ID;
		tx_msg.cmd = CMD_TX_DATA;
		
		
		
		
		
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		
		//SETUP FOR RECEIVING BOARD:
		
		puts("Hello World!");
		//can_get_data(0x201);
		while(can_cmd(&rx_conf) != CAN_CMD_ACCEPTED);
		//wait for receiving MOb to configure
		//while(can_cmd(&rx_conf) != CAN_CMD_ACCEPTED){
			//uint8_t temp_num;
			//temp_num = can_buffer_rx[0] | can_buffer_rx[1] | can_buffer_rx[2] | can_buffer_rx[3] | can_buffer_rx[4] | can_buffer_rx[5] | can_buffer_rx[6] | can_buffer_rx[7];
			//can_get_data(0x200);
			
			//if (temp_num == 0){
				//puts("X");
				//}else{
				//puts("O");
			//}
		//}
		//puts("O");
		_delay_ms(500);
		uint8_t temp_num;
		temp_num = can_buffer_rx[0] | can_buffer_rx[1] | can_buffer_rx[2] | can_buffer_rx[3] | can_buffer_rx[4] | can_buffer_rx[5] | can_buffer_rx[6] | can_buffer_rx[7];
		if (temp_num == 0){
			puts("X");
			}else{
			puts("O");
			break;
		}
		_delay_ms(500);
		
		
		
		//if(can_cmd(&rx_conf) == CAN_CMD_ACCEPTED);
		
		////uint8_t temp_status;
		////temp_status = rx_conf.status;
		////char[16] status_str;
		////itoa(temp_status, status_str, 16);
		////puts("Status:\n");
		////puts(status_str);
		//}
		//wait for RTR MOb to configure
		//while(can_cmd(&rx_rtr) != CAN_CMD_ACCEPTED);
			//puts("rtr");
			//}
		
		//if(can_buffer_rx[0] != 0){
		//set_gpio(GPIO1);
		//}
		//open_relay(RELAY1);
		//_delay_ms(500);
		//close_relay(RELAY1);
		
		//clear_gpio(GPIO1);
		//_delay_ms(500);
		
		//char buff;
		
		//puts("a");
		//stuff:
		/*
		uint8_t temp_num;
		temp_num = can_buffer_rx[0] | can_buffer_rx[1] | can_buffer_rx[2] | can_buffer_rx[3] | can_buffer_rx[4] | can_buffer_rx[5] | can_buffer_rx[6] | can_buffer_rx[7];
		
		////if (temp_num == 0){
			////puts("received message is all zeros");
		////}else{
			////puts("received message is NOT all zeros");
		////}
		////*/
		////
		////
		//////itoa(temp_num, buff, 16);
		////
		//////puts(buff);
		
		
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//SETUP FOR TRANSMITTING BOARD
		
		
		////while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED){
			////puts("first loop");
		////}
			//////uint8_t temp_status;
			//////temp_status = tx_msg.status;
			//////char status_str;
			//////itoa(temp_status, status_str, 16);
			//////puts(status_str);
		//////puts("Between while loops");
		////while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
			//////puts("second loop");
		//////puts("After transmission");
		////
		//////open_relay(RELAY1);
		////_delay_ms(500);
		////
		////
		////uint8_t temp_num;
		////temp_num = can_buffer_tx[0] | can_buffer_tx[1] | can_buffer_tx[2] | can_buffer_tx[3] | can_buffer_tx[4] | can_buffer_tx[5] | can_buffer_tx[6] | can_buffer_tx[7];
		////if (temp_num == 0xFF){
			////puts("its all F's!");
		////}else if(temp_num != 0){
			////puts("it's not zero!");
		////}
		////_delay_ms(500);
		
		//THIS IS BAD CODE AND CRASHES THE PROGRAM!!!!!!!!!!
		//char buff;
		//uint8_t temp_num;
		//temp_num = can_buffer_tx[0] | can_buffer_tx[1] | can_buffer_tx[2] | can_buffer_tx[3] | can_buffer_tx[4] | can_buffer_tx[5] | can_buffer_tx[6] | can_buffer_tx[7];
		//
		//itoa(temp_num, buff, 16);
		
		//puts(buff);
		
		//clear_gpio(GPIO1);
		//close_relay(RELAY1);
		
		
		
		
		


		
		}
	}
	
	