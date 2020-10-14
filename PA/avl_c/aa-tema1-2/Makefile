build:
	gcc -g main.c avl.c -o struct

runLeaks:
	valgrind --leak-check=yes ./struct

run:
	./struct
clean:
	rm -rf struct
