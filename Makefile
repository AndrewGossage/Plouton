CC = clang
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
  
# the name to use for both the target source file, and the output file:
OUTPUT_DIR = ./dist
SRC_DIR = ./src
TARGET = plouton
EXECUTABLE = $(OUTPUT_DIR)/$(TARGET)
DEBUGGER = lldb
# Find all .c files in src directory
SOURCES != ls $(SRC_DIR)/*.c 

all: $(EXECUTABLE)

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

$(EXECUTABLE): $(SOURCES) $(OUTPUT_DIR)
	@echo "Compiling $(TARGET)..."
	@$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)
	@echo "Done!"

run: $(EXECUTABLE)
	@$(EXECUTABLE)

debug: $(EXECUTABLE)
	@$(DEBUGGER) $(EXECUTABLE)


clean:
	@rm -rf $(OUTPUT_DIR)


test: $(EXECUTABLE)
	@for test_file in $(wildcard tests/*); do \
		echo "Running test: $$test_file"; \
		$(EXECUTABLE) $$test_file || exit 1; \
	done
	@echo "All tests passed!"


.PHONY: all run clean
