# ✨ Daniël Kamp's supersonic Makefile, oh yeah ✨

CC=g++
CFLAGS = -I/usr/local/include -Wall -std=c++1z
LDFLAGS= -ljack

SRC_DIR=src
OBJ_DIR=obj
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Make all
all: _localrun

# Link all
_localrun: $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

# Separate rule for main.cpp since it doesn't have a header
obj/main.o: src/main.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

# Make all .cpp source files into .o files, like magic ✨
obj/%.o: src/%.cpp src/%.h
	$(CC) -c $< $(CFLAGS) -o $@

# Install to system path
install:
	mv synth /usr/var/bin/

# Clean up
clean:
	rm synth *.o