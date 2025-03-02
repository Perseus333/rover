# Compiler and flags
# Compiler: GNU Compiler Collection
CC = gcc
# Flags: It should provide all possible waarnings
CFLAGS = -g -grecord-gcc-switches -O2 -Wall -Wextra -pedantic

# Source files
SRC = src/main.cpp src/utils.cpp src/pathfinding.cpp
# Output executable
OUT = bin/main

# Default target
all: $(OUT)

# Build the executable
$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)