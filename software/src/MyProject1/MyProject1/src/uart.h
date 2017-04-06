#ifndef UART_H 
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void uart_init(uint16_t baud);

//Transmit a single byte
int uart_putc(char, FILE*);

//Receive a single byte
uint8_t uart_getc(FILE*);

#endif /*LIN_UART_H*/
