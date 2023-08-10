CC = g++
CFLAGS = -g -Wall -O3
LDFLAGS = -g

all: csp

csp: csp.o
	$(CC) $(LDFLAGS) csp.o -o csp

csp.o: csp.cpp
	$(CC) $(CFLAGS) -c csp.cpp -o csp.o

clean:
	rm -rf csp csp.o