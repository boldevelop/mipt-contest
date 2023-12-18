CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror=maybe-uninitialized -Werror=declaration-after-statement -O0 -std=gnu11 -g -DDEBUG
CFLAG_STRESS = -Wall -Wextra -Wpedantic -Werror=maybe-uninitialized -Werror=declaration-after-statement -O2 -std=gnu11 -DDEBUG
LDFLAGS = -lm
BUILD_DIR = ./build
BIN_DIR = ./bin

SRC = main

all: $(SRC)
$(SRC):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $@.c -o $(BIN_DIR)/$@ $(LDFLAGS)

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

stress-bin:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAG_STRESS) main.c -o $(BIN_DIR)/main-stress $(LDFLAGS)

run: all
	$(BIN_DIR)/main

run8: all
	$(BIN_DIR)/main < ./in/8

test: all
	$(BIN_DIR)/main < ./in/4
	$(BIN_DIR)/main < ./in/8
	$(BIN_DIR)/main < ./in/32

valgrind: all
	valgrind $(BIN_DIR)/main < ./in/4
	valgrind $(BIN_DIR)/main < ./in/8
	valgrind $(BIN_DIR)/main < ./in/32

gdb: all
	gdb $(BIN_DIR)/main

STRESS_D = 18
make-test:
	python3 ./make-test.py $(STRESS_D)

stress: make-test stress-bin
	time $(BIN_DIR)/main < ./bin/$(STRESS_D) > /dev/null

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
