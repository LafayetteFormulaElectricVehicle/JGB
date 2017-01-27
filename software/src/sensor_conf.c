#include "sensor_conf.h"

#include <avr/eeprom.h>

void setup_io(struct sys_config *sys) {
	DDRA = 0; 
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;
	
	//ADC1
	sys->adc_1->en = 0;
	sys->adc_1->ddr = 0;
	sys->adc_1->slot = 0;
	sys->adc_1->frame = 0;
	sys->adc_1->get_value = 0;
	
	
	//ADC2
	sys->adc_2->en = 0;
	sys->adc_2->ddr = 0;
	sys->adc_2->slot = 0;
	sys->adc_2->frame = 0;
	sys->adc_1->get_value = 0;
	
	//ADC3
	sys->adc_3->en = 0;
	sys->adc_3->ddr = 0;
	sys->adc_3->slot = 0;
	sys->adc_3->frame = 0;
	sys->adc_1->get_value = 0;
	
	//ADC4
	sys->adc_4;
	sys->adc_4->en = 0;
	sys->adc_4->ddr = 0;
	sys->adc_4->slot = 0;
	sys->adc_4->frame = 0;
	sys->adc_1->get_value = 0;
	
	//AMP0
	sys->amp_0->en = 0;
	sys->amp_0->ddr = 0;
	sys->amp_0->slot = 0;
	sys->amp_0->frame = 0;
	
	//AMP1
	sys->amp_1->en = 0;
	sys->amp_1->ddr = 0;
	sys->amp_1->slot = 0;
	sys->amp_1->frame = 0;
	
	//D2A
	sys->d2a;
	sys->d2a->en = 0;
	sys->d2a->ddr = 0;
	sys->d2a->slot = 0;
	sys->d2a->frame = 0;
	DDRC |= (1 << PC7);
	
	//PWM0
	sys->pwm_0->en = 0;
	sys->pwm_0->ddr = 0;
	sys->pwm_0->slot = 0;
	sys->pwm_0->frame = 0;
	DDRC |= (1 << PC1);
	
	//PWM1
	sys->pwm_1->en = 0;
	sys->pwm_1->ddr = 0;
	sys->pwm_1->slot = 0;
	sys->pwm_1->frame = 0;
	DDRD |= (1 << PD2);
	
	//SSR0
	sys->ssr_0->en = 0;
	sys->ssr_0->ddr = 0;
	sys->ssr_0->slot = 0;
	sys->ssr_0->frame = 0;
	DDRC |= (1 << PC6);
	
	//SSR1
	sys->ssr_1->en = 0;
	sys->ssr_1->ddr = 0;
	sys->ssr_1->slot = 0;
	sys->ssr_1->frame = 0;
	DDRC |= (1 << PD0);
	
	//SSR2
	sys->ssr_2->en = 0;
	sys->ssr_2->ddr = 0;
	sys->ssr_2->slot = 0;
	sys->ssr_2->frame = 0;
	DDRC |= (1 << PD1);
}


