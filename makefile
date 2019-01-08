CC = g++
CFLAGS = -g -Wall
LIBS = `pkg-config --libs opencv`

all: program1 

program1: program1.cpp
	$(CC) $(CFLAGS) -o program1 program1.cpp $(LIBS)

.cpp.o:
	g++ -c -g pkg-config --cflags opencv $<

.PHONY: clean

clean:
	rm -f core *.o program1
