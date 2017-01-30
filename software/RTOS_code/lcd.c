#include "lcd.h"
#include "i2c.h"
#include <string.h>
#include "params.h"
#include "atomtimer.h"


void write_lcd(void){
	lcd_transmit(0x80,0,0);
	lcd_reset();
	lcd_message((unsigned char*)&display_0, 20);
	lcd_message((unsigned char*)&display_1, 20);
	lcd_message((unsigned char*)&display_2, 20);
	lcd_message((unsigned char*)&display_3, 20);
}

void display_top(void){
	unsigned char line0[20] = {'S','T','A','T','E',':',' ',' ',' ',' ',' ',' ','S','O','C',':',' ',' ',' ','%'};
	unsigned char line1[20] = {'V',':',' ',' ',' ',' ',' ','A',':',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','A','F','E','T','Y',' ','L','O','O','P',':',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line3[20] = {'T','>',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	char* d = "DEAD";
	switch(pack_state){
	case chrg: d = "CHRG";
		break;
	case chrgd: d = "CHRGD";
		break;
	case lco: d = "LCO";
		break;
	case flt: d = "FLT";
		break;
	case dead: d = "DEAD";
		break;
	case rdy: d = "RDY";
		break;
	case boot: d = "BOOT";
		break;
	}
	
	
	memcpy(line0+6,d, strlen(d));
	
	line3[2] = 'v';
	line3[3] = (int8_t)version+48;
	line3[4] = '.';
	line3[5] = (int8_t)(version*10)%10+48;
	line3[6] = (int8_t)(version*100)%10+48;
	
	
	line0[16] = (pack_SOC/100)+48;
	line0[17] = ((pack_SOC%100)/10)+48;
	line0[18] = (pack_SOC%10)+48;
	
	line1[2] = (int)(pack_voltage)/100 +48;
	line1[3] = ((int)(pack_voltage/10) % 10)+48;
	line1[4] = '.';
	line1[5] = ((int)(pack_voltage) % 10)+48;
	
	int32_t disp_current = pack_current;
	if(pack_current<0.0){
		line1[9] = '-';
		disp_current = disp_current *-1.0;
	}
	line1[10] = (disp_current/100000)%10 + 48;
	line1[11] = (disp_current/10000)%10 + 48;
	line1[12] = (disp_current/1000)%10 +48;
	line1[13] = '.';
	line1[14] = (disp_current/100)%10 +48;
	line1[15] = (disp_current/10)%10 +48;
	line1[16] = disp_current%10 +48;
	
	if(sloop_state == true){
		line2[13] = 'C';
		line2[14] = 'L';
		line2[15] = 'O';
		line2[16] = 'S';
		line2[17] = 'E';
		line2[18] = 'D';
	}
	else{
		line2[13] = 'O';
		line2[14] = 'P';
		line2[15] = 'E';
		line2[16] = 'N';
		line2[17] = ' ';
		line2[18] = ' ';
	}
	
	uint32_t seconds = (atomTimeGet()/100); //100 system ticks per sec
	uint32_t minutes = (seconds/60);
	uint32_t hours = minutes/60;
	uint32_t days = hours/24;
	seconds = seconds%60;
	minutes = minutes%60;
	hours = hours%24;
	line3[19]= seconds%10+48;
	line3[18]= (seconds/10)%10+48;
	line3[17]= ':';
	line3[16]= (minutes)%10+48;
	line3[15]= (minutes/10)%10+48;
	line3[14]= ':';
	line3[13]= (hours)%10+48;
	line3[12]= (hours/10)%10+48;
	line3[11]= ':';
	line3[10]= (days)%10+48;
	line3[9]=  (days/10)%10+48;
	line3[8]=  (days/100)%10+48;
	
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_chst(void){
	unsigned char line0[20] = {'L','A','S','T',' ','D','I','S','C','H','R','G','%',':',' ',' ',' ',' ',' ',' '};
	unsigned char line1[20] = {'L','A','S','T',' ','C','H','R','G','%',':',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'C','U','R','R','E','N','T',' ','C','H','R','G','%',':',' ',' ',' ',' ',' ',' '};
	unsigned char line3[20] = {'T','/','C','H','S','T','>',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cal1(void){
	unsigned char line0[20] = {'V','O','F','F',':',' ',' ',' ',' ',' ','A','O','F','F',':',' ',' ',' ',' ',' '};
	unsigned char line1[20] = {'V','S','L','P',':',' ',' ',' ',' ',' ','A','S','L','P',':',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line3[20] = {'T','/','C','A','L','>',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	if(pack_Voff <0.0){
		line0[5] = '-';
	}
	line0[6] = pack_Voff/1+48;
	line0[7] = '.';
	line0[8] = (int16_t)(pack_Voff*10)%10+48;
	
//	lin0[15] = 
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cal1_voff(void){
	unsigned char line0[20] = {'V','O','F','F',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C','A','L','1','/','V','O','F','F','>',' ',' ',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cal1_vslp(void){
	unsigned char line0[20] = {'V','S','L','P',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C','A','L','1','/','V','S','L','P','>',' ',' ',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cal1_aoff(void){
	unsigned char line0[20] = {'A','O','F','F',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C','A','L','1','/','A','O','F','F','>',' ',' ',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cal1_aslp(void){
	unsigned char line0[20] = {'A','S','L','P',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C','A','L','1','/','A','S','L','P','>',' ',' ',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cal2(void){
	unsigned char line0[20] = {'T','H','I','S',' ','S','C','R','E','E','N',' ','W','I','L','L',' ',' ',' ',' '};
	unsigned char line1[20] = {'A','L','L','O','W',' ','C','E','L','L',' ','M','O','D','E','L',' ',' ',' ',' '};
	unsigned char line2[20] = {'P','A','R','A','M',' ','F','O','R',' ','S','O','C',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line3[20] = {'T','/','C','A','L','2','>',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cell(uint8_t cell){
	unsigned char line0[20] = {'S','T','A','T','E',':',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line1[20] = {'C',cell+48,'_','V',':',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'C',cell+48,'_','T',':',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line3[20] = {'T','/','C',cell+48,'>',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

	line1[5] = cell_V[cell]/1000 + 48;
	line1[6] = '.';
	line1[7] = (cell_V[cell]%1000)/100 + 48;
	line1[8] = (cell_V[cell]%1000)%100/10 +48;
	line1[9] = cell_V[cell] %10 + 48;

	line2[5] = cell_T[cell]/1000 + 48;
	line2[6] = (cell_T[cell]%1000)/100 + 48;
	line2[7] = (cell_T[cell]%1000)%100/10 +48;
	line2[8] = '.';
	line2[9] = cell_T[cell]%10 +48;
	
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cell_cal(uint8_t cell){
	unsigned char line0[20] = {'V','O','F','F',':',' ',' ',' ',' ',' ','T','O','F','F',':',' ',' ',' ',' ',' '};
	unsigned char line1[20] = {'V','S','L','P',':',' ',' ',' ',' ',' ','T','S','L','P',':',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line3[20] = {'T','/','C',cell+48,'/','C','A','L','>',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cell_cal_voff(uint8_t cell){
	unsigned char line0[20] = {'V','O','F','F',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C',cell+48,'/','C','A','L','/','V','O','F','F','>',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cell_cal_vslp(uint8_t cell){
	unsigned char line0[20] = {'V','S','L','P',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C',cell+48,'/','C','A','L','/','V','S','L','P','>',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cell_cal_toff(uint8_t cell){
	unsigned char line0[20] = {'T','O','F','F',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C',cell+48,'/','C','A','L','/','T','O','F','F','>',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}

void display_cell_cal_tslp(uint8_t cell){
	unsigned char line0[20] = {'T','S','L','P',':',' ',' ',' ',' ',' ','C','U','R','R','E','N','T',' ',' ',' '};
	unsigned char line1[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','N','E','W',' ',' ',' ',' ',' ',' ',' '};
	unsigned char line2[20] = {'S','E','L','E','C','T',' ','D','I','G','I','T',' ','O','R',' ','O','K',' ',' '};
	unsigned char line3[20] = {'T','/','C',cell+48,'/','C','A','L','/','T','S','L','P','>',' ',' ',' ',' ',' ',' '};
	memcpy((void*)display_0, (void*) line0, 20);
	memcpy((void*)display_1, (void*) line1, 20);
	memcpy((void*)display_2, (void*) line2, 20);
	memcpy((void*)display_3, (void*) line3, 20);
}




