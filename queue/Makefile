all:
	gcc -Wall -O1 -Wno-unused-result -I. elf_queue.c main.c -o prog

debug:
	valgrind --leak-check=full ./prog
