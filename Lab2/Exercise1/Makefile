CC = gcc
CFLAGS = -Wall -pedantic -Werror -std=c11
SOURCE_DIR := src

SOURCES := $(shell find $(SOURCE_DIR) -name "*.c")

shell: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

.phony: clean
clean:
				-rm shell
