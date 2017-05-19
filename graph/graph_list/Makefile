# Compilation
CC=gcc
CFLAGS=-Wall -O1 -std=c99 -Wno-unused-result
LDFLAGS=

# Files
SRC=src/elf_graph.c src/elf_queue.c src/elf_list.c src/elf_vector.c src/elf_pqueue.c main.c
INCLUDE=./include/
BIN=prog

# Tools
RM=rm -vf

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -I $(INCLUDE) -o $(BIN) $(LDFLAGS)

clean:
	find -name "*~" -type f -exec $(RM) '{}' \;
	rm -f $(BIN)

run:
	./$(BIN)

zip: clean
	zip send.zip -r src/ include/ Makefile
