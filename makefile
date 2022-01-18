##### Beginning of user-defined parameters. #####

# Specify root directory of source code.
SOURCE_DIR := ./src
# Specify directories containing headers.
INCLUDE_FLAGS := -I./include
# Specify directory to store object files and final executable.
BUILD_DIR := ./build
# Specify source code of main program (main function is declared and defined).
MAIN := ./debug/cardio_debug.c
# Specify name of final executable.
TARGET := test.exe

# Specify C compiler.
CC := gcc
# Specify C compile flags.
CFLAGS := -Wall $(INCLUDE_FLAGS) 
# Specify linker flags.
LDFLAGS := -lm -lpthread

##### End of user-defined parameters. #####


# Generate names of all object files.
SOURCE_PATHS := $(shell find $(SOURCE_DIR) -name '*.c')
SOURCE_PATHS += $(MAIN)
OBJECT_FILES := $(SOURCE_PATHS:%=$(BUILD_DIR)/%.o)

# Link object files and external libraries.
$(BUILD_DIR)/$(TARGET) : $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) -o $@ $(LDFLAGS)

# Perform C compilation.
$(BUILD_DIR)/%.c.o : %.c
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ 

# Remove build directory with 'make clean'.
.PHONY : clean
clean:
	rm -rf $(BUILD_DIR)