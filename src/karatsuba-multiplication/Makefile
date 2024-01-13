BUILD_DIR = build
BIN_DIR = bin

VPATH = $(BUILD_DIR):$(BIN_DIR)
.DEFAULT_GOAL = kmp

CC = gcc
CFLAGS = -std=gnu11\
	-Wall -Wextra -Wpedantic -Werror=maybe-uninitialized\
	-Werror=declaration-after-statement
LDFLAGS = -lm

BIN_DEV = kmp-dev

obj = $(patsubst %.c,%.o,$(wildcard *.c))
app_obj = $(filter-out test.o,$(obj))
tst_obj = $(filter-out main.o,$(obj))

kmp: CFLAGS += -O2
kmp: CFLAGS += -DNDEBUG
kmp: cleanobj $(app_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/kmp $(addprefix $(BUILD_DIR)/,$(app_obj)) $(LDFLAGS)

$(BIN_DEV): CFLAGS += -g
$(BIN_DEV): CFLAGS += -O0
$(BIN_DEV): CFLAGS += -DDEBUG
$(BIN_DEV): $(app_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(app_obj)) $(LDFLAGS)

kmp-stress: CFLAGS += -g
kmp-stress: CFLAGS += -O2
kmp-stress: CFLAGS += -DDEBUG
kmp-stress: cleanobj $(app_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(app_obj)) $(LDFLAGS)

kmp-unit: CFLAGS += -g
kmp-unit: CFLAGS += -O0
kmp-unit: CFLAGS += -DDEBUG
kmp-unit: CFLAGS += -DDEGRADATION_TO_NAIVE=4
kmp-unit: $(tst_obj) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BUILD_DIR)/,$(tst_obj)) $(LDFLAGS)

mem.o: mem.h
io.o: io.h
main.o: polynom.h io.h
polynom.o: mem.h io.h polynom.h
test.o: polynom.h

%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $(BUILD_DIR)/$@

#  ‘$<’ is just the first prerequisite

run: $(BIN_DEV)
	$(BIN_DIR)/$(BIN_DEV)

run5: $(BIN_DEV)
	$(BIN_DIR)/$(BIN_DEV) < ./in/5

unit: kmp-unit
	valgrind $(BIN_DIR)/kmp-unit

test: $(BIN_DEV)
	$(BIN_DIR)/$(BIN_DEV) < ./in/2 > ./bin/2.out
	diff ./out/2 ./bin/2.out
	$(BIN_DIR)/$(BIN_DEV) < ./in/3 > ./bin/3.out
	diff ./out/3 ./bin/3.out
	$(BIN_DIR)/$(BIN_DEV) < ./in/5 > ./bin/5.out
	diff ./out/5 ./bin/5.out

valgrind: $(BIN_DEV)
	valgrind $(BIN_DIR)/$(BIN_DEV) < ./in/2
	@echo "- - -"
	valgrind $(BIN_DIR)/$(BIN_DEV) < ./in/3
	@echo "- - -"
	valgrind $(BIN_DIR)/$(BIN_DEV) < ./in/5

gdb: $(BIN_DEV)
	gdb $(BIN_DIR)/$(BIN_DEV)

STRESS_D = 18
stress-input: | $(BIN_DIR)
	python3 ./make-test.py $(STRESS_D)

stress: stress-input kmp-stress
	time $(BIN_DIR)/kmp-stress < $(BIN_DIR)/$(STRESS_D) > /dev/null

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