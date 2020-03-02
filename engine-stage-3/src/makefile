CC = g++
CFLAGS = -I. -I../libs -L../libs/sole -Wall

all: main clear

main: main.o main-classes/main-classes.o factories/entity-factory.o factories/ability-factory.o constants/constants.o main-classes/main-classes.hpp factories/entity-factory.hpp factories/ability-factory.hpp constants/constants.hpp
	$(CC) -o main main.o main-classes/main-classes.o factories/entity-factory.o factories/ability-factory.o constants/constants.o

clear:
	find . -type f -name '*.o' -delete