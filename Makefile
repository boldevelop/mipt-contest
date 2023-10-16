CC=gcc
CFLAGS=-Wall -Werror=declaration-after-statement -O0 -std=gnu11 -lm -g

SRC1=./src/1/

rl:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

cf:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

ee:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

de:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

ns:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

rps:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

fs:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

fm:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

pp:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

sf:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

mf:
	$(CC) $(CFLAGS) $(SRC1)$@.c -o ./bin/$@

hwf:
	$(CC) $(CFLAGS) $(SRC1)hwf/$@.c -o ./bin/$@

hwf-test:
	$(CC) $(CFLAGS) $(SRC1)hwf/hwf.c -o ./bin/$@ -DLOCAL_TEST

hwf-stress:
	$(CC) $(CFLAGS) $(SRC1)hwf/hwf.c -o ./bin/$@ -DLOCAL_STRESS_TEST


hwf.o:
	$(CC) $(CFLAGS) -o ./build/$@ -c $(SRC1)hwf/hwf.c

hwf-game.o:
	$(CC) $(CFLAGS) -o ./build/$@ -c $(SRC1)hwf/game.c

hwf-game: hwf-game.o hwf.o
	$(CC) $(CFLAGS) -o ./bin/$@ ./build/hwf-game.o ./build/hwf.o

lint:
	VERSION_CONTROL=none indent ./src/1/**/*.c -kr -as --no-tabs

clean:
	rm -rf ./bin/*
	rm -rf ./build/*
