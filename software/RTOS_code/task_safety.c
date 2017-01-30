#include "task_safety.h"
#include "params.h"
#include "atomport.h"
#include "atomtimer.h"


void safety_init(void) {
	PORTA |= 0x08;
	DDRA |= 0x39;
	sloop_state = false;

}

void task_safety(uint32_t data) {
	// initialize safety loop task
	safety_init();
	
	for(;;) {
		sloop_state = (PINA & 0x04)==0x04? false : true;
		if(pack_state == rdy){
			PORTA &= ~(0x08);// close safety loop relay
			if(sloop_state){//ready and loop is closed
				
			}
		}else{
			PORTA |= 0x08;// open safety loop relay
		}
	}
}
