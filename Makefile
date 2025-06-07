# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2 -Iinclude
LDFLAGS :=

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN := server

# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN)

# Link objects
$(BIN): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build dir
$(BUILD_DIR):
	mkdir -p $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN)

# Rebuild
rebuild: 
	clean all

.PHONY: all clean rebuild
