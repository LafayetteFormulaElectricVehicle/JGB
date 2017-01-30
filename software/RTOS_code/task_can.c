#include "task_can.h"

void task_can_init(void){
	uint8_t i = 0;
	for(i = 0; i<7; i = i + 1){
		can_buff[i] = 0x00;//lets make a nice clean buffer
	}
	can_frame.dlc = 7;
	can_frame.pt_data = (uint8_t*)&can_buff;
	can_frame.cmd = CMD_TX; //This frame is for transmission only.  Make another for receiving if needed
	//can_frame_ctrl.rtr = false;
	//can_frame_ctrl.ide = false;
	can_frame.ctrl.ide = 0;
	//can_frame_id.std = CANADD_PACKINFO;
	can_frame.id.std = CANADD_PACKINFO1;
}

void task_can(uint32_t data){
	task_can_init();
	can_init(0);
	uint8_t i = 0;
	uint32_t system_time = 0;
	for(;;){
		can_buff[0] = pack_state;
		can_buff[1] = pack_voltage >> 8;
		can_buff[2] = pack_voltage & 0xFF;
		can_buff[3] = pack_current >> 24 & 0xFF;
		can_buff[4] = pack_current >> 16 & 0xFF;
		can_buff[5] = pack_current >> 8 & 0xFF;
		can_buff[6] = pack_current & 0xFF;
		can_buff[7] = pack_SOC;
		can_frame.dlc = 8;
		can_frame.id.std = CANADD_PACKINFO1;
		while(can_cmd(&can_frame) != CAN_CMD_ACCEPTED){
		}
		while(can_get_status(&can_frame) == CAN_STATUS_NOT_COMPLETED);
		//atomTimerDelay(10);
		
		system_time = atomTimeGet();
		can_buff[0] = system_time>>24 & 0xFF;
		can_buff[1] = system_time>>16 & 0xFF;
		can_buff[2] = system_time>>8 & 0xFF;
		can_buff[3] = system_time & 0xFF;
		can_buff[4] = pack_coulombs>>24 & 0xFF;
		can_buff[5] = pack_coulombs>>16 & 0xFF;
		can_buff[6] = pack_coulombs>>8 & 0xFF;
		can_buff[7] = pack_coulombs & 0xFF;
		can_frame.dlc = 8;
		can_frame.id.std = CANADD_PACKINFO2;
		while(can_cmd(&can_frame) != CAN_CMD_ACCEPTED){
		}
		while(can_get_status(&can_frame) == CAN_STATUS_NOT_COMPLETED);
		//atomTimerDelay(10);
		
		for(i = 0; i < 7; i = i +1){
			can_buff[0] = i;
			can_buff[1] = cell_status[i];
			can_buff[2] = cell_V[i] >> 8;
			can_buff[3] = cell_V[i] & 0xFF;
			can_buff[4] = cell_T[i] >> 8;
			can_buff[5] = cell_T[i] & 0xFF;
			can_buff[6] = 0;
			can_frame.dlc = 6;
			can_frame.id.std = CANADD_CELLINFO;
			while(can_cmd(&can_frame) != CAN_CMD_ACCEPTED){
			}
			while(can_get_status(&can_frame) == CAN_STATUS_NOT_COMPLETED);
			//atomTimerDelay(10);
		}
	}
}
