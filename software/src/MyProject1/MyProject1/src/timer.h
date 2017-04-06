#ifndef TIMER_H

#include <stdbool.h>
#include <stdint.h>

#define TIMER_H

#define TIM_SCALE_0 0
#define TIM_SCALE_1 1
#define TIM_SCALE_8 2
#define TIM_SCALE_64 3
#define TIM_SCALE_256 4
#define TIM_SCALE_1024 5
#define TIM_SCALE_EXT_FALL 6
#define TIM_SCALE_EXT_RISE 7

#define TIM_DISCON 0
#define TIM_TOGGLE 1
#define TIM_NRM_PWM 2
#define TIM_INV_PWM 3

#define PWM_OCxA 0
#define PWM_OCxB 1

#define PWM_NC 0
#define PWM_TGL 1
#define PWM_NRM 2
#define PWM_INV 3

#define WGM_MODE_0 0
#define WGM_MODE_1 1
#define WGM_MODE_2 2
#define WGM_MODE_3 3
#define WGM_MODE_4 4
#define WGM_MODE_5 5
#define WGM_MODE_6 6
#define WGM_MODE_7 7
#define WGM_MODE_8 8
#define WGM_MODE_9 9
#define WGM_MODE_10 10
#define WGM_MODE_11 11
#define WGM_MODE_12 12
#define WGM_MODE_13 13
#define WGM_MODE_14 13
#define WGM_MODE_15 14

void timer0_init(uint8_t timerScale);


void timer0_disable(void);


void timer1_init(uint8_t timerScale);


void timer1_disable(void);


bool timer0_pwm_init(uint8_t ocPin, uint8_t pwmMode, uint8_t wgmMode);


bool timer0_pwm_write(uint8_t ocPin, uint8_t value);


void timer1_pwm_init();


void timer1_pwm_write();

#endif

