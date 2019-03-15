TARGET = shell
SOURCE_DIR := src
SOURCES := $(shell find $(SOURCE_DIR) -name "*.c")

# Lexer
LEX = flex
LFLAGS = -Cf

# Parser
YACC = bison
YFLAGS = -d

# Compiler
CC = gcc
CFLAGS = -Wall -pedantic -std=gnu11 -Werror


default: $(TARGET)

all: default

shell: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

.phony: clean
clean:
				-rm shell
