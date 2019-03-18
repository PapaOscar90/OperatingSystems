TARGET = shell
SOURCE_DIR := src
TEST_DIR := test
DOC_DIR := doc
LEX_SOURCES := $(shell find $(SOURCE_DIR) -name "*.l")
YACC_SOURCES := $(shell find $(SOURCE_DIR) -name "*.y")
CC_SOURCES := $(shell find $(SOURCE_DIR) -name "*.c")
TEST_SOURCES := $(shell find $(TEST_DIR) -name "test_*.c")
TEST_AUX_SOURCES := $(shell find $(SOURCE_DIR) -not -name "main.c" -name "*.c")

# Lexer
LEX = flex
LFLAGS =

# Parser
YACC = bison
YFLAGS = -d

# Compiler
CC = gcc
CFLAGS = -Wall -std=gnu11 -Werror
C_TEST_FLAGS = -std=gnu11

default: $(TARGET)

all: default

shell: $(SOURCES)
	# $(YACC) $(YFLAGS) $(YACC_SOURCES)
	# $(LEX) $(LFLAGS) $(LEX_SOURCES)
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
		-rm -f $(TARGET) *.tab.h *.tab.c *.yy.c

.PHONY: doc-clean
doc-clean:
		-rm -rf doc/html doc/latex
