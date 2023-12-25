CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror=maybe-uninitialized -Werror=declaration-after-statement -O0 -std=gnu11 -g -DDEBUG
LDFLAGS = -lm
BUILD_DIR = ./build
BIN_DIR = ./bin

SRC = al rbs hl hl2 rng lr ipo spo is no tt

all: $(SRC)
$(SRC):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $@.c -o $(BIN_DIR)/$@ $(LDFLAGS)

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

valgrind-al: all
	valgrind $(BIN_DIR)/al

valgrind-rbs: all
	valgrind --leak-check=full $(BIN_DIR)/rbs

valgrind-lr: all
	valgrind --leak-check=full $(BIN_DIR)/lr

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
