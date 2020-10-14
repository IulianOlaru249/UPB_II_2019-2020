CC=g++
CFLAGS=-Wall -O3 -std=c++11

all: nim

nim: nim.cpp
	$(CC) $^ -o $@ $(CFLAGS)

run: nim
	./nim

clean:
	rm -rf nim
