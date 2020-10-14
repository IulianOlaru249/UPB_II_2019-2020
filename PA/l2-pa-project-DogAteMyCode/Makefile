PROJECT=engine
SOURCES=engine.cpp xBoard.cpp board_0x88.cpp pieces.cpp move_gen.cpp
CFLAGS=-Wall
CC=g++

build:
	$(CC) $(CFLAGS) $(SOURCES) -o $(PROJECT) 
run:
	xboard -fcp "./$(PROJECT)"
debug:
	xboard -debug -fcp "./$(PROJECT)"
log:
	./$(PROJECT)
clean:
	rm -f engine
	rm -f xboard.debug
	rm -r log.txt
