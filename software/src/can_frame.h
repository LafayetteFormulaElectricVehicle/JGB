#ifndef CAN_FRAME_H
#define CAN_FRAME_H

uint8_t rvalue;

void set_ext_slot(uint8_t* frame, uint8_t slot, uint16_t value);

void set_ext_bit(uint8_t* frame, uint8_t slot, uint8_t bit, uint16_t value);

uint8_t get_ext_bit(uint8_t* frame, uint8_t slot, uint8_t bit);

void set_std_slot(uint8_t* frame, uint8_t slot, uint16_t value);

void set_std_bit(uint8_t* frame, uint8_t slot, uint8_t bit, uint16_t value);

uint8_t get_std_slot(uint8_t* frame, uint8_t slot, uint8_t value);

uint8_t get_std_bit(uint8_t* frame, uint8_t slot, uint8_t bit);


#endif          
