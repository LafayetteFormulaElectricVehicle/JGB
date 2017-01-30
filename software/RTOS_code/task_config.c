#include "task_config.h"
#include "params.h"

#include "atom.h"

//config sets initial state and SOC and monitors params for state change
void task_config(uint32_t data) {
	cell_Voff[0] = 6;
	cell_Voff[1] = 4;
	cell_Voff[2] = 12;
	cell_Voff[3] = -7;
	cell_Voff[4] = -27;
	cell_Voff[5] = -7;
	cell_Voff[6] = -8;
	cell_Vslp[0] = 1;
	cell_Vslp[1] = 0.989;
	cell_Vslp[2] = 1.001;
	cell_Vslp[3] = 1.001;
	cell_Vslp[4] = 1.008;
	cell_Vslp[5] = 0.996;
	cell_Vslp[6] = 1.014;
	cell_Toff[0] = 62;
	cell_Toff[1] = 70;
	cell_Toff[2] = 43;
	cell_Toff[3] = 57;
	cell_Toff[4] = 32;
	cell_Toff[5] = 49;
	cell_Toff[6] = 30;
	cell_Tslp[0] = 0.749;
	cell_Tslp[1] = 0.699;
	cell_Tslp[2] = 0.835;
	cell_Tslp[3] = 0.747;
	cell_Tslp[4] = 0.868;
	cell_Tslp[5] = 0.786;
	cell_Tslp[6] = 0.894;
	
	pack_state = boot;
	pack_coulombs = (uint32_t)(MAX_COULOMBS/2);//50% 
	pack_SOC= 50;//(uint16_t)(10000.0 * pack_coulombs / ((float)MAX_COULOMBS));
	uint8_t i = 0;
	for(;;) {
		//Detect Fault in all states
		for(i = 0; i<7; i = i+1){//if communication with an AMS board is lost, these values are out of range
			if(cell_T[i] > 600){//60.0 deg C cell temp
				pack_state = flt;
			}
			if(cell_V[i] > 4000){// || cell_V[i] <2000){//>4000 mV or <2000 mV cell voltage
				pack_state = flt;
			}
		}
		if(pack_voltage > 20800){//20800 * .00125 mV/bit = 26 V
			pack_state = flt;
		}
		if(pack_state != flt){//then determine the proper state
			switch(pack_state){
				case boot://we've booted and no fault
					atomTimerDelay(200);//wait one sec before ready
					pack_state = rdy;
					break; 
				case rdy:
					if(pack_SOC <1){//001.00 %
						pack_state = dead;
					}else if(CHRG_DET){
						pack_state = chrg;
					}else if(LOW_DET){
						pack_state = lco;
					}
					break;
				case dead:
					if(CHRG_DET){
						pack_state = chrg;
					}
					break;
				case chrg:
					if(!CHRG_DET){
						pack_state = rdy;
					}else if(pack_SOC>=100){//100.00%
						pack_state = chrgd;
					}
					break;
				case chrgd:
					if(!CHRG_DET){
						pack_state = rdy;
					}
					break;
				case lco:
					if(!LOW_DET){
						pack_state = rdy;
					}else if(pack_SOC < 1){//001.00%
						pack_state = dead;
					}
					break;
				default:
					pack_state = flt;
					break;
			}
		}else{//check if the fault is cleared
			State temp = rdy;
			for(i = 0; i<7; i = i+1){//if communication with an AMS board is lost, these values are out of range
				if(cell_T[i] > 600){//60.0 deg C cell temp
					temp = flt;
				}
				if(cell_V[i] > 4000){// || cell_V[i] <2000){//>4000 mV or <2000 mV cell voltage
					temp = flt;
				}
			}
			if(pack_voltage > 20800){//20800 * .00125 mV/bit = 26 V
				temp = flt;
			}
			pack_state = temp;
		}
		
		//atomTimerDelay(10);
	}
}
