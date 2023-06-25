#Compiler
CPP=g++

#Debug flags
DEBUG=-Wall -Warray-bounds -Wextra -g 

#Libraries (to add when linking)
LAPACKLIB=-llapack -lblas
SFMLLIB= -lsfml-graphics -lsfml-window -lsfml-system

#Libraries (to add when compiling)
EIGENDIR=-I /home/kilda/eigen-eigen-323c052e1731/

#Complete set of flags 
CPPFLAGS= -std=c++11 $(DEBUG) $(EIGENDIR) 
LDFLAGS=$(DEBUG)  
LOADLIBES=$(LAPACKLIB) $(SFMLLIB)

#List of programs
PROGRAMS=main 

#Make excutables for all programs
all: $(PROGRAMS)

#Object files for [main], and rule to make exec for [main]
main_OBJS = util.o sfml_util.o wall.o particle.o event.o collision_system.o initialize2D.o main.o    
main: $(main_OBJS) 
	$(CPP) $(LDFLAGS) $(main_OBJS) -o $@ $(LOADLIBES)

#Rule for creating .o files from .cpp files
%.o: %.cpp %.h
	$(CPP) $(CPPFLAGS) -c $*.cpp

clean:
	rm -f *.o $(PROGRAMS)

realclean: clean
	rm -f *~ 















