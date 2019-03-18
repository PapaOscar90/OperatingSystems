TARGET = shell
SOURCE_DIR := src
TEST_DIR := test
DOC_DIR := doc
LEX_SOURCES := $(shell find $(SOURCE_DIR) -name "*.l")
YACC_SOURCES := $(shell find $(SOURCE_DIR) -name "*.y")
CC_SOURCES := $(shell find $(SOURCE_DIR) -name "*.c")
TEST_SOURCES := $(shell find $(TEST_DIR) -name "*.c")

# Lexer
LEX = flex
LFLAGS =

# Parser
YACC = bison
YFLAGS = -d

# Compiler
CC = gcc
CFLAGS = -Wall -std=gnu99 -Werror
C_TEST_FLAGS = -std=gnu99

default: $(TARGET)

all: default

shell: $(SOURCES)
	# $(YACC) $(YFLAGS) $(YACC_SOURCES)
	# $(LEX) $(LFLAGS) $(LEX_SOURCES)
	$(CC) $(CFLAGS) $(CC_SOURCES) -o $@

.PHONY: test
test: $(TEST_SOURCES)
	for file in $(TEST_SOURCES); do \
		$(CC) $(C_TEST_FLAGS) $$file -o test_`basename "$$file" .c`;\
	done
	for file in $(TEST_SOURCES); do \
		./test_`basename "$$file" .c`;\
	done

.PHONY: doc
doc:
		doxygen $(DOC_DIR)/Doxyfile

.PHONY: clean
clean: src-clean doc-clean
.PHONY: src-clean
src-clean:
		-rm $(TARGET) *.tab.h *.tab.c *.yy.c test_*

.PHONY: doc-clean
doc-clean:
		-rm -r doc/html doc/latex
