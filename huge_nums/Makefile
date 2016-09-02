CC = gcc
INCLUDE = ./
RM = rm -f
OBJ = prog.o huge_num_dl.o
CFLAGS = -Wall

all:
	make prog

run:
	./prog

prog: $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(OBJ) -o $@

huge_num_dl.o: huge_num_dl.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $?

prog.o: prog.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $?

clean:
	$(RM) ./*~ *.o prog ./.*~

zip:
	zip prog.zip prog.c huge_num_dl.c huge_num_dl.h Makefile
