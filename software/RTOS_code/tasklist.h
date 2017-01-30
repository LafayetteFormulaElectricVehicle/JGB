#ifndef TASKLIST_H
#define TASKLIST_H

// configuration
#include "config.h"

// RTOS includes
#include "atom.h"

// RTOS tasks includes
#include "task_i2c.h"
#include "task_i2c.h"
#include "task_gui.h"
#include "task_watchdog.h"
#include "task_charge.h"
#include "task_safety.h"
#include "task_config.h"
#include "task_heartbeat.h"

/**
* @name 
* @{ */
#define TASK_I2C_SIZE       200//was 100
#define TASK_GUI_SIZE       200
#define TASK_WATCHDOG_SIZE  200
#define TASK_CHARGE_SIZE    200
#define TASK_SAFETY_SIZE    200
#define TASK_CONFIG_SIZE    200
#define TASK_HEARTBEAT_SIZE 200
#define TASK_CAN_SIZE		200
#define TASK_BUTTON_SIZE		200
/**  @} */


/**
* @brief 
*/
typedef struct {
	ATOM_TCB *tcb_ptr;
	uint8_t priority;
	void (*entry_point)(uint32_t);
	uint32_t entry_param;
	void *stack_top;
	uint32_t stack_size;
} ATOM_TASK;

/**
* @brief 
*
* @return 
*/
uint8_t atomThreadStartAll(void);

#endif /*TASKLIST_H*/
