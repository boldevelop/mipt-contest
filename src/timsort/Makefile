BUILD_DIR=build
BIN_DIR=bin

VPATH=${BUILD_DIR}:${BIN_DIR}
.DEFAULT_GOAL=ts

PERF=/usr/lib/linux-tools/5.15.0-91-generic/perf
CC=gcc
CFLAGS=-std=gnu11\
	-Wall -Wextra -Wpedantic\
	-Werror=maybe-uninitialized\
	-Werror=declaration-after-statement
LDFLAGS=-lm

BIN_DEV=ts-d

obj=${patsubst %.c,%.o,$(wildcard *.c)}
app_obj = $(filter-out test.o,$(obj))
tst_obj = $(filter-out main.o,$(obj))

# prod
ts: CFLAGS += -O2
ts: cleanobj $(app_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(app_obj)) $(LDFLAGS)

# for tests
$(BIN_DEV): CFLAGS += -g
# $(BIN_DEV): CFLAGS += -ggdb3 -fno-omit-frame-pointer
$(BIN_DEV): CFLAGS += -O0
# $(BIN_DEV): CFLAGS += -O2
$(BIN_DEV): CFLAGS += -DDEBUG
$(BIN_DEV): $(tst_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(tst_obj)) $(LDFLAGS)

perf-record:
	 $(PERF) record -c 1000 -g $(BIN_DIR)/$(BIN_DEV)
	 $(PERF) report  -g

perf-stat:
	$(PERF) stat $(BIN_DIR)/$(BIN_DEV)

main.o: timsort.h
test.o: timsort.h

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
