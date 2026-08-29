CC      := g++
CFLAGS  := $(shell pkg-config --cflags sdl2) -Wall -Wextra -Iinclude
LIBS    := $(shell pkg-config --libs sdl2)

MAIN    := main.cpp
SOURCES := $(wildcard src/*.cpp)
TARGET  := out

.PHONY: compile debug clean compile_commands

compile_commands:
	@printf '[\n' > compile_commands.json; \
	first=1; \
	for f in main.cpp src/*.cpp; do \
		[ $$first -eq 1 ] || printf ',\n' >> compile_commands.json; \
		first=0; \
		printf '  {\n    "directory": "%s",\n    "command": "g++ %s -c %s",\n    "file": "%s"\n  }' \
			"$(CURDIR)" "$(CFLAGS)" "$$f" "$$f" >> compile_commands.json; \
	done; \
	printf '\n]\n' >> compile_commands.json; \
	echo "Generated compile_commands.json"

compile:
	$(CC) $(CFLAGS) $(MAIN) $(SOURCES) -o $(TARGET) $(LIBS)
	@echo "Built $(TARGET)" && ./$(TARGET)

debug:
	$(CC) $(CFLAGS) -g -O0 $(MAIN) $(SOURCES) -o $(TARGET) $(LIBS)
	@echo "Built $(TARGET) (debug)"

clean:
	rm -f $(TARGET)
	@echo "Cleaned"
