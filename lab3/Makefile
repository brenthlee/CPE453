TARGET   = a.out
LDFLAGS  = -lm -pthread
SOURCES  = $(wildcard *.c)
INCLUDES = $(wildcard *.h)
OBJECTS  = $(SOURCES:.c=.o)

all: create sequence thread dinePhil

create: create.c
	gcc -o create create.c

sequence: sortSequence.c
	gcc -o sequence -pthread sortSequence.c

thread: sortThread.c
	gcc -o thread -pthread sortThread.c

dinePhil: dinePhil.c
	gcc -o dinePhil -pthread dinePhil.c

clean:
	rm -f dinePhil create sequence thread *.txt *.out
