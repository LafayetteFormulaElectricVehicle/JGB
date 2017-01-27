#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

//Check for BUFFER_SIZE
//#ifndef BUFFER_SIZE
//#	error "BUFFER_SIZE" must be defined!
//#endif

//Circular serial buffer
struct ringbuffer {
	uint8_t buffer[BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
	bool empty;
};

void buffer_write(struct ringbuffer*, uint8_t);

uint8_t buffer_read(struct ringbuffer*);

void buffer_write_str(struct ringbuffer*, char*);

void buffer_read_str(struct ringbuffer*, char*);

bool buffer_full(struct ringbuffer*);

bool buffer_empty(struct ringbuffer*);
#endif
