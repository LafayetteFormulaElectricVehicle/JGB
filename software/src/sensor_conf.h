#ifndef SENSOR_CONF_H
#define SENSOR_CONF_H

//GPIO
const  uint8_t GPIO0_BIT  = PB2;
const *uint8_t GPIO0_PIN  = PINB;
const *uint8_t GPIO0_PORT = PORTB;
const *uint8_t GPIO0_DDR  = DDRB;

const  uint8_t GPIO1_BIT  = PD7;
const *uint8_t GPIO1_PIN  = &PIND;
const *uint8_t GPIO1_PORT = &PORTD;
const *uint8_t GPIO1_DDR  = &DDRD;

const  uint8_t GPIO2_BIT  = PC0;
const *uint8_t GPIO2_PIN  = &PINC;
const *uint8_t GPIO2_PORT = &PORTC;
const *uint8_t GPIO2_DDR  = &DDRC;

const  uint8_t GPIO3_BIT  = PB7;
const *uint8_t GPIO3_PIN  = &PINB;
const *uint8_t GPIO3_PORT = &PORTB;
const *uint8_t GPIO3_DDR  = &DDRB;

const  uint8_t GPIO4_BIT  = PB1;
const *uint8_t GPIO4_PIN  = &PINB;
const *uint8_t GPIO4_PORT = &PORTB;
const *uint8_t GPIO4_DDR  = &DDRB;

const  uint8_t GPIO5_BIT  = PB0;
const *uint8_t GPIO5_PIN  = &PINB;
const *uint8_t GPIO5_PORT = &PORTB;
const *uint8_t GPIO5_DDR  = &DDRB;

struct in_config {
	uint8_t en : 1;
	uint8_t slot : 2;
	uint8_t frame: 3;
	uint16_t (*get_value)(void);
};

struct out_config {
	uint8_t en : 1;
	uint8_t slot : 2;
	uint8_t frame: 3;
	void (*set_value)(uint8_t);
};

struct bit_config {
	uint8_t en : 1;
	uint8_t ddr : 1;
	uint8_t bit : 4;
	uint8_t slot : 2;
	uint8_t frame: 3;
	*uint8_t pin;
	*uint8_t port;
	*uint8_t ddr;
	void (*set_value)(uint8_t);
	uint8_t (*get_value)(void);
};

struct sys_config {
	in_config adc_1;
	in_config adc_2;
	in_config adc_3;
	in_config adc_4;
	in_config amp_0;
	in_config amp_1;
	in_config amp_1;

	out_config d2a;
	out_config pwm_0;
	out_config pwm_1;
	out_config ssr_0;
	out_config ssr_1;
	out_config ssr_2;
	
	bit_config gpio0;
	bit_config gpio1;
	bit_config gpio2;
	bit_config gpio3;
	bit_config gpio4;
	bit_config gpio5;
};

//Setup pin directions, etc
void setup_io(struct *sys_config);


#endif
