CC=clang
CFLAGS=-std=gnu99 -pedantic -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-parameter -Wno-overlength-strings -g
LDFLAGS=-lpthread -lSDL2 
SOURCES=src/main.c src/util.c src/interconnect.c src/cpu.c src/gfx.c src/timer.c src/renderer_sdl.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bin/strategema
all: $(SOURCES) $(EXECUTABLE)
	    
$(EXECUTABLE): $(OBJECTS) 
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@
clean:
	rm -f src/*.o