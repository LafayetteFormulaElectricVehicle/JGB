#include <stdint.h>

#include <stdint.h>

#include "can_frame.h"

void set_ext_slot(uint8_t* frame, uint8_t slot, uint16_t value) {
	slot *= 2;
	if(slot < 8) {
		frame[slot] = value >> 8;
		frame[slot + 1] = value;
	}
}

void set_ext_bit(uint8_t* frame, uint8_t slot, uint8_t bit, uint16_t value) {
	//16-bit multiply slot by 2
	slot *= 2;
	if(slot < 8) {
		uint16_t slot_value = (frame[slot] << 8) | frame[slot+1];
		slot_value &= ~(1 << bit);
		slot_value |= (value << bit)
		frame[slot*2] = slot_value;
	}
}

uint8_t get_ext_slot(uint8_t* frame, uint8_t slot, uint8_t value) {
	slot *= 2;
	if(slot < 8) {
		return rvalue = (frame[slot] << 8) | (frame[slot + 1]);
	}
	return 0;
}


uint8_t get_ext_bit(uint8_t* frame, uint8_t slot, uint8_t bit) {
	slot *= 2;
	if(slot < 8) {
		return rvalue = ((frame[slot] << 8) | (frame[slot + 1])) & (1 << bit);
	}
	return 0;
}

void set_std_slot(uint8_t* frame, uint8_t slot, uint16_t value) {
	if(slot < 8) {
		frame[slot] = value;
	}
}

void set_std_bit(uint8_t* frame, uint8_t slot, uint8_t bit, uint16_t value) {
	//16-bit multiply slot by 2
	if(slot < 8) {
		uint8_t slot_value = frame[slot];
		slot_value &= ~(1 << bit);
		slot_value |= (value << bit)
		frame[slot*2] = slot_value;
	}
}

uint8_t get_std_slot(uint8_t* frame, uint8_t slot, uint8_t value) {
	if(slot < 8) {
		return rvalue = frame[slot];
	}
	return 0;
}


uint8_t get_std_bit(uint8_t* frame, uint8_t slot, uint8_t bit) {
	if(slot < 8) {
		return rvalue = (frame[slot] << 8) & (1 << bit);
	}
	return 0;
}



