CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all clean run

# Filter MAKECMDGOALS to find the executable name
# We remove 'run' and 'clean' from the list of command line goals.
# The remaining item should be the desired executable name (e.g., 'is_palindrome').
# 'filter-out' removes specific words from the list.
EXEC_GOALS = $(filter-out run clean all, $(MAKECMDGOALS))

# If an executable is specified (i.e., EXEC_GOALS is not empty), use it for TARGET.
# Otherwise, fall back to the default 'main'.
BUILD_DIR = build
TARGET = $(strip $(if $(EXEC_GOALS), $(EXEC_GOALS), main))
SOURCES = $(TARGET).c util.c
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(BUILD_DIR)/$(TARGET)
	@echo "Build successful! Executable in $(BUILD_DIR)/"

$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 3. Rule to compile source files into object files inside the BUILD_DIR
# This is a Pattern Rule that matches all files ending in .o inside the build folder.
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 4. Run the executable from the BUILD_DIR
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

# 5. Updated clean rule
clean:
	rm -rf $(BUILD_DIR)
