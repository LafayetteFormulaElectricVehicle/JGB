#include "task_gui.h"
#include "lcd.h"
#include "atom.h"
#include "atomport.h"

void task_gui_init(){
	display_top();
}

void task_gui(uint32_t data) {
	//uint8_t i = 0;
	task_gui_init();
	int8_t screen = 0;
	for(;;){
		switch(screen){
			case 0:
				display_top();
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				display_cell(screen-1);	
				break;
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				display_cell_cal(screen-8);	
				break;
			default:
				display_top();
		}
		if(button_down){
			screen = screen + 1;
			if(screen ==8){
				screen = 7;
			}
			button_down = false;
		}
		if(button_up){
			screen = screen - 1;
			if(screen ==-1){
				screen = 0;
			}
			button_up = false;
		}
		
	}
}
