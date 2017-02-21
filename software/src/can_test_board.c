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
#define CAN_ID  0x80


uint8_t can_buffer_tx[CAN_BUFFER_SIZE];
uint8_t can_buffer_rx[CAN_BUFFER_SIZE];



int main(void)
{
	//_delay_ms(5);
	/* Replace with your application code */
	//DDRC = (1 << 6);
	//DDRD = (1 << 0) | (1 << 1);
	//DDRD = (1 << 1);
	
	set_gpio_mode(GPIO0, INPUT);
	set_gpio_mode(GPIO1, INPUT);
	set_gpio_mode(GPIO2, INPUT);
	//set_gpio(GPIO0);
	uart_init(9600);
	fdevopen(&uart_putc, NULL);
	fdevopen((int (*)(char,  struct __file *))&uart_getc, NULL);
	//fdevopen(&uart_getc, NULL);
	can_init(0);
	//CANGIE |= (1 << ENBOFF);
	CANGIE |= (1 << ENIT);
	CANGIE |= (1 << ENRX);
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
	
	char temp_buff [30];
	
	can_buffer_tx[0] = 0x00; //Byte 0
	can_buffer_tx[1] = 0x00; //Physical throttle
	can_buffer_tx[2] = 0x00; //Software throttle
	can_buffer_tx[3] = 0x00; //Current
	can_buffer_tx[4] = 0x00; //Current
	can_buffer_tx[5] = 0x00; //Voltage
	can_buffer_tx[6] = 0x00; //Voltage
	can_buffer_tx[7] = 0x00; //Reserved
	
	uint16_t aval;
	uint8_t sval;
	uint16_t ad2val;
	
	uint8_t update_tx0;
	uint8_t update_tx1;
	uint8_t update_tx2;
	uint8_t update_tx3;
	uint8_t update_tx4;
	uint8_t update_tx5;
	uint8_t update_tx6;
	uint8_t update_tx7;
	
	
	aval = 0;
	sval = 0;
	ad2val = 0;
	
	while (1)
	{
		

		
		
		tx_msg.pt_data = &can_buffer_tx[0];
		tx_msg.ctrl.ide = 0;
		tx_msg.dlc = 7;
		tx_msg.id.std = 0x011;
		tx_msg.cmd = CMD_TX_DATA;
		
		
		
		
		
		

		
		//SETUP FOR TRANSMITTING BOARD
		
		analog_setup(ADC_AVCC, 64);
		
		
		
		ad2val = adc_read(ADC2);
		
		uint8_t update_msg;
		update_msg = (ad2val > 0x200);
		

		
		update_tx0 = (!read_gpio(GPIO2) & !read_gpio(GPIO1) & !read_gpio(GPIO0));
		update_tx1 = (!read_gpio(GPIO2) & !read_gpio(GPIO1) &  read_gpio(GPIO0));
		update_tx2 = (!read_gpio(GPIO2) &  read_gpio(GPIO1) & !read_gpio(GPIO0));
		update_tx3 = (!read_gpio(GPIO2) &  read_gpio(GPIO1) &  read_gpio(GPIO0));
		update_tx4 = ( read_gpio(GPIO2) & !read_gpio(GPIO1) & !read_gpio(GPIO0));
		update_tx5 = ( read_gpio(GPIO2) & !read_gpio(GPIO1) &  read_gpio(GPIO0));
		update_tx6 = ( read_gpio(GPIO2) &  read_gpio(GPIO1) & !read_gpio(GPIO0));
		update_tx7 = ( read_gpio(GPIO2) &  read_gpio(GPIO1) &  read_gpio(GPIO0));
		
		if (update_msg){
			aval = adc_read(ADC1);
			sval = aval & 0xff;
			if(update_tx0){
				can_buffer_tx[0] = sval;
			}else if(update_tx1){
			can_buffer_tx[1] = sval;
			}else if(update_tx2){
			can_buffer_tx[2] = sval;
			}else if(update_tx3){
			can_buffer_tx[3] = sval;
			}else if(update_tx4){
			can_buffer_tx[4] = sval;
			}else if(update_tx5){
			can_buffer_tx[5] = sval;
			}else if(update_tx6){
			can_buffer_tx[6] = sval;
			}else if(update_tx7){
			can_buffer_tx[7] = sval;
			}
		}
		
		
		
		
		sprintf(temp_buff, "sval: %x", sval);
		puts(temp_buff);
		
		
		
		
		while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
	
		while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);

		
		for(int i = 0; i <8; i++){
			uint8_t can_msg;
			can_msg = can_buffer_tx[i];
			sprintf(temp_buff, "message %d: %x", i, can_msg);
			puts(temp_buff);
		}
		
		
		_delay_ms(5000);

		
		


		
		}
	}
	
	