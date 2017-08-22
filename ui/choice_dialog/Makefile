SRC= elf_canvas.c elf_utf_buf.c elf_string_buf.c elf_encodings.c elf_string.c elf_choice_dialog.c
IFILES= $(patsubst %.c, %.h, $(SRC))

all: main.c $(SRC) $(IFILES)
	gcc -Wall -O2 -Wno-unused-result -I. $(SRC) main.c -o prog

clean:
	rm -vf *~
