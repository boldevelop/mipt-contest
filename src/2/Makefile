CC = gcc
CFLAGS = -Wall -Werror=declaration-after-statement -O0 -std=gnu11 -lm -g
BUILD_DIR = ./build
BIN_DIR = ./bin

SRC = pn se

all: $(SRC)
$(SRC):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $@.c -o $(BIN_DIR)/$@

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
