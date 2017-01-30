#ifndef CONFIG_H
#define CONFIG_H

// AVR system clock, 10MHz
#define F_CPU 10E6

/*
 * Idle thread stack size
 *
 * This needs to be large enough to handle any interrupt handlers
 * and callbacks called by interrupt handlers (e.g. user-created
 * timer callbacks) as well as the saving of all context when
 * switching away from this thread.
 *
 * In this case, the idle stack is allocated on the BSS via the
 * idle_thread_stack[] byte array.
 */
#define IDLE_STACK_SIZE_BYTES       512//was 128 (didn't work). 512 seems to work but very large %.  Consider smaller value is space becomes an issue


/*
 * Main thread stack size
 *
 * Note that this is not a required OS kernel thread - you will replace
 * this with your own application thread.
 *
 * The Main thread stack generally needs to be larger than the idle
 * thread stack, as not only does it need to store interrupt handler
 * stack saves and context switch saves, but the application main thread
 * will generally be carrying out more nested function calls and require
 * stack for application code local variables etc.
 *
 */
#define MAIN_STACK_SIZE_BYTES       1024//was 204(didn't work).  1024 seems to work but very large %.  Consider smaller value is space becomes an issue

/*
 * CAN Transmission values
 */
#define FOSC 10000 //clock freq

#define CAN_BAUDRATE 125

#endif /* CONFIG_H */
