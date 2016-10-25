CC=clang
CFLAGS=-std=gnu99 -pedantic -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-parameter -Wno-overlength-strings -g
LDFLAGS=-lpthread -framework CoreFoundation -framework Foundation -lobjc -framework CoreGraphics -framework cocoa
SOURCES=src/main.c src/util.c src/interconnect.c src/cpu.c src/gfx.c src/timer.c src/renderer_osx.c 3rdparty/minifb/MacMiniFB.m 3rdparty/minifb/OSXWindow.m 3rdparty/minifb/OSXWindowFrameView.m
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bin/strategema

all: $(SOURCES) $(EXECUTABLE)
	    
$(EXECUTABLE): $(OBJECTS) 
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@
clean:
	rm -f src/*.o 3rdparty/minifb/*.o
