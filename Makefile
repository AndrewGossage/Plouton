CC = clang
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
  
# the name to use for both the target source file, and the output file:
OUTPUT_DIR = ./dist
SRC_DIR = ./src
TARGET = main
EXECUTABLE = $(OUTPUT_DIR)/$(TARGET)
  
all: $(EXECUTABLE)

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

$(EXECUTABLE): $(SRC_DIR)/$(TARGET).c $(OUTPUT_DIR)
	@echo "Compiling $(TARGET)..."
	@$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRC_DIR)/$(TARGET).c
	@echo "Done!"

run: $(EXECUTABLE)
	@$(EXECUTABLE)

clean:
	@rm -rf $(OUTPUT_DIR)

.PHONY: all run clean
