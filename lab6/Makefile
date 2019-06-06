CC = gcc

CFLAGS = -Wall -fPIC -g -I .

AR = ar r

RANLIB = ranlib

LIBS = liblwp.so

OBJS = lwp.o magic64.o rr.o stackStuff.o

SRCS = lwp.c rr.c threadStuff.c magic64.S

HDRS =

EXTRACLEAN = core liblwp.a liblwp.so

all: $(LIBS)

allclean: clean
	@rm -f $(EXTRACLEAN)

clean:
	rm -f $(OBJS) liblwp.so *.gch *~ TAGS

$(LIB): $(OBJS)
	$(AR) $@ $(OBJS)
	ranlib $@

depend:
	@echo Regenerating local dependencies
	@makedpened -Y $(SRCS) $(HDRS)

test: nums
	./nums

nums: numbersmain.o liblwp.a
	$(CC) $(CFLAGS) -g -o nums numbersmain.o -L. -llwp

numbersmain.o: numbersmain.c
	$(CC) $(CFLAGS) -c numbersmain.c

magic64.o: magic64.S
	$(CC) $(CFLAGS) -c -o magic64.o magic64.S

liblwp.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -m64 -c -o $@ $*.c

lwp.o: threadStuff.h stackStuff.h
threadStuff.o: threadStuff.h
