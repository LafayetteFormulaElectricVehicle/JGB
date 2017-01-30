#include "task_i2c.h"
#include "params.h"
#include "i2c.h"
#include "atom.h"

void bypass_selection(void){
	uint8_t sorted_V[7];
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t temp = 0;
	uint8_t bypass_divider = 7;
	if(pack_state==chrg && bypass_flag == true){
		bypass_flag = false;
		for(i = 0 ; i <7; i = i+1){
			sorted_V[i] = i;
		}			

		//created a sorted list of indexes of cell voltage low to high
		for(i = 0; i < 6 ; i = i+1){//CS double major may want to merge sort or quicksort...
			for(j = 0; j < 6; j = j+1){
				if(cell_V[sorted_V[j]]>cell_V[sorted_V[j+1]]){
					temp = sorted_V[j+1];
					sorted_V[j+1] = sorted_V[j];
					sorted_V[j] = temp;
				}
			}
		}
			
		bypass_divider = 7;
	
		//find the lowest gap of 50 mV or more
		for(i = 6; i >0; i = i-1){	
			if(((cell_V[sorted_V[i]])-(cell_V[sorted_V[i-1]]))>= 25){
				bypass_divider = i;
			}
		}
			
	//set all cells above that to bypass
		for(i = 0; i < 7; i = i + 1){
			if(i>=bypass_divider){
				cell_status[sorted_V[i]] = 1;
			}else{
				cell_status[sorted_V[i]] = 0;
			}
		}
	}else{
		 if(pack_state != chrg){
			for(i = 0 ; i <7; i = i+1){//set all cells to not bypass
				cell_status[i] = 0;	
			}
		}
	}
}

void task_i2c(uint32_t data) {
	i2c_init();
	adc_init();
	power_init();
	lcd_init();
	uint8_t i = 0;
	lcd_flag = true;
	bypass_flag = false;
	for(;;) {
		
		//write to LCD once a second.
		if(atomTimeGet()%100>50){
			if(lcd_flag){
				lcd_flag = false;
				bypass_flag = true;
				write_lcd();
			}
		}else{
			lcd_flag = true;
		}
		
		for(i=0;i<7;i=i+1){
			ams_cell_status(I2C_ADDR_AMS[i]);
			cell_V[i] = (uint16_t)(ams_voltage*2*3*cell_Vslp[i]+cell_Voff[i]); //2 mV per bit in AMS ADC, voltage divided by 3 for ADC input, stored result is mV
			cell_T[i] = (uint16_t)((ams_temp*2-500)*cell_Tslp[i]+cell_Toff[i]); //2mV per bit in AMS ADC, 500mV = 0 *C, stored result is tenths of a degree
			//atomTimerDelay(2);
		}
		
		pack_voltage = (uint16_t)(10.0*(get_pack_voltage()*0.00125*pack_Vslp+pack_Voff));
		get_detect();//CHRG_DET and LOW_DET values are set by this
		
		adc_value = (int32_t)get_shunt_voltage()*-1;
		if(adc_value < 12000 && adc_value >-12000){//a sensible number from successful adc and i2c (+/-30amps)
			shunt_voltage = (int16_t)adc_value;
		}
		
		
		if(pack_state == rdy && sloop_state == true){
			current_sensor_on();
			adc_value = (int32_t)get_adc_current();
			atomMutexGet(&A_mutex,0);
			//float pt calculation of A,  stored as 16 bit int in hundredths of amps
			pack_current = (int32_t)(1000.0*(-1*adc_value*0.000125*pack_ametes_Aslp+pack_ametes_Aoff));
			//not using shunt current as the error in ametes current is of the same magnitude
			// /* .125 mV/bit*/+(((shunt_voltage * 0.0000025)/0.001)*pack_INA226_Aslp+pack_INA226_Aoff));// 2.5 uV per bit and 1 mOhm shunt
			atomMutexPut(&A_mutex);
		}else{
			current_sensor_off();
			adc_value = (int32_t)get_adc_current();
			if(adc_value<700 && adc_value>-700){//a sensible number from a good adc and i2c message
				atomMutexGet(&A_mutex,0);
				//float pt calculation of A,  stored as 16 bit int in hundredths of amps
				pack_current = (int32_t)(1000.0*(((shunt_voltage * 0.0000025)/0.001)*pack_INA226_Aslp+pack_INA226_Aoff));// 2.5 uV per bit and 1 mOhm shunt
				atomMutexPut(&A_mutex);
			}
		}
		
		

		bypass_selection();

		for(i = 0 ; i < 7; i = i +1){
			if(cell_status[i] == 1){
				ams_set_bypass(I2C_ADDR_AMS[i]);
			}
			else{	
				ams_reset_bypass(I2C_ADDR_AMS[i]);
			}
		}
		
		
	}
}
