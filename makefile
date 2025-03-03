# Compiler and flags
# Compiler: GNU Compiler Collection
CC = gcc
# Flags: It should provide all possible waarnings plus good optimization
CFLAGS   = -g -grecord-gcc-switches -O2 -Wall -Wextra -pedantic
CFLAGS  += -I./include

# Source files
SRC = src/main.cpp src/hardware.cpp src/utils.cpp
# Output executable
OUT = bin/main

#Arguments, default values
SCANS_PER_SWIPE = 10
MAX_MEMORY = 1020
INITIAL_CAPACITY = 255

# For when testing on a computer without a board
# Fakes importing the arduino functions
TESTING := true
ifeq ($(TESTING), true)
	CFLAGS += -I./fake
endif

# Default target
all: $(OUT)

# Build the executable
# You can change the arguments by defining them after `make`. E.g.:
# make SCANS_PER_SWIPE=5 MAX_MEMORY=100 INITIAL_CAPACITY=50
$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT) -DSCANS_PER_SWIPE=$(SCANS_PER_SWIPE) -DMAX_MEMORY=$(MAX_MEMORY) -DINITIAL_CAPACITY=$(INITIAL_CAPACITY) -DTESTING=$(TESTING)