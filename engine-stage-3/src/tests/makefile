CC = g++
CFLAGS = -I. -I../../libs -L../../libs/sole -Wall

all: main clear

main: engine.spec.o ../main-classes/main-classes.o ../factories/entity-factory.o ../factories/ability-factory.o ../constants/constants.o ../main-classes/main-classes.hpp ../factories/entity-factory.hpp ../factories/ability-factory.hpp ../constants/constants.hpp
	$(CC) -o main engine.spec.o ../main-classes/main-classes.o ../factories/entity-factory.o ../factories/ability-factory.o ../constants/constants.o

clear:
	cd ../../ && find . -type f -name '*.o' -delete