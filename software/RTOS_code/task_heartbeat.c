#include "task_heartbeat.h"
#include "config.h"
#include "params.h"
#include "atomport.h"
#include "atomtimer.h"


void heartbeat_init(void){
	DDRC |= (0x03); //sets port output for spare led
}

void task_heartbeat(uint32_t data) {
	// initialize heartbeat task
	heartbeat_init();
	
	for(;;) {
		//turn on the fault led if pack state is fault, updates about twice a second
		if(pack_state == flt){
			PORTC |= 0x02;
		}else{
			PORTC &= ~(0x02);
		}
		//blink spare led as heartbeat twice a second
		PORTC |= 0x01;
		atomTimerDelay(25);
		PORTC &= ~(0x01);
		atomTimerDelay(25);
	}
}
