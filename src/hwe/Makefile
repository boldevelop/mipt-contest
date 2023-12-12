CC = gcc
CFLAGS = -Wall -Werror=maybe-uninitialized -Werror=declaration-after-statement -O0 -std=gnu11 -g -DDEBUG
LDFLAGS = -lm
BUILD_DIR = ./build
BIN_DIR = ./bin

SRC = hwe

SRC = main sieve bit-sieve
OBJS = $(SRC:%=$(BUILD_DIR)/%.o)

all: hwe

.PHONY: run
run: hwe
	$(BIN_DIR)/$<

hwe:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) -c sieve.c -o $(BUILD_DIR)/sieve.o
	$(CC) $(CFLAGS) -c bit-sieve.c -o $(BUILD_DIR)/bit-sieve.o
	$(CC) $(OBJS) -o $(BIN_DIR)/$@ $(LDFLAGS)

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
