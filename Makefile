CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

LIB = libdsa.a
SRC_DIR = src
BUILD_DIR = .build
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
EXAMPLES = $(wildcard examples/*.c)
EXE = $(patsubst examples/%.c, $(BUILD_DIR)/%, $(EXAMPLES))

# === Build static lib ===
all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# === Build all examples ===
examples: $(EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%: examples/%.c $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -L. -ldsa -o $@

# === Install / Uninstall ===
PREFIX = /usr/local
INCLUDE_DIR = $(PREFIX)/include/dsa-lib
LIB_DIR = $(PREFIX)/lib

install: $(LIB)
	@echo "Installing headers to $(INCLUDE_DIR)..."
	mkdir -p $(INCLUDE_DIR)
	cp -r include/* $(INCLUDE_DIR)/
	@echo "Installing static library to $(LIB_DIR)..."
	cp -r $(LIB) $(LIB_DIR)/

uninstall:
	@echo "Removing installed headers from $(INCLUDE_DIR)..."
	rm -rf $(INCLUDE_DIR)
	@echo "Removing static library from $(LIB_DIR)..."
	rm -f $(LIB_DIR)/$(LIB)

clean:
	rm -rf $(BUILD_DIR) $(LIB)

.PHONY: all examples clean install uninstall
