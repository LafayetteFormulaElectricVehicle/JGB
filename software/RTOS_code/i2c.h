#ifndef _I2C_H
#define _I2C_H

// standard library includes
#include <stdint.h>

// RTOS includes
#include "atom.h"
#include "atommutex.h"

// Local Includes
#include "config.h"
#include "params.h"
#include "lcd.h"

// local i2c device addresses
#define I2C_ADDR_HVADC		0X48
#define I2C_ADDR_HVGPIO		0x40
#define I2C_ADDR_HVEXP_W	0x70
#define I2C_ADDR_HVEXP_R	0x71
#define I2C_ADDR_HVADC_W	0x90
#define I2C_ADDR_HVADC_R	0x91
#define I2C_ADDR_HVINA226_W	0x88
#define I2C_ADDR_HVINA226_R	0x89
#define I2C_ADDR_LCD		0x40

uint8_t I2C_ADDR_AMS[8];


// Pointer Register Values for INA226 (I2C: SLA + POINTER VALUE + DATA)
#define INA226_CONFIG		0x00
#define INA226_SHUNTV		0x01
#define INA226_VBUS			0x02

// ADS1013/4/5 Config Register
#define ADS_OS          7
#define ADS_MUX2        6
#define ADS_MUX1        5
#define ADS_MUX0        4
#define ADS_MUX         4
#define ADS_PGA2        3
#define ADS_PGA1        2
#define ADS_PGA0        1
#define ADS_PGA         1
#define ADS_MODE        0
#define ADS_DR2         7
#define ADS_DR1         6
#define ADS_DR0         5
#define ADS_DR          5
#define ADS_COMP_MODE   4
#define ADS_COMP_POL    3
#define ADS_COMP_LAT    2
#define ADS_CCMP_QUE1   1
#define ADS_CCMP_QUE0   0
#define ADS_CCMP_QUE    0
#define ADS_128SPS      0b000
#define ADS_250SPS      0b001
#define ADS_490SPS      0b010
#define ADS_920SPS      0b011
#define ADS_1600SPS     0b100 // default sample rate
#define ADS_2400SPS     0b101
#define ADS_3300SPS     0b110


/**
* @brief AMS Commands
* 
* This enum descirbes each of the AMS board commands.
*/
typedef enum {
	/// Reset Bypass
	RESET_BY = 0,
	
	/// Set Bypass
	SET_BY,
	
	/// Set Panic Mode
	PANIC,
	
	/// Clear Panic Mode
	RELAX,
	
	/// Set AMS Address
	SET_ADDR,
	
	/// Set AMS Bypass Timeout Counter
	SET_BYTIME,
} AMS_CMD;

typedef enum{shunt, pack} ina_selection;
ina_selection v_select;
uint16_t ams_voltage;
uint16_t ams_temp;

void current_sensor_on(void);
void current_sensor_off(void);
void adc_init(void);
void power_init(void);
int16_t get_adc_current(void);
int16_t get_shunt_voltage(void);
int16_t get_pack_voltage(void);
void select_shunt_voltage(void);
void select_pack_voltage(void);
void lcd_init(void);
void lcd_reset(void);
void lcd_transmit(uint8_t data, uint8_t rs, uint8_t rw);
void lcd_message(unsigned char* message, uint8_t width);


/**
* @brief Initialize I2C Communications
* 
* This function is used to initialize the i2c hardware
* located on the pacman board.
* 
* This function MUST be called before utilizing any
* functions and should only be called ONCE.
*/
void i2c_init(void);

/**
* @brief Get Charger/LCO Detect Signals
* 
* Sets the appropriate param if the charge connector 
* or lco connector is attached
* to the battery pack.
*
*/
void get_detect(void);


/**
* @brief Reset All AMS Boards
* 
* This command resets all devices connected to the high
* voltage I2C bus. The reset signal is held low for a 
* period of 100ms. This function returns once this
* holding period has passed.
*/
//void ams_reset_all(void);


/**
* @brief Clear AMS Bypass Mode
*
* @param addr AMS board address
* 
* This command clears bypass mode for the AMS board
* specified by the addr parameter.
*/
void ams_reset_bypass(uint8_t addr);


/**
* @brief Set AMS to Bypass Mode
*
* @param addr AMS board address
* 
* This command places the AMS board specified by the
* addr parameter into bypass mode. The AMS board will
* remain in bypass until the mode is cleared, or a
* bypass timeout occurs.

*/
void ams_set_bypass(uint8_t addr);


/**
* @brief Set AMS Panic Mode
*
* @param addr AMS board address
* 
* This command places the AMS board specified by the
* addr parameter into panic mode. This causes the
* ACT led to blink with a period of 500ms.
*/
//void ams_panic(uint8_t addr);


/**
* @brief Clear AMS Panic Mode
*
* @param addr AMS board address
* 
* This command removes the AMS board specified by the
* addr parameter from panic mode. The ACT led will stop
* blinking.
*/
//void ams_relax(uint8_t addr);


/**
* @brief Set AMS Bypass Counter
*
* @param addr AMS board address
* @param count AMS bypass timeout counter value
* 
* This command sets the bypass timeout counter value.
* The bypass timer ticks every 256ms, once the counter
* value provided by the parameter count is reached, the
* AMS board exits bypass mode.
*/
//void ams_set_bypass_time(uint8_t addr, uint16_t count);


/**
* @brief Get Battery Cell Status from AMS
*
* @param addr AMS board address
* 
* Polls the specified AMS board for temperature and 
* voltage readings. Both values are unsigned 16-bit
* integers corresponding to the PIC16LF1827 ADC reading.
* 
* The ADC uses a 3.3V scale, and returns 12-bit
* right-aligned values.
* 
* The return value of this function consists of the
* voltage reading stored in the upper 16-bits, and the
* temperature reading stored in the lower 16-bits.
*
* @return Cell temperature (upper 16-bits) and voltage (lower 16-bits)
*/
void ams_cell_status(uint8_t addr);

#endif /*TASK_I2C_H*/
