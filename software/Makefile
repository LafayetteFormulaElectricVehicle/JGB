AVR=atmega16m1
CC=avr-gcc
CFLAGS= -Os -g -Wall -std=c99 -mmcu=$(AVR)

AVRDUDE=avrdude
PRGMR=usbtiny
#PRGMR=dragon_isp -P usb
AVR_AVRD=m16

ELF=lfevcan_16m1.elf
HEX=lfevcan_16m1.hex

BIN_DIR = bin
OBJ_DIR = obj

SRC_DIR = src

SRC_FILES = \
	main.c \
	adc.c \
	timer.c \
	can_lib.c \
	can_drv.c \
	uart.c \
	ringbuf.c \


TEST_DIR = test

TEST_FILES = \
	ringbuf_test.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
TEST = $(addprefix $(TEST_DIR)/, $(TEST_FILES))
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o,$(SRC))

all: program

#Generate Hex File
$(HEX):$(ELF)
	avr-objcopy -j .text -j .data -O ihex $(BIN_DIR)/$(ELF) $(BIN_DIR)/$(HEX)
	
#Link Object Files
$(ELF):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN_DIR)/$(ELF)
	
#Compile Object Files
$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


#Program Device
program: $(HEX)
	$(AVRDUDE) -F -V -c $(PRGMR) -p $(AVR_AVRD) -U flash:w:$(BIN_DIR)/$(HEX)

#PHONY
fuse:
	$(AVRDUDE) -c $(PRGMR) -p $(AVR_AVRD) -F -U lfuse:w:0xef:m

clean:
	rm -rf bin/*.hex bin/*.elf obj/*.o

.PHONY: fuse clean
