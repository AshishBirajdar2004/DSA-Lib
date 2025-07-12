CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

LIB = libdsa.a
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, %.o, $(SRC))

# where to install (standard user-wide defaults)
PREFIX = /usr/local
INCLUDE_DIR = $(PREFIX)/include/dsa-lib
LIB_DIR = $(PREFIX)/lib

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^
	rm -f *.o

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

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
	rm -f *.o *.out $(LIB)

.PHONY: all clean
