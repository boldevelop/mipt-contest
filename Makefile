CC = gcc
CFLAGS = -Wall -Werror=declaration-after-statement -O0 -std=gnu11 -lm -g
BUILD_DIR = ./build


SRC1 = ./src/1/

TARGET_EXEC1 = cf ee de ns rps fs fm pp sf mf
contest1: $(TARGET_EXEC1)
$(TARGET_EXEC1):
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

# hwf
hwf:
	$(CC) $(CFLAGS) $(SRC1)hwf/$@.c -o ./bin/$@

hwf-test:
	$(CC) $(CFLAGS) $(SRC1)hwf/hwf.c -o ./bin/$@ -DLOCAL_TEST

hwf-stress:
	$(CC) $(CFLAGS) $(SRC1)hwf/hwf.c -o ./bin/$@ -DLOCAL_STRESS_TEST

# hwf game
$(BUILD_DIR)/hwf-io.o:  $(SRC1)hwf/hwf-io.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/hwf.o: $(SRC1)hwf/hwf.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/hwf-game.o: $(SRC1)hwf/hwf-game.c
	$(CC) $(CFLAGS) -o $@ -c $<

hwf-game: $(BUILD_DIR)/hwf-game.o $(BUILD_DIR)/hwf.o $(BUILD_DIR)/hwf-io.o
	$(CC) $(CFLAGS) -o ./bin/$@ ./build/hwf-game.o ./build/hwf.o ./build/hwf-io.o


lint:
	VERSION_CONTROL=none indent ./src/1/**/*.c -kr -as --no-tabs

clean:
	rm -rf ./bin/*
	rm -rf ./build/*
