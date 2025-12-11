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

clean:
	@rm -rf $(OUTPUT_DIR)

.PHONY: all run clean
