CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o COO2CSR.o CGSolver.o
	$(CC) $(CFLAGS) -o main main.o COO2CSR.o CGSolver.o
 
# The main.o target can be written more simply
 
main.o: main.cpp COO2CSR.hpp CGSolver.hpp
	$(CC) $(CFLAGS) -c main.cpp
 
COO2CSR.o: COO2CSR.hpp
 
CGSolver.o: CGSolver.hpp COO2CSR.hpp

clean:
	rm -f main *.o