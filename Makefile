CC          = gcc
CFLAGS      = -Wall -Wextra -Werror
DEBUG_FLAGS = -g

BIN          = bin
SRC          = src
OBJ          = obj
PREPROCESSED = preprocessed
LIB          = lib
TESTS        = tests
LIB_NAME     = oopc

SOURCES      = $(wildcard $(SRC)/*.c)
OBJECTS      = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SOURCES))
TEST_SOURCES = $(wildcard $(TESTS)/*.c)
TEST_OBJECTS = $(patsubst $(TESTS)/%.c,$(OBJ)/test_%.o,$(TEST_SOURCES))
TEST_BINS    = $(patsubst $(TESTS)/%.c,$(BIN)/test_%,$(TEST_SOURCES))

all: run_tests

.PHONY: clean folders obj debug lib lib_debug tests_obj tests run_tests preprocess

folders:
	mkdir -p $(BIN) $(OBJ) $(LIB) $(PREPROCESSED)

obj: $(SOURCES) folders
	$(CC) $(CFLAGS) -c $(SOURCES) -o $(OBJECTS)

debug: $(SOURCES) folders
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $(SOURCES) -o $(OBJECTS) -DDEBUG

lib: obj
	ar rcs $(LIB)/lib$(LIB_NAME).a $(OBJECTS)

lib_debug: debug
	ar rcs $(LIB)/lib$(LIB_NAME).a $(OBJECTS)

preprocess: $(TEST_SOURCES) folders
	for src_file in $(TEST_SOURCES); do \
		preprocessed_file=$(patsubst $(TESTS)/%.c,$(PREPROCESSED)/test_%.c,$$src_file); \
		$(CC) $(CFLAGS) -E $$src_file -o $$preprocessed_file; \
	done

tests_obj: $(TEST_SOURCES) folders
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $(TEST_SOURCES) -o $(TEST_OBJECTS)

tests: lib tests_obj
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -I$(SRC) -L$(LIB) -l$(LIB_NAME) $(TEST_SOURCES) -o $(TEST_BINS)

run_tests: tests
	for test in $(TEST_BINS); do \
		echo "======= Running $$test ======="; \
		./$$test; \
		echo ""; \
	done

clean:
	rm -rf $(BIN)/* $(OBJ)/* $(LIB)/* $(PREPROCESSED)/*
