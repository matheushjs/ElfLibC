# Compilation
CC=gcc
CFLAGS=-Wall -O1 -std=c99 -Wno-unused-result
LDFLAGS=

# Files
SRC=elf_graph.c elf_queue.c elf_list.c main.c
INCLUDE=./
BIN=prog

# Tools
RM=rm -vf

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -I $(INCLUDE) -o $(BIN) $(LDFLAGS)

clean:
	find -name "*~" -type f -exec $(RM) '{}' \;
	rm $(BIN)

run:
	./$(BIN)

zip: clean
	zip send.zip -r src/ include/ Makefile
