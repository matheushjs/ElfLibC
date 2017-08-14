all: elf_string_buf.c elf_string_buf.h
	gcc -O2 -Wall -Wno-unused-result -I. elf_string_buf.c main.c -o prog

clean:
	find -name "*~" -exec rm -vf '{}' \;
