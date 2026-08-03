CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -pedantic -g

SRC_DIR := src
BIN_DIR := bin

SOURCES := $(wildcard $(SRC_DIR)/*.c)
TARGETS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SOURCES))

.PHONY: all test clean

all: $(TARGETS)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Each source file is a standalone program with its own main().
$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

test: all
	@./run_tests.sh

clean:
	@rm -rf $(BIN_DIR)
	@echo "Removed $(BIN_DIR)/"
