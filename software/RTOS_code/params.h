#ifndef PARAMS_H
#define PARAMS_H

// standard library includes
#include <stdint.h>
#include "atom.h"
#include "config.h"
#include "atomqueue.h"
#include "atomutils.h"
#include "atomport.h"
#include "atmel/can_lib.h"
#include "atommutex.h"


//SOFTWARE VERSION (based on commit number in gitlab)
#define version 0.14

// LCD values
#define EN_LOW	0x08

//4 arrays for the 4 lines on the LCD
unsigned char display_0[20];
unsigned char display_1[20];
unsigned char display_2[20];
unsigned char display_3[20];

//typedef enum {false,true} bool;  stdbool.h is included in can_lib.h so this is no longer necessary.
typedef enum {boot,chrg,chrgd,lco,flt,dead,rdy} State; //mirrored values in VSCADA

//various parameters that will be displayed on the LCD
volatile int16_t shunt_voltage;  //part of pack current calc
volatile int16_t pack_voltage;
volatile int32_t pack_current;
volatile int32_t adc_value;
volatile uint8_t pack_SOC;
volatile int32_t pack_coulombs;
volatile uint16_t cell_V[7];
volatile uint16_t cell_T[7];
volatile uint8_t cell_status[7];
volatile State pack_state;
volatile bool sloop_state;
volatile bool CHRG_DET;
volatile bool LOW_DET;
volatile bool button_up;
volatile bool button_down;


//Flags for lcd message and bypass selection timing
volatile bool lcd_flag;
volatile bool bypass_flag;

//Calibration factors
#define pack_Voff				-0.3
#define pack_ametes_Aoff  		.6775//amps
#define pack_INA226_Aoff 		-0.00886//amps
#define pack_Vslp				1.014
#define pack_ametes_Aslp 		107.4977// A/V
#define pack_INA226_Aslp		0.9636 //none
volatile uint16_t cell_Voff[7];
volatile uint16_t cell_Toff[7];
volatile float cell_Vslp[7];
volatile float cell_Tslp[7];

//CAN parameters
#define CANADD_PACKINFO1  0x0500  //additional packs should be at 0x0510, 0x520, 0x0530
#define CANADD_PACKINFO2  0x0501  //additional packs should be at 0x0511, 0x521, 0x0531
#define CANADD_CELLINFO  0x0502  //additional packs should be at 0x0512, 0x522, 0x0532
volatile uint8_t can_buff[8];
st_cmd_t can_frame;
volatile can_ctrl_t can_frame_ctrl;
volatile can_id_t can_frame_id;

//SOC parameters
#define MAX_COULOMBS  216000000 // * 10^-3 coulombs in a full pack

//Mutex to prevent integration of a bad current value
ATOM_MUTEX A_mutex;

#endif
