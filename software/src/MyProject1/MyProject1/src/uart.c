#include "uart.h"

#include <stdbool.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "ringbuf.h"

#include "config.h"
//NOTE:
//Useful Code Reference http://www.avrfreaks.net/forum/usart-interrupt-atmega32m1

/************************************************************************
 * Written by: John Gehrig
 * Modified by: Raji Birru on 04/06/2017                                                                     */
/************************************************************************/

volatile struct ringbuffer tx_buffer = {{0}, 0, 0, true};
volatile struct ringbuffer rx_buffer = {{0}, 0, 0, true};

//Receive interrupt routine
ISR(LIN_TC_vect) {
	if(LINSIR & (1 << LRXOK)) {
		//clear LRXOK
		LINSIR &= ~(1 << LRXOK);
		
		//write buffer
		uint8_t recv = LINDAT;
		buffer_write(&tx_buffer, recv);	
	}
	if(LINSIR & (1 << LTXOK)) {
		//clear LTXOK
		//LINSIR &= ~(1 << LTXOK);
		uint8_t data = buffer_read(&tx_buffer); 
		LINDAT = data;
		
		if(buffer_empty(&tx_buffer)) {
			LINENIR  &= ~(1 << LENTXOK);
		}
	}
}


void uart_init(uint16_t baud){
	//Reset LIN/UART hardware
	LINCR = (1 << LSWRES);
	
	//Calculate, and set UART baud rate
	LINBRRH = (((F_CPU / baud) / 16) - 1) >> 8;
	LINBRRL = (((F_CPU / baud) / 16) - 1);
	
	//Bit sampling rate/re-sync
	LINBTR = (1 << LDISR) | (16 << LBT0);

	//RX interrupt
	LINENIR = (1 << LENRXOK);

	//Enable UART RX & TX
	LINCR = (1 << LENA) | (1 << LCMD2) | (1 << LCMD1) | (1 << LCMD0);

	sei();
}
			
int uart_putc(char data, FILE* file) {
	cli();
	//the the buffer was empty, enable tx interrupt
	if(!(LINENIR & (1 << LENTXOK))) {
		LINENIR |= (1 << LENTXOK);
		LINDAT = data;
	}
	else {
		//add data to buffer
		buffer_write(&tx_buffer, data);
	}
	sei();
	return data;
}

uint8_t uart_getc(FILE* file) {
	cli();
	//while(buffer_empty(&rx_buffer)); 
	return buffer_read(&rx_buffer);
	sei();
}