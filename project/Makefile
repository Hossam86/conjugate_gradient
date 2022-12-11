CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o matvecops.o CGSolver.o
	$(CC) $(CFLAGS) -o main main.o matvecops.o CGSolver.o
 
# The main.o target can be written more simply
 
main.o: main.cpp matvecops.hpp CGSolver.hpp
	$(CC) $(CFLAGS) -c main.cpp
 
matvecops.o: matvecops.hpp
 
CGSolver.o: CGSolver.hpp matvecops.hpp

clean:
	rm -f main *.o