CC = gcc
CFLAGS = -Os -g -Wall -std=c99 -mmcu=$(AVR)
LDFLAGS = -lcunit

BIN = unit_test

BIN_DIR = ./bin
OBJ_DIR = ./obj

SRC_DIR = ../src

SRC_FILES = \
	ringbuf.c \
	can_frame.c


TEST_DIR = test

TEST_FILES = \
	ringbuf_test.c \
	can_frame_test.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
TEST = $(addprefix $(TEST_DIR)/, $(TEST_FILES))
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o,$(SRC))

default: $(BIN)

run_tests: $(BIN)
	./$(BIN_DIR)/$(BIN)

dir_check:
	if [ ! -d $(BIN_DIR)]; then
		mkdir $(BIN_DIR)
	fi
	if [ ! -d $(OBJ_DIR)]; then
		mkdir $(BIN_DIR)
	fi

#Generate executable File
$(BIN):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN_DIR)/$(ELF)
	
#Compile Object Files
$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin/*.hex bin/*.elf obj/*.o

.PHONY: fuse clean
