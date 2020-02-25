CC = g++
CFLAGS = -I. -Wall

all: main clear

main:  engine.o engine-types.o engine-types.h engine-constants.h
	$(CC) -o main engine.o engine-types.o 

clear:
	rm -rf engine.o engine-types.o

