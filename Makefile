CC=g++
PROG=mandelbrot
CLIBS=-lSDL -fopenmp
OBJS=main.o Complex.o

all : $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(CLIBS)

%.o : $*.cpp $*.hpp
	$(CC) -c $^ -O3 $(CLIBS)
