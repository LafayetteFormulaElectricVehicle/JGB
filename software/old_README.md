# LFEV 2015 CAN Communications Microprocessor #

This directory contains the Lafayette College Formula Electric Vehicle CAN Communications Board. This board is responsible for relaying all low level inputs and outputs (ADC, D2A, GPIO) from vehicle sub-systems to the main SCADA computer. This design contains both hardware (PCB), and the firmware written for the device.

## Hardware ##
All hardware was created in KiCad (old-stable), and is located in the 'hardware' directory. The project is described in more detail there.

## Software ##
Software written for this project is located in the 'software' directory. The target architecture for the firmware is the ATMega16M1 AVR. Software build proceedures, programming the device, and project organization are all described in more detail in the folder.

# Tractive System Controller #
## TSC Layout ##
The following pins are configured as such:

-ADC1 (Named ADC2 in code) = High voltage input (10-bit precision)

-ADC2 (Named ADC3 in code) = Physical throttle input (10-bit precision)

-ADC3 (Named ADC6 in code) = Motor controller voltage input (10-bit precision)

-ADC4 (Named ADC7 in code) = Mode selector input (***MUST*** hook to GND for TSC operation)

-RELAY1 (Named SSR0 in code) = Safety loop fault (1 = open, 0 = closed)

-RELAY2 (Named SSR1 in code) = Physical throttle/drive mode light enabled (1 = closed, 0 = open)

-RELAY3 (Named SSR2 in code) = Precharge relay enable (1 = closed, 0 = open)

-GPIO0 = Drive button input

-GPIO1 = Buzzer output

-GPIO2 = AIR voltage present input

-D2A = Throttle output (10-bit precision, calibrated so a 0x0000 to 0xffff range corresponds to 0 to 4 volts)

-AMP0 = Current sensor input (10-bit precision)

## TSC Operation ##
On startup or following a reset, the system checks the voltage present on ADC4; if 0V are present, it is set to TSC mode. In this mode, the onboard relays start closed and the throttle is disabled.
Following the initialization, it reads in the measurands above and stores their values. Additionally, it checks if the throttle has been enabled and, if so, outputs the software or physical throttle value to the D2A (or 0 if the throttle is disabled) and, if the throttle is controlled by the driver's pedal, it enables the momentary buzzer and drive mode light.
If the TSC receives a CAN remote transmit message addressed to 0x201, it transmits the relevant data from address 0x200, as shown below. If the TSC receives a CAN data message, it reads it in, adjusts the throttle settings and relays accordingly.
In addition, it is designed to transmit a data message over CAN from 0x200 both in response to a received message and approximately every 1.048576 seconds.
Also, if the throttle is enabled in software throttle mode and the board loses connection with SCADA for 1 second, the TSC ramps the throttle down to 0 and will not accept any throttle commands until the throttle reaches 0 and SCADA sends it a message telling it to disable throttle and set the software throttle to 0; this should take approximately 6.7 seconds from 100% and less time for smaller percentages.
If there are no faults detected, there is voltage present on the AIRs line, the physical throttle is not pressed, the precharge voltage to the motor controller is present, and the drive button is pressed, the system will switch into physical throttle mode, as it would if SCADA were to tell it to do so.

## TSC CAN Setup ##
Data message layout to SCADA from address 0x200:

    Byte 0:
    
        Bit Layout: [7 6 5 4 3 2 1 0]
                7 = Overwrite value in SCADA with value from board
                
                6 = Motor controller voltage present
                
                5 = AIR voltage present
                
                4 = Drive mode button pressed
                
                3 = Precharge relay closed
                
                2 = Fault/safety loop open
                
                1 = Throttle source (1 = software, 0 = physical)
                
                0 = Throttle enable
                
    Byte 1:  Physical throttle value read from potentiometer
    
    Byte 2:  Software throttle value provided from SCADA
    
    Byte 3:  High byte of the 16-bit measured current meter value
    
    Byte 4:  Low byte of the 16-bit measured current meter value
    
    Byte 5:  High byte of the 16-bit measured high voltage input value
    
    Byte 6:  Low byte of the 16-bit measured high voltage input value
    
Data message layout from SCADA to address 0x201:
    
    Byte 0:
    
        Bit Layout: [7 6 5 4 3 2 1 0]
                7 = Overwrite value in SCADA with value from board
                
                6 = Motor controller voltage present
                
                5 = AIR voltage present
                
                4 = Drive mode button pressed
                
                3 = Precharge relay closed
                
                2 = Fault/safety loop open
                
                1 = Throttle source (1 = software, 0 = physical)
                
                0 = Throttle enable
                
    Byte 1:  Software throttle value
    
# Huff Box DAQ #
## HB-DAQ Layout ##
The following pins are configured as such:

-ADC1 (Named ADC2 in code) = Torque sensor voltage input (10-bit precision)

-ADC2 (Named ADC3 in code) = Oil temperature sensor voltage input (10-bit precision)

-ADC4 (Named ADC7 in code) = Mode selector input (***MUST*** hook to 5V for HB-DAQ operation)

-D2A = Load valve output (10-bit precision, calibrated so a 0x0000 to 0xffff range corresponds to 0 to 4 volts)

## HB-DAQ Operation ##
On startup or following a reset, the system checks the voltage present on ADC4; if 5V are present, it is set to HB-DAQ mode. In this mode, the D2A is set to its minimum calibrated value of 0V.
Following the initialization, it reads in the measurands above and stores their values.
If the HB-DAQ receives a CAN remote transmit message addressed to 0x321, it transmits the relevant data from address 0x320, as shown below. If the HB-DAQ receives a CAN data message, it reads it in, adjusts the load valve settings accordingly.
In addition, it is designed to transmit a data message over CAN from 0x320 both in response to a received message and approximately every 1.048576 seconds.

## HB-DAQ CAN Setup ##
Data message layout to SCADA from address 0x320:

    Byte 0:  High byte of the 16-bit output load valve value
    
    Byte 1:  Low byte of the 16-bit output load valve value
    
    Byte 2:  High byte of the 16-bit measured torque value
    
    Byte 3:  Low byte of the 16-bit measured torque value
    
    Byte 4:  High byte of the 16-bit measured oil temperature value
    
    Byte 5:  Low byte of the 16-bit measured oil temperature value
    
Data message layout from SCADA to address 0x321:
    
    Byte 0:  High byte of the 16-bit output load valve value
    
    Byte 1:  Low byte of the 16-bit output load valve value