TARGET   = a.out
LDFLAGS  = -lm
SOURCES  = $(wildcard *.c)
INCLUDES = $(wildcard *.h)
OBJECTS  = $(SOURCES:.c=.o)

all:a.out

a.out:translator.c
	gcc translator.c

clean:
	rm -rf a.out output.txt
