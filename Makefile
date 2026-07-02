CC = gcc
CFLAGS = -Wall -Wextra -O3 -DNDEBUG
LFLAGS =

TARGET = name

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(OBJ_DIR):
	mkdir -p $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# debug: CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g -O0 -DDEBUG
# debug: clean all

# install: $(TARGET)
# 	install -d $(DESTDIR)$(PREFIX)/bin
# 	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

# uninstall:
#   rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)

.PHONY: all clean # debug install uninstall
