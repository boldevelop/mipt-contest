BUILD_DIR=build
BIN_DIR=bin

VPATH=${BUILD_DIR}:${BIN_DIR}
.DEFAULT_GOAL=hm

CC=gcc
CFLAGS=-std=gnu11\
	-Wall -Wextra -Wpedantic\
	-Werror=maybe-uninitialized\
	-Werror=declaration-after-statement
LDFLAGS=-lm

BIN_DEV=hm-d
BIN_TST=hm-tst

obj=${patsubst %.c,%.o,$(wildcard *.c)}
app_obj = $(filter-out test.o,$(obj))
tst_obj = $(filter-out main.o,$(obj))

# prod
hm: CFLAGS += -O2
hm: CFLAGS += -DNDEBUG
hm: cleanobj $(app_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(app_obj)) $(LDFLAGS)

# dev
$(BIN_DEV): CFLAGS += -g
$(BIN_DEV): CFLAGS += -O0
$(BIN_DEV): CFLAGS += -DDEBUG
$(BIN_DEV): $(app_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(app_obj)) $(LDFLAGS)

# test
$(BIN_TST): CFLAGS += -g
$(BIN_TST): CFLAGS += -O0
$(BIN_TST): CFLAGS += -DDEBUG
$(BIN_TST): $(tst_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(tst_obj)) $(LDFLAGS)

main.o: hashmap.h
test.o: hashmap.h

%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $(BUILD_DIR)/$@

run: $(BIN_DEV)
	$(BIN_DIR)/$(BIN_DEV)

run-test: $(BIN_TST)
	$(BIN_DIR)/$(BIN_TST)

valgrind: $(BIN_DEV)
	valgrind $(BIN_DIR)/$(BIN_DEV) < ./in/1.dat

valgrind-test: $(BIN_TST)
	valgrind $(BIN_DIR)/$(BIN_TST)

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
