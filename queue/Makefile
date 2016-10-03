all:
	gcc -Wall -I. queue.c main.c -o queue

debug:
	valgrind --leak-check=full ./queue
