#include <stdbool.h>
#include <stdint.h>

#include "ringbuf.h"

void buffer_write(struct ringbuffer* buffer, uint8_t byte) {
	if(buffer->empty) buffer->empty = false;
	else if(buffer->head == buffer->tail) {
		buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
	}
	
	buffer->buffer[buffer->head] = byte;
	
	buffer->head = (buffer->head + 1) % BUFFER_SIZE;
}

uint8_t buffer_read(struct ringbuffer* buffer) {
	if(!(buffer->empty)) {
		uint8_t byte = buffer->buffer[buffer->tail];
		buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
		buffer->empty = (buffer->tail == buffer->head);
		return byte;
	}
	return 0;
}

void buffer_write_str(struct ringbuffer* buffer, char* str) {
	for(int i=0;str[i] != '\0';i++) {
		buffer_write(buffer, str[i]);
	}
}
	
void buffer_read_str(struct ringbuffer* buffer, char* str) {
	int i = 0;
	while(!buffer_empty(buffer)) {
		str[i] = buffer_read(buffer);
	}
	str[i] = '\0';
}

bool buffer_full(struct ringbuffer* buffer) {
	return ((buffer->head == buffer->tail) && !(buffer->empty));
}


bool buffer_empty(struct ringbuffer* buffer) {
	return buffer->empty;
}
