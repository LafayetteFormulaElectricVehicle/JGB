/**
* @file jgb.h
* @author Greg Flynn
* @date 2017-01-28
* @brief File to allow interfacing with JGB
*
* This file allows the user to control the JGB (John Gehrig Board)
* The JGB is a CAN bus DAQ board.
* It can interface with:
*	3 relays
*	3 GPIOs
*	1 D2A (DAC AVR likes to call it D2A)
*	1 Sync line with MISO/MOSI
*	2 PWM lines
*	4 ADCs
*	2 Analog comparators
*	1 CAN bus line
*
* Currently the CAN bus line is terminated on the board.  This may be changed in the future.  If it is make sure that there is a 120 ohm resistor at the end of the CAN bus line.
**/

#include <stdint.h>

/**
* Use these defines to set the scale of the analog sources
*/
#define ADC_SCALE_2		0
#define ADC_SCALE_4		2
#define ADC_SCALE_8		3
#define ADC_SCALE_16	4
#define ADC_SCALE_32	5
#define ADC_SCALE_64	6
#define ADC_SCALE_128	7

/** 
* Use these defines to pick analog source 
*/
#define ADC_AREF 0
#define ADC_AVCC 1

/**
* This maps the ADC pins on the JGB to the corresponding ADC on the AVR.
* These values are used in the adc_read method to select what pin to sample
*/
#define ADC1 0x02
#define ADC2 0x03
#define ADC3 0x06
#define ADC4 0x07

/**
* @brief setup the analog reference
*
* This function must be called to use the ADC and DACs
* @param mode ADC_AREF or ADC_AVCC
* @param scale ADC_SCALE_(2,4,8,16,32,64,128)
*/
void analog_setup(uint8_t mode, uint8_t scale);

/**
* @brief get the adc value (10 bit) of a pin
*
* This function reads the ADC value from one of the ADC pins.
* The wiring of the JGB means that ADC1 is not what the AVR labels as ADC1
* What is defined as ADC1 is what has the ADC1 silkscreen
* Do not worry about the lower levels unless the board is rerouted.
* @param pin which ADC to use, ADC(1-4)
*/
uint16_t adc_read(uint8_t pin);

/**
* @brief turn the DAC on
*/
void dac_enable(void);

/**
* @brief turn the DAC off
*/
void dac_disable(void);

/**
* @brief set the DAC value
*
* There is only one DAC on the JGB.
* It is a 10 bit DAC
*/
void dac_write(uint16_t level);


