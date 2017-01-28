#include "config.h"

//AVR-LIBC Headers
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <util/delay.h>

#include <stdio.h>

#include "can_lib.h"
#include "adc.h"
#include "timer.h"
#include "uart.h"
#include "sensor_conf.h"
#include "calibration.h"

//Device CAN_IDs
#define CAN_ID  0x200

//#define CAN_MSK 0x200//~(0x03)

//Device modes
#define MAINTENANCE_MODE 0
#define ANALOG_MODE 1
#define SOFTWARE_MODE 2

//Bits in the first byte of the tx buffer
#define throttle_en 0
#define throttle_src 1
#define s_loop_fault 2
#define precharge 3
#define drive_button 4
#define AIR_volt 5
#define mc_volt 6
#define control_bit 7

//Define values for mode selection ranges from ADC
#define TSI_min 0x00
#define TSI_max 0x3F
#define mode2_min 0x40
#define mode2_max 0x7F
#define mode3_max 0x80
#define mode3_min 0xBF
#define Huff_min 0xC0
#define Huff_max 0xFF

//uint8_t response_data[CAN_BUFFER_SIZE];
uint8_t can_buffer_tx[CAN_BUFFER_SIZE];
uint8_t can_buffer_rx[CAN_BUFFER_SIZE];

//Pointers to the values of the physical throttle, software throttle, current sensor, and high voltage respectively
uint8_t *throttle_hi, *soft_throttle_hi, *current_sensor_hi, *current_sensor_lo, *high_voltage_hi, *high_voltage_lo;

//16-bit values taken in by sensors
uint16_t throttle, soft_throttle, mc_voltage, current_sensor, AIRLED, high_voltage;

//Counter for buzzer
uint8_t mode, buzzer = 0;

//Boolean for the throttle ramp-down state and transmission preparation
bool soft_down = false;
bool transmit_rdy = false;

//The io config from sensor_conf.h
struct sys_config io_conf;

//The messages that shall be used
st_cmd_t tx_msg;
st_cmd_t rx_conf, rx_rtr;

void setup(void)
{
	// setup onboard analog hardware (ADC/DAC)
	analog_setup(ADC_AVCC, ADC_SCALE_128);

	//Configure IO ports
    setup_io(&io_conf);

    //Initialize CAN
    can_init(0);

	// enable the DAC
	dac_enable();

	// initialize serial device
	uart_init(BAUD);
	fdevopen(&uart_putc, NULL);
	fdevopen((int (*)(char,  struct __file *))&uart_getc, NULL);

    //Enable interrupt for if CAN controller enters off state due to errors
    CANGIE |= (1 << ENBOFF);

	// enable interrupts
	sei();
}

//Helper methods for TSI functionality
void enable_throttle(bool enabled)
{
    can_buffer_tx[0] &= ~(1 << throttle_en);
    can_buffer_tx[0] |= (enabled << throttle_en);
}

bool throttle_enabled()
{
    return ((can_buffer_tx[0] & (1 << throttle_en)) != 0);
}

void enable_software_mode(bool enabled)
{
    can_buffer_tx[0] &= ~(1 << throttle_src);
    can_buffer_tx[0] |= (enabled << throttle_src);
}

bool software_throttle_enabled()
{
    return ((can_buffer_tx[0] & (1 << throttle_src)) != 0);
}

void set_fault(bool fault)
{
    can_buffer_tx[0] &= ~(1 << s_loop_fault);
    can_buffer_tx[0] |= (fault << s_loop_fault);
    set_ssr(SSR0, !fault);
}

bool safety_loop_fault()
{
    return ((can_buffer_tx[0] & (1 << s_loop_fault)) != 0);
}

void enable_precharge(bool close)
{
    can_buffer_tx[0] &= ~(1 << precharge);
    can_buffer_tx[0] |= (close << precharge);
    set_ssr(SSR2, close);
}

bool precharge_enabled()
{
    return ((can_buffer_tx[0] & (1 << precharge)) != 0);
}

void drive_button_set(bool pressed)
{
    can_buffer_tx[0] &= ~(1 << drive_button);
    can_buffer_tx[0] |= (pressed << drive_button);
}

bool drive_button_pressed()
{
    return ((can_buffer_tx[0] & (1 << drive_button)) != 0);
}

void AIR_voltage_set(bool present)
{
    can_buffer_tx[0] &= ~(1 << AIR_volt);
    can_buffer_tx[0] |= (present << AIR_volt);
}

bool AIR_voltage_present()
{
    return ((can_buffer_tx[0] & (1 << AIR_volt)) != 0);
}

void mc_voltage_set(bool present)
{
    can_buffer_tx[0] &= ~(1 << mc_volt);
    can_buffer_tx[0] |= (present << mc_volt);
}

bool mc_voltage_present()
{
    return ((can_buffer_tx[0] & (1 << mc_volt)) != 0);
}

void tsc_control_set(bool control)
{
    can_buffer_tx[0] &= ~(1 << control_bit);
    can_buffer_tx[0] |= (control << control_bit);
}

bool tsc_control()
{
    return ((can_buffer_tx[0] & (1 << control_bit)) != 0);
}

bool rx_throttle_enabled()
{
    return ((can_buffer_rx[0] & (1 << throttle_en)) != 0);
}

bool rx_software_throttle_enabled()
{
    return ((can_buffer_rx[0] & (1 << throttle_src)) != 0);
}

bool rx_safety_loop_fault()
{
    return ((can_buffer_rx[0] & (1 << s_loop_fault)) != 0);
}

bool rx_precharge_enabled()
{
    return ((can_buffer_rx[0] & (1 << precharge)) != 0);
}
//End TSI helper methods

int main(void)
{
    // initialize uc hardware
	setup();

    //Delay so board has a chance to begin outputting voltage
    _delay_ms(5);

	//Mode selection
    mode = (adc_read(ADC7) >> 8);

    if((mode >= TSI_min) & (mode <= TSI_max)) //TSI mode = ADC input 4 connected to ground
    {
        set_ssr(SSR0, false);
        set_ssr(SSR1, false);
        set_ssr(SSR2, false);


        //Enable timer 1 interrupt
        TIMSK1 |= (1 << TOIE1);

        // initialize timer for throttle broadcast
        //Tclkio = 62.5 ns, 16 bit counter, 256 prescale -> 62.5e-9 * 2^16 * 256 = 1.048576s
        timer1_init(TIM_SCALE_256 );

        //Enable timer 1 compare A interrupt
        TIMSK1 |= (1 << OCIE1A);

        //Set the compare interrupt to go off in 1s
        OCR1A = TCNT1 - 1;

        soft_throttle = throttle = 0;

        //Assign pointers to buffer locations
        throttle_hi = &can_buffer_tx[1];
        soft_throttle_hi = &can_buffer_tx[2];
        current_sensor_hi = &can_buffer_tx[3];
        current_sensor_lo = &can_buffer_tx[4];
        high_voltage_hi = &can_buffer_tx[5];
        high_voltage_lo = &can_buffer_tx[6];



        //Start with empty buffers
        /*
            Byte 0 Layout: [7 6 5 4 3 2 1 0]
                7 = Overwrite value in SCADA with value from board
                6 = Motor controller voltage present
                5 = AIR voltage present
                4 = Drive mode button pressed
                3 = Precharge relay closed
                2 = Fault/safety loop open
                1 = Throttle source (1 = software, 0 = physical)
                0 = Throttle enable
        */

        can_buffer_tx[0] = 0x04; //Byte 0
        can_buffer_tx[1] = 0x00; //Physical throttle
        can_buffer_tx[2] = 0x00; //Software throttle
        can_buffer_tx[3] = 0x00; //Current
        can_buffer_tx[4] = 0x00; //Current
        can_buffer_tx[5] = 0x00; //Voltage
        can_buffer_tx[6] = 0x00; //Voltage
        can_buffer_tx[7] = 0x00; //Reserved

        can_buffer_rx[0] = 0x00; //Byte 0
        can_buffer_rx[1] = 0x00; //Software throttle
        can_buffer_rx[2] = 0x00;
        can_buffer_rx[3] = 0x00;
        can_buffer_rx[4] = 0x00;
        can_buffer_rx[5] = 0x00;
        can_buffer_rx[6] = 0x00;
        can_buffer_rx[7] = 0x00;

        //Set up the message for the mode output
        tx_msg.pt_data = &can_buffer_tx[0]; //point message object to first element of data buffer
        tx_msg.ctrl.ide = 0;                //standard CAN frame type (2.0A)
        tx_msg.dlc = 7;                     //Number of bytes being sent (8 max)
        tx_msg.id.std = CAN_ID;             //populate ID field with ID Tag
        tx_msg.cmd = CMD_TX_DATA;           //assign this as a "Standard (2.0A) Reply" message object

        //Setup receive message
        rx_conf.pt_data = &can_buffer_rx[0];
        rx_conf.ctrl.ide = 0;
        rx_conf.dlc = 2;
        rx_conf.ctrl.rtr = 0;
        rx_conf.id.std = CAN_ID+1;
        rx_conf.cmd = CMD_RX_DATA_MASKED;

        //Setup receive message
        rx_rtr.pt_data = &can_buffer_rx[7];
        rx_rtr.ctrl.ide = 0;
        rx_rtr.ctrl.rtr = 1;
        rx_rtr.dlc = 0;
        rx_rtr.id.std = CAN_ID+1;
        rx_rtr.cmd = CMD_RX_REMOTE_MASKED;

        //wait for receiving MOb to configure
        while(can_cmd(&rx_conf) != CAN_CMD_ACCEPTED);

        //wait for RTR MOb to configure
        while(can_cmd(&rx_rtr) != CAN_CMD_ACCEPTED);

        //Prepare to send on initial boot
        transmit_rdy = true;

        //Running loop
        for(;;)
        {
            //Check state of drive button
            drive_button_set(get_gpio_value(0));

            //Sample the AIRLED voltage
            AIR_voltage_set(get_gpio_value(2)); //moved to gpio

            //Sample the physical throttle
            throttle = get_calibrated_value(adc_read(ADC3), GENERIC_CAL);
            *throttle_hi = (throttle >> 8);

            //Sample the motor controller voltage
            mc_voltage = get_calibrated_value(adc_read(ADC6), GENERIC_CAL);
            mc_voltage_set(mc_voltage <= 0x3333);

            //Sample the current sensor input
            current_sensor = get_calibrated_value(adc_read(AMP0), GENERIC_CAL);
            *current_sensor_hi = (current_sensor >> 8);
            *current_sensor_lo = current_sensor;

            //Sample the high voltage
            high_voltage = get_calibrated_value(adc_read(ADC2), GENERIC_CAL);
            *high_voltage_hi = (high_voltage >> 8);
            *high_voltage_lo = high_voltage;

            //Drive through software mode
            if(drive_button_pressed() & !safety_loop_fault() & (throttle < 0x00FF) & AIR_voltage_present()/* & mc_voltage_present()*/) //mc_voltage not wired properly, so disabled
            {
                //Enable the buzzer if applicable
                if((!throttle_enabled()))
                {
                    OCR1B = TCNT1-1;
                    TIMSK1 |= (1 << OCIE1B);
                    set_gpio_value(1, true);
                }
                //Tell SCADA that JGB is asserting a value
                tsc_control_set(true);

                enable_software_mode(false);
                enable_precharge(true);
                enable_throttle(true);

                //Send the new status to SCADA
                while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
                while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);

                tsc_control_set(false);
            }
            else if(!AIR_voltage_present() | /*!mc_voltage_present() | */safety_loop_fault())
            {
                if(/*precharge_enabled() | */throttle_enabled())
                {
                    //Tell SCADA that JGB is asserting a value
                    tsc_control_set(true);

                    enable_precharge(false);
                    enable_throttle(false);

                    //Send the new status to SCADA
                    while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
                    while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);

                    tsc_control_set(false);
                }
            }

            //Check if it has been time to transmit
            if(transmit_rdy)
            {
                while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
                // send a response
                while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
                transmit_rdy = false;
            }

            //check for a transmit request
            if(can_get_status(&rx_conf) == CAN_STATUS_COMPLETED)
            {
                //Control SSRs regardless of whether the board is in the software ramp-down state
                set_fault(rx_safety_loop_fault());
                enable_precharge(rx_precharge_enabled());

                //Start accepting SCADA commands if the throttle is set to 0 and SCADA disables the throttle
                soft_down = soft_down & (rx_throttle_enabled() | (can_buffer_rx[1] != 0) | (soft_throttle > 0));

                if(!soft_down)
                {
                    //Enable the buzzer if applicable
                    if((!rx_software_throttle_enabled() & software_throttle_enabled() & rx_throttle_enabled()) | (!rx_software_throttle_enabled() & !throttle_enabled() & rx_throttle_enabled()))
                    {
                        OCR1B = TCNT1-1;
                        TIMSK1 |= (1 << OCIE1B);
                        set_gpio_value(1, true);
                    }

                    //Set the mode
                    enable_software_mode(rx_software_throttle_enabled());
                    enable_throttle(rx_throttle_enabled());

                    //Set the compare interrupt time
                    OCR1A = TCNT1 + 16384;

                    //Set the sotware throttle
                    soft_throttle = (can_buffer_rx[1] << 8);
                    *soft_throttle_hi = can_buffer_rx[1];
                }

                //check if MOb configured
                while(can_cmd(&rx_conf) != CAN_CMD_ACCEPTED);
                while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);

                // send a response
                while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
            }

            //Decide on drive mode light output and throttle source
            if((throttle_enabled() | soft_down) & !safety_loop_fault())
            {
                if(software_throttle_enabled() | soft_down)
                {
                    dac_write(get_calibrated_value(soft_throttle, THROTTLE_OUT_CAL));
                    set_ssr(SSR1, false);
                }
                else
                {
                    dac_write(get_calibrated_value(throttle, THROTTLE_OUT_CAL));
                    set_ssr(SSR1, true);
                }
            }
            else
            {
                dac_write(0);
                set_ssr(SSR1, false);
            }


            if(can_get_status(&rx_rtr) == CAN_STATUS_COMPLETED)
            {
                while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
                // send a response
                while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
                while(can_cmd(&rx_rtr) != CAN_CMD_ACCEPTED);
            }
        }
    }
    else if((mode >= Huff_min ) & (mode <= Huff_max)) //Huff box mode = ADC input 4 connected to +5V
    {
        uint16_t load_valve, torque, temperature;

        uint8_t *load_valve_hi, *load_valve_lo, *torque_hi, *torque_lo, *temperature_hi, *temperature_lo;

        load_valve_hi = &can_buffer_tx[0];
        load_valve_lo = &can_buffer_tx[1];
        torque_hi = &can_buffer_tx[2];
        torque_lo = &can_buffer_tx[3];
        temperature_hi = &can_buffer_tx[4];
        temperature_lo = &can_buffer_tx[5];


        //Enable timer 1 interrupt
        TIMSK1 |= (1 << TOIE1);

        // initialize timer for throttle broadcast
        //Tclkio = 62.5 ns, 16 bit counter, 256 prescale -> 62.5e-9 * 2^16 * 256 = 1.048576s
        timer1_init(TIM_SCALE_256 );

        can_buffer_tx[0] = 0x00; //Load valve hi
        can_buffer_tx[1] = 0x00; //Load valve lo
        can_buffer_tx[2] = 0x00; //Torque hi
        can_buffer_tx[3] = 0x00; //Torque lo
        can_buffer_tx[4] = 0x00; //Oil temp hi
        can_buffer_tx[5] = 0x00; //Oil temp lo
        can_buffer_tx[6] = 0x00; //Reserved
        can_buffer_tx[7] = 0x00; //Reserved

        can_buffer_rx[0] = 0x00; //Load valve hi
        can_buffer_rx[1] = 0x00; //Load valve lo
        can_buffer_rx[2] = 0x00;
        can_buffer_rx[3] = 0x00;
        can_buffer_rx[4] = 0x00;
        can_buffer_rx[5] = 0x00;
        can_buffer_rx[6] = 0x00;
        can_buffer_rx[7] = 0x00;

        //Set up the message for the mode output
        tx_msg.pt_data = &can_buffer_tx[0]; //point message object to first element of data buffer
        tx_msg.ctrl.ide = 0;                //standard CAN frame type (2.0A)
        tx_msg.dlc = 6;                     //Number of bytes being sent (8 max)
        tx_msg.id.std = CAN_ID+0x120;             //populate ID field with ID Tag
        tx_msg.cmd = CMD_TX_DATA;           //assign this as a "Standard (2.0A) Reply" message object

        //Setup receive message
        rx_conf.pt_data = &can_buffer_rx[0];
        rx_conf.ctrl.ide = 0;
        rx_conf.dlc = 2;
        rx_conf.ctrl.rtr = 0;
        rx_conf.id.std = CAN_ID+0x121;
        rx_conf.cmd = CMD_RX_DATA_MASKED;

        //Setup receive message
        rx_rtr.pt_data = &can_buffer_rx[3];
        rx_rtr.ctrl.ide = 0;
        rx_rtr.ctrl.rtr = 1;
        rx_rtr.dlc = 0;
        rx_rtr.id.std = CAN_ID+0x121;
        rx_rtr.cmd = CMD_RX_REMOTE_MASKED;

        //wait for receiving MOb to configure
        while(can_cmd(&rx_conf) != CAN_CMD_ACCEPTED);

        //wait for RTR MOb to configure
        while(can_cmd(&rx_rtr) != CAN_CMD_ACCEPTED);

        for(;;)
        {
            torque = get_calibrated_value(adc_read(ADC2), GENERIC_CAL);
            *torque_hi = (torque >> 8);
            *torque_lo = torque;
            temperature = get_calibrated_value(adc_read(ADC3), GENERIC_CAL);
            *temperature_hi = (temperature >> 8);
            *temperature_lo = temperature;

            load_valve = (*load_valve_hi << 8) | *load_valve_lo;
            dac_write(get_calibrated_value(load_valve, LOAD_OUT_CAL)); //Value set to be limited to 0xf3

            //Check if it has been time to transmit
            if(transmit_rdy)
            {
                while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
                // send a response
                while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
                transmit_rdy = false;
            }

            //check for a transmit request
            if(can_get_status(&rx_conf) == CAN_STATUS_COMPLETED)
            {
                *load_valve_hi = can_buffer_rx[0];
                *load_valve_lo = can_buffer_rx[1];
                load_valve = (*load_valve_hi << 8) | *load_valve_lo;

                while(can_cmd(&rx_conf) != CAN_CMD_ACCEPTED);
                while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);

                // send a response
                while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
            }

            if(can_get_status(&rx_rtr) == CAN_STATUS_COMPLETED)
            {
                while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
                // send a response
                while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
                while(can_cmd(&rx_rtr) != CAN_CMD_ACCEPTED);
            }
        }
    }
    else if((mode >= mode2_min) & (mode <= mode2_max))
    {

    }
    else if((mode >= mode3_min) & (mode <= mode3_max))
    {

    }
	return 0;
}

//Interrupts when the timer compare A triggers, indicating that there has not been a response from SCADA for 1s and sets the throttle to 0
ISR(TIMER1_COMPA_vect)
{
    if((mode >= TSI_min) & (mode <= TSI_max)) //TSI mode = ADC input 4 connected to ground
    {
        //Run ramp-down if the software throttle is in control
        if(!soft_down)
        {
            //Set the compare interrupt to go off again in 1s
            OCR1A = TCNT1 - 1;

            //Sets the ramp-down state if the throttle is currently running in software mode
            soft_down = (throttle_enabled() & software_throttle_enabled());

            if(soft_down)
            {
                timer0_init(TIM_SCALE_64);
                TIMSK0 |= (1 << TOIE0);
            }

            //Disables throttle if set to software throttle mode
            enable_throttle(throttle_enabled() & !software_throttle_enabled());
        }
    }
    else if((mode >= Huff_min ) & (mode <= Huff_max)) //Huff box mode = ADC input 4 connected to +5V
    {

    }
    else if((mode >= mode2_min) & (mode <= mode2_max))
    {

    }
    else if((mode >= mode3_min) & (mode <= mode3_max))
    {

    }
}

ISR(TIMER1_COMPB_vect)
{
    if((mode >= TSI_min) & (mode <= TSI_max)) //TSI mode = ADC input 4 connected to ground
    {
    //Interrupts when the timer compare B triggers, indicating that the buzzer should turn off
        buzzer = buzzer + 1;
        if(buzzer >= 2)
        {
            set_gpio_value(1, false);
            TIMSK1 &= ~(1 << OCIE1B);
            buzzer = 0;
        }
    }
    else if((mode >= Huff_min ) & (mode <= Huff_max)) //Huff box mode = ADC input 4 connected to +5V
    {

    }
    else if((mode >= mode2_min) & (mode <= mode2_max))
    {

    }
    else if((mode >= mode3_min) & (mode <= mode3_max))
    {

    }
}

ISR(TIMER1_OVF_vect)
{
    if((mode >= TSI_min) & (mode <= TSI_max)) //TSI mode = ADC input 4 connected to ground
    {
        //Interrupts when the timer overflows at ~1s in order to send the data over CAN
        /*while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
        // send a response
        while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);*/
        transmit_rdy = true;
    }
    else if((mode >= Huff_min ) & (mode <= Huff_max)) //Huff box mode = ADC input 4 connected to +5V
    {
        //Interrupts when the timer overflows at ~1s in order to send the data over CAN
        /*while(can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
        // send a response
        while(can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);*/
        transmit_rdy = true;

    }
    else if((mode >= mode2_min) & (mode <= mode2_max))
    {

    }
    else if((mode >= mode3_min) & (mode <= mode3_max))
    {

    }
}

ISR(TIMER0_OVF_vect)
{
    if((mode >= TSI_min) & (mode <= TSI_max)) //TSI mode = ADC input 4 connected to ground
    {
        //10 selected to give ~6.7 seconds for 100% to ramp down to 0 (2^8 for 8-bit counter * 64 prescale * 62.5ns * 2^16 bits for throttle / 10)
        if(soft_throttle > 10)
        {
            soft_throttle = soft_throttle - 10;
            *soft_throttle_hi = (soft_throttle >> 8);
        }
        else
        {
            soft_throttle = 0;
            *soft_throttle_hi = 0;//(soft_throttle >> 8);
            timer0_disable();
        }
    }
    else if((mode >= Huff_min ) & (mode <= Huff_max)) //Huff box mode = ADC input 4 connected to +5V
    {

    }
    else if((mode >= mode2_min) & (mode <= mode2_max))
    {

    }
    else if((mode >= mode3_min) & (mode <= mode3_max))
    {

    }
}

//Restart CAN controller if software disables it due to errors
ISR(CAN_INT_vect)
{
    can_init(0);//wait for receiving MOb to configure
    while(can_cmd(&rx_conf) != CAN_CMD_ACCEPTED);

    //wait for RTR MOb to configure
    while(can_cmd(&rx_rtr) != CAN_CMD_ACCEPTED);
}
