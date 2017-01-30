#ifndef PINOUT_H
#define PINOUT_H

#define GPIO_INPUT          0b00
#define GPIO_INPUT_PULLUP   0b01
#define GPIO_OUTPUT_LOW     0b10
#define GPIO_OUTPUT_HIGH    0b11

/**
* @name 
* @{ */
#define HIGH  1
#define LOW   0
/**  @} */

/**
* @brief 
*/
typedef struct {
	void* prt_reg,
	void* ddr_reg,
	void* pin_reg,
	uint8_t pin_num,
} GPIO_PIN;


/**
* @name 
* @{ */
// PORTA gpio pins
static const GPIO_PIN WATCHDOG    =  {&PORTA, &DDRA, &PINA, 0};
static const GPIO_PIN SW_SDA      =  {&PORTA, &DDRA, &PINA, 1};
static const GPIO_PIN SW_SCL      =  {&PORTA, &DDRA, &PINA, 2};
static const GPIO_PIN SLOOP_CTRL  =  {&PORTA, &DDRA, &PINA, 3};
static const GPIO_PIN CHRG_CTRL   =  {&PORTA, &DDRA, &PINA, 4};
static const GPIO_PIN FAN_CTRL    =  {&PORTA, &DDRA, &PINA, 5};

// PORTB gpio pins
static const GPIO_PIN GPIO_PB0    =  {&PORTB, &DDRB, &PINB, 0};
static const GPIO_PIN GPIO_PB1    =  {&PORTB, &DDRB, &PINB, 1};
static const GPIO_PIN GPIO_PB2    =  {&PORTB, &DDRB, &PINB, 2};
static const GPIO_PIN GPIO_PB3    =  {&PORTB, &DDRB, &PINB, 3};
static const GPIO_PIN GPIO_PB4    =  {&PORTB, &DDRB, &PINB, 4};
static const GPIO_PIN GPIO_PB5    =  {&PORTB, &DDRB, &PINB, 5};
static const GPIO_PIN GPIO_PB6    =  {&PORTB, &DDRB, &PINB, 6};
static const GPIO_PIN GPIO_PB7    =  {&PORTB, &DDRB, &PINB, 7};

// PORTC gpio pins
static const GPIO_PIN FAULT       =  {&PORTC, &DDRC, &PINC, 0};
static const GPIO_PIN REQUEST     =  {&PORTC, &DDRC, &PINC, 1};
static const GPIO_PIN LOW_VOLT    =  {&PORTC, &DDRC, &PINC, 2};

// PORTF gpio pins
static const GPIO_PIN GPIO_ADC0   =  {&PORTF, &DDRF, &PINF, 0};
static const GPIO_PIN GPIO_ADC1   =  {&PORTF, &DDRF, &PINF, 1};
static const GPIO_PIN GPIO_ADC2   =  {&PORTF, &DDRF, &PINF, 2};
static const GPIO_PIN GPIO_ADC3   =  {&PORTF, &DDRF, &PINF, 3};
/**  @} */


/**
* @brief 
*
* @param pin
* @param mode
*/
static inline void pinMode(GPIO_PIN pin, uint8_t mode) {
	*(pin.ddr_reg) = 
}


/**
* @brief 
*
* @param pin
* @param value
*/
static inline void pinWrite(GPIO_PIN pin, uint8_t value) {
	pin.
	*PORT = (*PORT & _BV(pin)) | (value << pin);
}


/**
* @brief 
*
* @param pin
*
* @return 
*/
static inline uint8_t pinRead(GPIO_PIN pin) {
	
}


#endif /*PINOUT_H*/
