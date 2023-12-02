#================================================================================
# Shell configurations.
#================================================================================
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c

#================================================================================
# Make configurations.
#================================================================================
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --warn-undefined-variables
ifdef NUMBER_OF_PROCESSORS
	NUM_CORES := $(NUMBER_OF_PROCESSORS)
else
	NUM_CORES := 4
endif
MAKEFLAGS += -j$(NUM_CORES)

#================================================================================
# Define paths for project directory, define variables for build.
#================================================================================
SCRIPT_DIR = scripts
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.d,$(SRC_FILES))

#================================================================================
# Compiler configurations.
#================================================================================
TARGET := run.exe
CC := gcc
CFLAGS = -Wall -Werror -Wextra -Wunused-variable -std=c99 -I$(INC_DIR)
DFLAGS = -MMD -MP
OPT := -O2

#================================================================================
# Rule to invoke all rules necessary to produce the final executable.
#================================================================================
.PHONY: all
all: $(TARGET)

#================================================================================
# Rule to link object files into final executable.
#================================================================================
$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^

#================================================================================
# Rule to compile source files into object files.
#================================================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(DFLAGS) $(OPT) -c $< -o $@

#================================================================================
# Rule to remove build artefacts and outputs.
#================================================================================
.PHONY: clean
clean:
	@echo 'Cleaning project directory...'
	@rm -rf $(BUILD_DIR)/*.o
	@rm -rf $(BUILD_DIR)/*.d
	@rm -rf $(TARGET)

#================================================================================
# Rule for running Python script.
#================================================================================
.PHONY: py
py:
	@python $(SCRIPT_DIR)/main.py

#================================================================================
# Include the dependencies.
#================================================================================
-include $(DEP_FILES)