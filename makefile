CC = gcc
CFLAGS = -I. -Wall

all: main clear

main:  main.o
	$(CC) -o main main.o 

clear:
	rm -rf main.o 

