#include "task_watchdog.h"

#include "atom.h"

void task_watchdog(uint32_t data) {
	DDRA |= (0x01);
	
	for(;;) {
		PORTA &= (0xFE);
		atomTimerDelay(3);
		PORTA |= (0x01);
		atomTimerDelay(3);
	}
}
