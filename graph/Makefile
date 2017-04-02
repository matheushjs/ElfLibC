# Compilation
CC=gcc
CFLAGS=-Wall -O1 -std=c99 -Wno-unused-result
LDFLAGS=

# Files
SRC=src/elf_graph_matrix.c src/elf_graph_list.c src/main.c
INCLUDE=./include/
BIN=prog
TO_ZIP=$(SRC) $(SRC:src%.c=include%.h) Makefile README.txt
ZIPFILE=send.zip

# Tools
RM=rm -vf

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -I $(INCLUDE) -o $(BIN) $(LDFLAGS)

clean:
	find -name "*~" -type f -exec $(RM) '{}' \;

run:
	./$(BIN)

zip:
	zip $(ZIPFILE) $(TO_ZIP)
