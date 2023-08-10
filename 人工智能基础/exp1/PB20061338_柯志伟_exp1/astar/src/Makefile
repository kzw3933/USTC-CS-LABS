CC = g++
CFLAGS = -g -Wall
LDFLAGS = -g

all: astar

astar: astar.o
	$(CC) $(LDFLAGS) astar.o -o astar

astar.o: astar.cpp
	$(CC) $(CFLAGS) -c astar.cpp -o astar.o

clean:
	rm -rf astar astar.o