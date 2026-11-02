# Simple Shell - Build and Test Automation Makefile
# Complies with ISO C11 standard and strict compiler diagnostic flags

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -g -Iinclude
LDFLAGS := 

TARGET  := simple-shell
BUILD   := build
SRC     := $(wildcard src/*.c)
OBJ     := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))

# Sanitizer flags for AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan)
SAN_CFLAGS  := $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer -g3
SAN_LDFLAGS := -fsanitize=address,undefined

.PHONY: all clean test asan asan-test ci debug info valgrind help

# Default target: compile the simple-shell executable
all: $(TARGET)

# Link simple-shell binary from compiled object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Compile C source files into object files inside build directory
$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build artifact directory if it does not exist
$(BUILD):
	mkdir -p $(BUILD)

# Compile parser unit test harness
test_parser: $(BUILD)/test_parser.o $(BUILD)/parser.o
	$(CC) $(CFLAGS) $^ -o $(BUILD)/test_parser $(LDFLAGS)

$(BUILD)/test_parser.o: tests/test_parser.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

test_executor: $(BUILD)/test_executor.o $(BUILD)/executor.o $(BUILD)/parser.o $(BUILD)/builtins.o
	$(CC) $(CFLAGS) $^ -o $(BUILD)/test_executor $(LDFLAGS)

$(BUILD)/test_executor.o: tests/test_executor.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Run unit tests and shell integration test scenarios
test: $(TARGET) test_parser test_executor
	@echo "=== Running Parser Unit Tests ==="
	./$(BUILD)/test_parser
	@echo "=== Running Executor Tests ==="
	./$(BUILD)/test_executor
	@echo "=== Running Shell Integration Test Suite ==="
	bash tests/test_shell.sh ./$(TARGET)

# Build simple-shell binary with AddressSanitizer and UndefinedBehaviorSanitizer
asan: clean
	$(MAKE) CFLAGS='$(SAN_CFLAGS)' LDFLAGS='$(SAN_LDFLAGS)' all

# Run full test suite with AddressSanitizer and UndefinedBehaviorSanitizer active
asan-test: clean
	$(MAKE) CFLAGS='$(SAN_CFLAGS)' LDFLAGS='$(SAN_LDFLAGS)' test

# Comprehensive CI target: clean, build standard, run tests, and run sanitizer tests
ci: clean all test asan-test
	@echo "All CI build, unit, integration, and sanitizer checks passed successfully."

# Run Valgrind memory leak analysis against parser and executor test harnesses
valgrind: test_parser test_executor
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(BUILD)/test_parser
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(BUILD)/test_executor

# Print compiler and environment information for debugging and CI reporting
info:
	@echo "=== Build Environment Information ==="
	@echo "Compiler: $(CC)"
	@$(CC) --version | head -n 1
	@echo "CFLAGS:   $(CFLAGS)"
	@echo "LDFLAGS:  $(LDFLAGS)"
	@echo "Sources:  $(SRC)"
	@echo "Objects:  $(OBJ)"

# Remove all compiled objects, binaries, and temporary files
clean:
	rm -rf $(BUILD) $(TARGET) $(TARGET).exe vgcore.* core.*

# Display help message describing available Makefile targets
help:
	@echo "Simple Shell Build Targets:"
	@echo "  make            - Build simple-shell binary"
	@echo "  make test       - Build and run unit and integration test suite"
	@echo "  make asan       - Build binary with AddressSanitizer and UBSan"
	@echo "  make asan-test  - Run full test suite under AddressSanitizer and UBSan"
	@echo "  make ci         - Execute full CI pipeline locally (clean, test, asan-test)"
	@echo "  make valgrind   - Run Valgrind leak detection on unit test harness"
	@echo "  make clean      - Remove build directory and compiled binaries"
	@echo "  make info       - Display compiler version and configuration flags"
