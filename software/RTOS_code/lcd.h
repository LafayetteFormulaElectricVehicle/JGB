#ifndef LCD_H
#define LCD_H

// standard library includes
#include <stdint.h>
#include <stdio.h>

// local includes
#include "params.h"

uint8_t cell;
const char* Pack_State_Strings[7];
void write_lcd(void);
void display_top(void);
void display_chst(void);
void display_cal1(void);
void display_cal1_voff(void);
void display_cal1_vslp(void);
void display_cal1_aoff(void);
void display_cal1_aslp(void);
void display_cal2(void);
void display_cell(uint8_t cell);
void display_cell_cal(uint8_t cell);
void display_cell_cal_voff(uint8_t cell);
void display_cell_cal_vslp(uint8_t cell);
void display_cell_cal_toff(uint8_t cell);
void display_cell_cal_tslp(uint8_t cell);





#endif
