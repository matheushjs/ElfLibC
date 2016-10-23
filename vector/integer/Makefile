CC=gcc
CFLAGS=-Wall -O3
INCLUDE=./include/
SRC=src/utils.c src/vector.c src/bubblesort.c src/insertionsort.c src/mergesort.c \
    src/shellsort.c src/heapsort.c src/countingsort.c src/quicksort.c
EXEC1=prog
LIBS=-lm
RM=rm -fv

MAIN1=src/main.c
MAIN2=src/benchmark.c
EXEC1=prog
EXEC2=test

all:
	$(CC) $(CFLAGS) -I$(INCLUDE) $(MAIN1) $(SRC) -o $(EXEC1) $(LIBS)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(MAIN2) $(SRC) -o $(EXEC2) $(LIBS)

clean:
	find -type f -name "*~" -exec $(RM) '{}' \;
