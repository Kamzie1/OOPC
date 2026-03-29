CC=gcc
FLAGS=-Wall -Wextra -Werror
DEBUG=-g

BIN=bin
SRC=src
OBJ=obj
PREPROCESSED=preprocessed
LIB=lib
TESTS=tests
LIBNAME=oopc

SOURCES=$(wildcard $(SRC)/*.c)
OBJECTS=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SOURCES))
TEST_SOURCES=$(wildcard $(TESTS)/*.c)
TEST_OBJECTS=$(patsubst $(TESTS)/%.c,$(OBJ)/test_%.o,$(TEST_SOURCES))
TEST_BINARIES=$(patsubst $(TESTS)/%.c,$(BIN)/test_%,$(TEST_SOURCES))

all: run_tests

.PHONY: clean folders obj debug lib lib_debug tests_obj tests run_tests, preprocess

folders:
	mkdir -p $(BIN) $(OBJ) $(LIB) $(PREPROCESSED)

obj: $(SOURCES) folders
	$(CC) $(FLAGS) -c $(SOURCES) -o $(OBJECTS)

debug: $(SOURCES) folders
	$(CC) $(FLAGS) $(DEBUG) -c $(SOURCES) -o $(OBJECTS) -DDEBUG

lib: obj
	ar rcs $(LIB)/lib$(LIBNAME).a $(OBJECTS)

lib_debug: debug
	ar rcs $(LIB)/lib$(LIBNAME).a $(OBJECTS)

preprocess: $(TEST_SOURCES) folders
	for src_file in $(TEST_SOURCES); do \
		preprocessed_file=$(patsubst $(TESTS)/%.c,$(PREPROCESSED)/test_%.c,$$src_file); \
		$(CC) $(FLAGS) -E $$src_file -o $$preprocessed_file; \
	done

tests_obj: $(TEST_SOURCES) folders
	$(CC) $(FLAGS) $(DEBUG) -c $(TEST_SOURCES) -o $(TEST_OBJECTS)

tests: lib tests_obj
	$(CC) $(FLAGS) $(DEBUG) -I$(SRC) -L$(LIB) -l$(LIBNAME) $(TEST_SOURCES) -o $(TEST_BINARIES)

run_tests: tests
	for test in $(TEST_BINARIES); do \
		echo "======= Running $$test ======="; \
		./$$test; \
		echo ""; \
	done

clean:
	rm -rf $(BIN) $(OBJ) $(LIB)
