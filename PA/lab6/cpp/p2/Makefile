CC=g++
CFLAGS=-Wall -O3 -std=c++11

all: reversi

reversi: reversi.cpp
	$(CC) $^ -o $@ $(CFLAGS) -lm

run: reversi
	./reversi

clean:
	rm -rf reversi
