CC=g++
PROG=mandelbrot
CLIBS=-lSDL -fopenmp
OBJS=main.o Complex.o

all : $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(CLIBS)

main.o : main.cpp
	$(CC) -c main.cpp $(CLIBS)

Complex.o : Complex.cpp Complex.hpp
	$(CC) -c Complex.cpp $(CLIBS)
