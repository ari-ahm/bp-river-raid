TARGET = prog
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
CXX = gcc
CFLAGS = -g -std=c11 -O2 -Wall -Wextra

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(shell find . -type f -name '*.c'))
HEADERS = $(shell find . -type f -name '*.h')

%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)