VPATH = $(BUILD_DIR):$(BIN_DIR)

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror=maybe-uninitialized -Werror=declaration-after-statement -O0 -std=gnu11 -g -DDEBUG
LDFLAGS = -lm
BUILD_DIR = build
BIN_DIR = bin

bins := $(patsubst %.c,%.bin,$(wildcard *.c))

all: $(bins)

%.bin: %.c | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(basename $@).c $(CFLAGS) $(LDFLAGS)

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)