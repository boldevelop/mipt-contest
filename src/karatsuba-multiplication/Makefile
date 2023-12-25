CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror=maybe-uninitialized -Werror=declaration-after-statement -O0 -std=gnu11 -g -DDEBUG
CFLAG_STRESS = -Wall -Wextra -Wpedantic -Werror=maybe-uninitialized -Werror=declaration-after-statement -O2 -std=gnu11 -DDEBUG
LDFLAGS = -lm
BUILD_DIR = ./build
BIN_DIR = ./bin

BIN_NAME = kmp

SRC = main polynom mem io
OBJS = $(SRC:%=$(BUILD_DIR)/%.o)

all: kmp

kmp:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) -c polynom.c -o $(BUILD_DIR)/polynom.o
	$(CC) $(CFLAGS) -c mem.c -o $(BUILD_DIR)/mem.o
	$(CC) $(CFLAGS) -c io.c -o $(BUILD_DIR)/io.o
	$(CC) $(OBJS) -o $(BIN_DIR)/$@ $(LDFLAGS)

kmp-stress:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAG_STRESS) -c main.c -o $(BUILD_DIR)/main.o
	$(CC) $(CFLAG_STRESS) -c polynom.c -o $(BUILD_DIR)/polynom.o
	$(CC) $(CFLAG_STRESS) -c mem.c -o $(BUILD_DIR)/mem.o
	$(CC) $(CFLAG_STRESS) -c io.c -o $(BUILD_DIR)/io.o
	$(CC) $(OBJS) -o $(BIN_DIR)/$@ $(LDFLAGS)

run: kmp
	$(BIN_DIR)/$(BIN_NAME)

run5: kmp
	$(BIN_DIR)/$(BIN_NAME) < ./in/5

test: kmp
	$(BIN_DIR)/$(BIN_NAME) < ./in/2 > ./bin/2.out
	diff ./out/2 ./bin/2.out
	$(BIN_DIR)/$(BIN_NAME) < ./in/3 > ./bin/3.out
	diff ./out/3 ./bin/3.out
	$(BIN_DIR)/$(BIN_NAME) < ./in/5 > ./bin/5.out
	diff ./out/5 ./bin/5.out

valgrind: kmp
	valgrind $(BIN_DIR)/$(BIN_NAME) < ./in/2
	@echo "- - -"
	valgrind $(BIN_DIR)/$(BIN_NAME) < ./in/3
	@echo "- - -"
	valgrind $(BIN_DIR)/$(BIN_NAME) < ./in/5

gdb: kmp
	gdb $(BIN_DIR)/$(BIN_NAME)


STRESS_D = 18
stress-input:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	python3 ./make-test.py $(STRESS_D)

stress: stress-input kmp-stress
	time $(BIN_DIR)/kmp-stress < ./bin/$(STRESS_D) > /dev/null

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs
	VERSION_CONTROL=none indent ./*.h -kr -as --no-tabs

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
