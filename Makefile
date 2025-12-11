CC = clang
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
  
# the name to use for both the target source file, and the output file:
OUTPUT_DIR = ./dist
SRC_DIR = ./src
TARGET = main
  
all: $(TARGET)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(TARGET): $(SRC_DIR)/$(TARGET).c $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(TARGET) $(SRC_DIR)/$(TARGET).c 

run: $(TARGET)
	$(OUTPUT_DIR)/$(TARGET)

clean:
	rm -rf $(OUTPUT_DIR)
