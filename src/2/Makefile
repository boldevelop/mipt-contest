CC = gcc
CFLAGS = -Wall -Werror=maybe-uninitialized -Werror=declaration-after-statement -O0 -std=gnu11 -g -DDEBUG
LDFLAGS = -lm
BUILD_DIR = ./build
BIN_DIR = ./bin

SRC = pn se ts ps gf cc lm bp bi pe ft pf

all: $(SRC)
$(SRC):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $@.c -o $(BIN_DIR)/$@ $(LDFLAGS)

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
