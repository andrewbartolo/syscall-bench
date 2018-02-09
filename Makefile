ALL:
	gcc --std=c99 bench.c -o bench

clean:
	rm -f bench
