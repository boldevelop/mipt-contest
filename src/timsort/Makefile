BUILD_DIR=build
BIN_DIR=bin

VPATH=${BUILD_DIR}:${BIN_DIR}
.DEFAULT_GOAL=run

CC=gcc
CFLAGS=-std=gnu11\
	-Wall -Wextra -Wpedantic\
	-Werror=maybe-uninitialized\
	-Werror=declaration-after-statement
LDFLAGS=-lm

BIN_DEV=ts-d

obj=${patsubst %.c,%.o,$(wildcard *.c)}

$(BIN_DEV): CFLAGS += -g
$(BIN_DEV): CFLAGS += -O0
$(BIN_DEV): CFLAGS += -DDEBUG
$(BIN_DEV): $(obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(obj)) $(LDFLAGS)

%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $(BUILD_DIR)/$@

run: $(BIN_DEV)
	$(BIN_DIR)/$(BIN_DEV)

gdb: $(BIN_DEV)
	gdb $(BIN_DIR)/$(BIN_DEV)

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs
	VERSION_CONTROL=none indent ./*.h -kr -as --no-tabs

cleanobj:
	rm -rf $(BUILD_DIR)

clean: cleanobj
	rm -rf $(BIN_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)