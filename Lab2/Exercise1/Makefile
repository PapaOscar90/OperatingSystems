TARGET = shell
SOURCE_DIR := src
TEST_DIR := test
DOC_DIR := doc
CC_SOURCES := $(shell find $(SOURCE_DIR) -name "*.c")
TEST_SOURCES := $(shell find $(TEST_DIR) -name "test_*.c")
TEST_AUX_SOURCES := $(shell find $(SOURCE_DIR) -not -name "main.c" -name "*.c")

# Compiler
CC = gcc
CFLAGS = -Wall -std=gnu11 -Werror
C_TEST_FLAGS = -std=gnu11

default: $(TARGET)

all: default

.PHONY: shell
shell: $(SOURCES)
	$(CC) $(CFLAGS) $(CC_SOURCES) -o $@

.PHONY: test
test: $(TEST_SOURCES) $(TEST_AUX_SOURCES)
	for file in $(TEST_SOURCES); do \
		$(CC) $(C_TEST_FLAGS) $$file $(TEST_AUX_SOURCES) -o `basename "$$file" .c`;\
	done
	for file in $(TEST_SOURCES); do \
		./`basename "$$file" .c`;\
	done

.PHONY: doc
doc:
		doxygen $(DOC_DIR)/Doxyfile

.PHONY: clean
clean: src-clean doc-clean test-clean

.PHONY: test-clean
test-clean:
		-rm -f test_*

.PHONY: src-clean
src-clean:
		-rm -f $(TARGET)

.PHONY: doc-clean
doc-clean:
		-rm -rf doc/html doc/latex

.PHONY: submission
submission:
		-rm -f submission.zip && zip submission.zip doc/* Makefile src/* test/*
