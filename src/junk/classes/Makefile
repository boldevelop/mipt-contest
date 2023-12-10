CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -O0 -std=c++20 -g
BIN_DIR = ./bin
TARGET_EXEC = a

all: $(TARGET_EXEC)
$(TARGET_EXEC):
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $@.cpp -o ./bin/$@