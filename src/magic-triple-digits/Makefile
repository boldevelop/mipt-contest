CC = gcc
CFLAGS = -Wall -Werror=declaration-after-statement -O0 -std=gnu11 -lm -g
CFLAGSD =
# CFLAGS = -Wall -O0 -std=gnu11 -lm -g
BUILD_DIR = ./build
BIN_DIR = ./bin

SRC = main combinatorics mtd_mem
OBJS = $(SRC:%=$(BUILD_DIR)/%.o)

all: mtd

.PHONY: run
run: mtd
	$(BIN_DIR)/$<

mtd: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(BIN_DIR)/$@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGSD) -c $< -o $@

mtd-research:
	make mtd CFLAGSD=-DRESEARCH

mtd-research-run: mtd-research
	$(BIN_DIR)/mtd > research.data

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

clean:
	rm -rf ./bin/*
	rm -rf ./build/*

# targets: prerequisites
#     command

# targets...: target-pattern: prereq-patterns ...
#     commands

# f1.o f2.o:
#    echo $@

# the commands will be run for each target. $@
# $^ Outputs all prerequisites
# $< The name of the first prerequisite
