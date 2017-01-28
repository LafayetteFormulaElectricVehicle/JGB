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

int main(){
	sei();
	fdevopen(&uart_putc, NULL);
	fdevopen(&uart_getc, NULL);
	dac_enable();
	char str [] = "Hello world";
	puts(str);
	while(1){
		_delay_ms(1000);
		strcpy(str, "High");
		puts(str);
		dac_write(0x2FF);
		_delay_ms(1000);
		strcpy(str, "Low");
		puts(str);
		dac_write(0x000);
	}
		

}
