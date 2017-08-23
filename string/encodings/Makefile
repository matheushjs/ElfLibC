all: elf_encodings.c main.c elf_encodings.h
	gcc -Wall -O2 -Wno-unused-result -I. elf_encodings.c main.c -o prog

clean:
	rm -vf *~
