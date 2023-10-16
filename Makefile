CC=gcc
CFLAGS=-Wall -Werror=declaration-after-statement -O0 -std=gnu11 -lm -g

SRC1=./src/1/

mkbin:
	mkdir -p ./bin

rl: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

cf: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

ee: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

de: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

ns: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

rps: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

fs: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

fm: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

pp: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

sf: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

mf: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

hwf: mkbin
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

hwf-test: mkbin
	$(CC) $(CFLAGS) $(SRC1)hwf.c -o ./bin/hwf -DLOCAL_TEST

lint:
	VERSION_CONTROL=none indent ./src/**/*.c -kr -as --no-tabs

clean:
	rm -rf ./bin/*
