#Compiler
CC=gcc
CFLAGS=-Wall -O3 -Wno-unused-result
LIBS=

#files
SRCFILES=avltree.c
INCLUDEDIR=.
MAIN1=main.c
BIN1=prog

#tools
RM=rm -v


all:
	make $(BIN1)

$(BIN1): $(MAIN1) $(SRCFILES)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $(MAIN1) $(SRCFILES) -o $(BIN1)

clean:
	find -type f -name "*~" -exec $(RM) '{}' \;
