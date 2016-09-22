all:
	gcc -o tree tree.c main.c -I.

debug:
	valgrind --leak-check=full ./tree

clean:
	rm -vf *~
