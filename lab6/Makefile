TARGET = snake hungry nums

all: $(TARGET)

snakes: snakemain.c
	gcc -o snakes lwp.c rr.c threadStuff.c stackStuff.c magic64.S snakemain.c libncurses.so libsnakes.a 2>/dev/null

hungry: hungrymain.c
	gcc -o hungry lwp.c rr.c threadStuff.c stackStuff.c magic64.S hungrymain.c libncurses.so libsnakes.a 2>/dev/null

nums: numbersmain.c
	gcc -o nums lwp.c rr.c threadStuff.c stackStuff.c magic64.S numbersmain.c 2>/dev/null

clean:
	rm -rf $(TARGET)
