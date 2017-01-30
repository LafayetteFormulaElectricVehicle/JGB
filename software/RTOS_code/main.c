/**
 * \file
 *
 * \brief Battery Pack Mangement Software
 *
 * Lafayette College LFEV
 * Pack Mangement Software
 * Copyright © 2015 John Gehrig
 *
 * \page License
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

// standard library includes
#include <stdio.h>

// configuration
#include "config.h"

// avr libc includes
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

// RTOS includes
#include "atom.h"
#include "atomport-private.h"
#include "atomtimer.h"

// local includes
#include "tasklist.h"


// Idle thread's stack area
static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];

int main ( void ) {
	int8_t status;
	
	/**
	 * Reuse part of the idle thread's stack for the stack required
	 * during this startup function.
	 */
	SP = (int)&idle_thread_stack[(IDLE_STACK_SIZE_BYTES/2) - 1];

	/**
	 * Initialise the OS before creating our threads.
	 *
	 * Note that we tell the OS that the idle stack is half its actual
	 * size. This prevents it prefilling the bottom half with known
	 * values for stack-checkig purposes, which we cannot allow because
	 * we are temporarily using it for our own stack. The remainder will
	 * still be available once the OS is started, this only prevents the
	 * OS from prefilling it.
	 *
	 * If you are not reusing the idle thread's stack during startup then
	 * you should pass in the correct size here.
	 */
	status = atomOSInit(&idle_thread_stack[IDLE_STACK_SIZE_BYTES - 1], (IDLE_STACK_SIZE_BYTES/2));
	
	if (status == ATOM_OK) {
		// Enable the system tick timer
		avrInitSystemTickTimer();

		// start all user-generated tasks
		status = atomThreadStartAll();

		if (status == ATOM_OK) {
			// start RTOS
			atomOSStart();
		}
	}
	
	DDRA = 0xFF;

	// There was an error starting the OS if we reach here
	for(;;) PORTA = !PORTA; // Loop forever, preserve state for debugger
	
	return (0);
}
