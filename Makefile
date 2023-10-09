CC=gcc
CFLAGS=-Wall -Werror=declaration-after-statement -O2 -std=gnu11 -lm -g

rl:
	$(CC) $(CFLAGS) ./src/$@.c -o ./bin/$@

cf:
	$(CC) $(CFLAGS) ./src/$@.c -o ./bin/$@

ee:
	$(CC) $(CFLAGS) ./src/$@.c -o ./bin/$@

de:
	$(CC) $(CFLAGS) ./src/$@.c -o ./bin/$@

ns:
	$(CC) $(CFLAGS) ./src/$@.c -o ./bin/$@

rps:
	$(CC) $(CFLAGS) ./src/$@.c -o ./bin/$@

fs:
	$(CC) $(CFLAGS) ./src/$@.c -o ./bin/$@

clean:
	rm -rf ./bin/*
