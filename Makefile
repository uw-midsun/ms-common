CC := gcc
CFLAGS := -I . -Wall -Wextra -Werror

TARGET := program
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
DIRS := $(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR)

SRC_C := $(wildcard *.c)
DEP_C := $(wildcard *.h)

OBJ := $(addprefix $(OBJ_DIR)/,$(SRC_C:.c=.o))

.PHONY: all directories run clean

all: $(TARGET)

directories: $(DIRS)

run:
	@./$(BIN_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

$(DIRS):
	@mkdir -p $@

$(OBJ_DIR)/%.o: %.c $(DEP_C) | $(DIRS)
	@echo "$(CC): $(notdir $<) -> $(notdir $@)"
	@$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	@echo "$(CC): Linking $(TARGET)"
	@$(CC) -o $(BIN_DIR)/$(TARGET) $^
