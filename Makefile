TARGET = prog
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
CXX = gcc
CFLAGS = -g -std=c11 -O2 -Wall -Wextra
SRCDIR = src
BUILDDIR = build

.PHONY: default all clean directories

default: directories $(TARGET)
all: default

SRC := $(shell find $(SRCDIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))
HEADERS := $(shell find $(SRCDIR) -type f -name '*.h')
FOLDERS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(dir $(shell find $(SRCDIR) -type f -name '*.c')))

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(BUILDDIR)/$(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

directories:
	mkdir -p $(FOLDERS)
	

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -rf $(BUILDDIR)