# A simple makefile of the sdb_xy program written in C
# https://github.com/PBrus/Search-XYcoordinates

PROG = sdb_xy
OBJ = main.o sdbxy.o comm.o
LIB = lib/sdbxy.h lib/comm.h
CC = gcc
CFLAGS = -lm -O2 -Wall

$(PROG) : $(OBJ)
		$(CC) -o $@ $(OBJ) $(CFLAGS)

main.o : src/main.c $(LIB)
		$(CC) -c $< -o $@ $(CFLAGS)

sdbxy.o : src/sdbxy.c $(LIB)
		$(CC) -c $< -o $@ $(CFLAGS)
		
comm.o : src/comm.c lib/comm.h
		$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean :
		rm $(OBJ)
