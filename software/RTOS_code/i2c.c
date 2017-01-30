#include "i2c.h"
#include "lcd.h"
#include "params.h"

// RTOS includes
#include "atom.h"
#include "atomqueue.h"
#include "atomutils.h"

// Atmel ASF includes
#include "atmel/TWI_Master.h"

/*
 * This mutex is used to prevent reseource collisions when
 * mutliple threads attempt to read from the i2c bus.
 */
ATOM_MUTEX i2c_mutex;

void i2c_init(void) {
	// instantiate i2c mutex
	atomMutexCreate(&i2c_mutex);
	
	// initialize i2c controller
	TWI_Master_Initialise();
	I2C_ADDR_AMS[0] = 0x10;
	I2C_ADDR_AMS[1] = 0x11;
	I2C_ADDR_AMS[2] = 0x12;
	I2C_ADDR_AMS[3] = 0x13;
	I2C_ADDR_AMS[4] = 0x14;
	I2C_ADDR_AMS[5] = 0x15;
	I2C_ADDR_AMS[6] = 0x16;
	I2C_ADDR_AMS[7] = 0x17;//not a real board
}


// Get Charger Detect Signal
void get_detect(void) {
	//holder for expander response (a spot for address, a sport for 1 byte)
	unsigned char expander_data[2] = {0x00,0x00};
	unsigned char msg[2]={I2C_ADDR_HVEXP_R,0x00};//address expander for read
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 2);
	
	// get the response from the expander
	TWI_Get_Data_From_Transceiver((unsigned char*)&expander_data, 2);
	// release mutex
	atomMutexPut(&i2c_mutex);
	if((expander_data[1]&0x01) == (0x01)){
		CHRG_DET = false;
	}
	else{
		CHRG_DET = true;
	}
	if((expander_data[1]&0x08) == (0x08)){
		LOW_DET = false;
	}
	else{
		LOW_DET = true;
	}
}

//Initialize the ADC that the TSV current sensor data is acquired through
void adc_init(void){
	//adc address, point to config reg, MSB config, LSB config (pg11 and pg18)
	unsigned char msg[4] = {I2C_ADDR_HVADC_W, 0x01, 0x82, 0x83};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 4);
	
	//adc address, point to conversion register
	unsigned char msg2[2] = {I2C_ADDR_HVADC_W, 0x00};
	
	//send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg2[0], 2);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

//Initialize the INA226 that pack voltage and charge current is acquired from
void power_init(void){
	//SLA address, point to config reg, MSB config, LSB config
	unsigned char msg[4] = {I2C_ADDR_HVINA226_W, INA226_CONFIG, 0x45, 0x27};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 4);
	
	//adc address, point to shunt voltage conversion register
	unsigned char msg2[2] = {I2C_ADDR_HVINA226_W, INA226_SHUNTV};
	v_select = shunt;
	
	//send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg2[0], 2);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
	
}

//Retrieve the Shunt voltage value 
int16_t get_shunt_voltage(void){
	
	//if(v_select != shunt){
		select_shunt_voltage();
	//}

	//INA226 address
	unsigned char msg[3] = {I2C_ADDR_HVINA226_R, 0x00, 0x00};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 3);
	
	//INA226 address, two slots for data
	unsigned char shunt[3] = {I2C_ADDR_HVINA226_R, 0x00, 0x00};
	
	//send data on i2c bus
	TWI_Get_Data_From_Transceiver((unsigned char*)&shunt[0], 3);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
	int16_t output = (shunt[1]<<8) + shunt[2];
	return output;
}

//Retrieve the pack voltage value 
int16_t get_pack_voltage(void){
	
	//if(v_select != pack){
		select_pack_voltage();
	//}

	//INA226 address
	unsigned char msg[3] = {I2C_ADDR_HVINA226_R, 0x00, 0x00};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 3);
	
	//INA226 address, two slots for data
	unsigned char pack[3] = {I2C_ADDR_HVINA226_R, 0x00, 0x00};
	
	//send data on i2c bus
	TWI_Get_Data_From_Transceiver((unsigned char*)&pack[0], 3);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
	int16_t output = (pack[1]<<8) + pack[2];
	return output;
}

void select_shunt_voltage(void){
	//INA226 address
	unsigned char msg[2] = {I2C_ADDR_HVINA226_W, INA226_SHUNTV};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 2);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

void select_pack_voltage(void){
	//INA226 address
	unsigned char msg[2] = {I2C_ADDR_HVINA226_W, INA226_VBUS};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 2);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

//Retrieve the ADC value for the current sensor
int16_t get_adc_current(void){
	//adc address
	unsigned char msg[3] = {I2C_ADDR_HVADC_R, 0x00, 0x00};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 3);
	
	//adc address, point to conversion register
	unsigned char adc_value[3] = {I2C_ADDR_HVADC_R, 0x00, 0x00};
	
	//send data on i2c bus
	TWI_Get_Data_From_Transceiver((unsigned char*)&adc_value[0], 3);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
	int16_t output = (adc_value[1]<<8) + adc_value[2];
	return output;
}


//Turn ON Power to the Ametes Current Sensor
void current_sensor_on(void){
	unsigned char msg[2]={I2C_ADDR_HVEXP_W,0xFB};//address expander for write
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 2);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

//Turn OFF Power to the Ametes Current Sensor
void current_sensor_off(void){
	unsigned char msg[2]={I2C_ADDR_HVEXP_W,0xFF};//address expander for write
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 2);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

//Initialize the LCD screen
void lcd_init(void){
	
	//wait after start up to ensure LCD is ready (per datasheet)
	atomTimerDelay(40); 

	//setup the display for menu top level 
	//display_top();
	//message to set to 4 bit mode (w/ Enable High to write data)
	unsigned char msg[2] = {I2C_ADDR_LCD,0X2C};
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 2);
	
	//Enable low to complete transmission
	msg[1] = EN_LOW;
	
	//send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 2);
	
	
	
	// release mutex
	atomMutexPut(&i2c_mutex);
	lcd_reset();
}

void lcd_reset(void){
	//sets number of display lines
	lcd_transmit(0x28, 0, 0);
	// Turn on Display and Cursor
	lcd_transmit(0x0E, 0, 0);
	// Set entry mode to increment address and cursor after write
	lcd_transmit(0x06, 0, 0);
	
}

//Transmit a whole message to the LCD screen. Writes to Data Register
void lcd_message(unsigned char* message, uint8_t width){
	uint8_t i = 0;
	for(i = 0; i < width; i=i+1){
		lcd_transmit(message[i], 1, 0);
	}
}

//Transmits a byte to the LCD screen
void lcd_transmit(uint8_t data, uint8_t rs, uint8_t rw){
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	//message that will be sent, start with SLA
	unsigned char msg[5] = {I2C_ADDR_LCD,0X00,0x00,0x00,0x00};
	
	//high nibble
	msg[1] = (data & 0xF0) | (0x0C) | (rs) | (rw<<1);
	
	//Enable low to clock high nibble to LCD
	msg[2] = EN_LOW;
	
	//low nibble
	msg[3] = ((data & 0x0F)<<4) | (0x0C) | (rs) | (rw<<1);
	
	//Enable low to complete transmission
	msg[4] = EN_LOW;
	
	//send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], 5);
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

/*  This function is not used in the current version of PacMan software.  If it is used, it will need to be debugged.
// Reset All AMS Boards
void ams_reset_all(void) {
	// FIXME this won't work with charge detect?
	unsigned char msg_set[2] = {
			I2C_ADDR_HVGPIO,
			0xFF,
			};
	
	// set AMS reset signal
	TWI_Start_Transceiver_With_Data(&msg_set[0], sizeof(msg_set));
	
	// wait for reset to propagate
	atom_delay_ms(100);
	
	unsigned char msg_clr[2] = {
			I2C_ADDR_HVGPIO,
			0x00};
	
	// clear AMS reset signal
	TWI_Start_Transceiver_With_Data(&msg_clr[0], sizeof(msg_clr));
}*/


// Clear AMS Bypass Mode
void ams_reset_bypass(uint8_t addr) {
	unsigned char msg[2] = {
			(addr << 1),  // device address (write mode)
			RESET_BY,     // device command
			};
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], sizeof(msg));
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}


// Set AMS to Bypass Mode
void ams_set_bypass(uint8_t addr) {
	unsigned char msg[2] = {
			(addr << 1),  // device address (write mode)
			SET_BY,       // device command
			};
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], sizeof(msg));
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

/*  These functions are not used in the current version of PacMan software.  If they are used, they will need to be debugged.
	The default bypass time in ams firmware is sufficient and panic is an unused condition.
	
// Set AMS Panic Mode
void ams_panic(uint8_t addr) {
	unsigned char msg[2] = {
			(addr << 1),  // device address (write mode)
			PANIC,        // device command
			};
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], sizeof(msg));
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}


// Clear AMS Panic Mode
void ams_relax(uint8_t addr) {
	unsigned char msg[2] = {
			(addr << 1),  // device address (write mode)
			RELAX,        // device command
			};
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], sizeof(msg));
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

//DO NOT USE THIS FUNCTION!  IT WILL CORRUPT AMS EEPROM!!!!!!!!
// Set AMS Bypass Counter
void ams_set_bypass_time(uint8_t addr, uint16_t count) {
	unsigned char msg[4] = {
			(addr << 1),  // device address (write mode)
			SET_BYTIME,   // device command
			count >> 8,   // upper 8 bits
			count & 0xFF, // lower 8 bits
			};   // device command
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// send data on i2c bus
	TWI_Start_Transceiver_With_Data(&msg[0], sizeof(msg));
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}*/

//Get Battery Cell Status from AMS
void ams_cell_status(uint8_t addr) {
	// device address (read mode)
	unsigned char msg[5] = {(addr << 1) | 1,0x00,0x00,0x00,0x00};
	
	// take mutex, wait until available
	atomMutexGet(&i2c_mutex, 0);
	
	// read all 4 bytes from i2c bus
	// upper 16-bits are voltage reading
	// lower 16-bits are temperature reading
	TWI_Start_Transceiver_With_Data((unsigned char*)&msg, 5);
	TWI_Get_Data_From_Transceiver((unsigned char*)&msg, 5);
	ams_voltage = (msg[3]<<8);
	ams_voltage += (msg[4]);
	ams_temp = (msg[1]<<8);
	ams_temp += msg[2];
	
	// release mutex
	atomMutexPut(&i2c_mutex);
}

