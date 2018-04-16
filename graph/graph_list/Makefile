# MIT License
# 
# Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# Compilation
CC=gcc
CFLAGS=-Wall -O1 -std=c99 -Wno-unused-result
LDFLAGS=

# Files
SRC=src/elf_graph.c src/elf_queue.c src/elf_list.c src/elf_int_vector.c src/elf_pqueue.c src/elf_int_uf_set.c  main.c
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
