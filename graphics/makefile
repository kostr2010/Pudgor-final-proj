CXX = g++
PROG = main
CXXFLAGS += -Wall
CXXFLAGS += -lsfml-graphics -lsfml-window -lsfml-system

PROG_OBJ = main.o sprite_sheet.o graphic_body.o view.o 
PROG_LIB = sprite_sheet.hpp graphic_body.hpp view.hpp
PROG_CPP = sprite_sheet.cpp graphic_body.cpp view.cpp

all: $(PROG) clean

$(PROG): $(PROG_OBJ) $(PROG_LIB)
	$(CXX) -o $@ $(PROG_OBJ) $(CXXFLAGS)

add: $(PROG_LIB) $(PROG_OBJ)
	git add $(PROG_LIB) $(PROG_CPP)

clean:
	rm -rf *.o