#include <stdint.h>

//#define	ADC0	0
//#define ADC1	1
//#define ADC2	2
//#define ADC3	3
//#define ADC4	4
//#define ADC5	5
//#define ADC6	6
//#define ADC7	7
//#define ADC8	8
//#define ADC9	9
//#define ADC10	10
//#define TEMP	11
//#define VCC4	14
//#define AMP0	15
//#define AMP1	16
//#define AMP2	17

#define ADC_SCALE_2		0
#define ADC_SCALE_4		2
#define ADC_SCALE_8		3
#define ADC_SCALE_16	4
#define ADC_SCALE_32	5
#define ADC_SCALE_64	6
#define ADC_SCALE_128	7

#define ADC_AREF 0
#define ADC_AVCC 1

void analog_setup(uint8_t mode, uint8_t scale);

uint16_t adc_read(uint8_t pin);

void dac_enable(void);

void dac_disable(void);

void dac_write(uint16_t level);


