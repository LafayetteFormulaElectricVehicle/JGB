# LFEV 2015 CAN Communications Microprocessor #
This software is written for a CAN 2.0 communications board. The is a microcontroller was designed to interface all low level analog and digital sensors present on the 2015 LFEV with the main Data Acquisition Computer (SCADA). The software is capable of handling both reading and writing data.  

## Overview ##

## Target Device ##
This software is written for the **Atmel ATMEGA 16M1 Automotive AVR**  
  
This device was selected for its price, software availability, documentation and onboard CAN bus support.

The capabilities and documentation for this device can be found at:

1. [ATMega16M1 Overview](http://www.atmel.com/devices/atmega16m1.aspx)
2. [ATMega16M1 Datasheet](http://www.atmel.com/images/doc8209.pdf)


### Toolchain Information ###
**Compiler:** GCC-4.9.2 (AVR)  
**C Library:** AVR LibC  
**Downloader:** AVRDude 5.11.1  
**Programmer:** USBtinyISP  

### Toolchain Manuals ###
[GCC User Manual](https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/)  
[AVR LibC User Manual](http://www.nongnu.org/avr-libc/user-manual/index.html)  
[AVRDude User Manual](http://www.nongnu.org/avrdude/user-manual/avrdude.html)  
[USBtinyISP Information](https://learn.adafruit.com/usbtinyisp)  
[ATMega16M1 User Manual](http://www.atmel.com/images/doc8209.pdf)
  
### Compiler Information ###
Here is output information from the C compiler used to write all code. A standard GCC compiler with AVR target has been used. This compiler was built from source, however the latest version of this prebuilt toolchain of could be extracted from the Arduino IDE for Mac/Window/Linux if required. [Download](http://arduino.cc/en/Main/Software)

To build a GCC AVR cross compiler, insturctions can be found at: [Tutorial](http://www.nongnu.org/avr-libc/user-manual/install_tools.html)

**Compiler Details**
```
user@scada:~/$ avr-gcc -v

Using built-in specs.
COLLECT_GCC=avr-gcc
COLLECT_LTO_WRAPPER=/usr/local/bin/../libexec/gcc/avr/4.9.2/lto-wrapper
Target: avr
Configured with: ../gcc-4.9.2/configure --prefix=/home/scada/avr-gcc/ --target=avr --enable-languages=c,c++ --disable-nls --disable-libssp --with-dwarf2
Thread model: single
gcc version 4.9.2 (GCC)

```

## Building The Software ##

This command will build the firmware image, and upload it to the target device using 'avrdude'. 
```
make
```
This makefile was configured to run with the usbTinyISP AVR programmer.   
If you are not using a usbTinyISP programmer, you will need to change the following to match your programmer.  
```
vi Makefile
```

and modify the following line

```
PGRMR={your programmer}
```

See AVRDude User Manual above for a list of supported programmers.

## Programming The Microcontroller##
**To build the firmware image only:**  
```
make build
```
**To upload the last built firmware only:**  
```
make program
```
  

## Configuring the Onboard Crystal Oscillator##
This command must be run to configure the AVR device upon its first program. Once this command is issued, it should not need to be run again.  

The command below configures the device to use the proper clock source (16MHz crystal) instead of the internal oscillator.
```
make fuse
```
*NOTE: This command will throw a Device ID mis-match, this is normal. This is because the ATMEGA16M1 is present listed in the AVRDude list of devices*


can_drv.h/c
can_lib.h/c

Found at:
https://eewiki.net/display/microcontroller/CAN+Example+-+ATmega32M1+-+STK600

modified to compile w/ avr-gcc
