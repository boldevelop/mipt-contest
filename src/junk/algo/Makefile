CXX = g++
CXXFLAGS = -Wall -O0 -std=c++20 -g

BIN_DIR = ./bin

SRC = b c d e

all: $(SRC)
$(SRC):
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $@.cpp -o $(BIN_DIR)/$@

lint:
	VERSION_CONTROL=none indent ./*.c -kr -as --no-tabs

clean:
	rm -rf $(BIN_DIR)/*
