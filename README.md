# LFEV 2015 CAN Communications Microprocessor #

## For now we have decided not to add JGBs to the system
JGBs provide good developement platforms but we will use the hardware and software in special configurations in each subsystem.

This directory contains the Lafayette College Formula Electric Vehicle CAN Communications Board. This board is responsible for relaying all low level inputs and outputs (ADC, D2A, GPIO) from vehicle sub-systems to the main SCADA computer. This design contains both hardware (PCB), and the firmware written for the device.

## Hardware ##
All hardware was created in KiCad (old-stable), and is located in the 'hardware' directory. The project is described in more detail there.

## Software ##
Software written for this project is located in the 'software' directory. The target architecture for the firmware is the ATMega16M1 AVR. Software build proceedures, programming the device, and project organization are all described in more detail in the folder.
