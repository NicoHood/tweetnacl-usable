CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-Wall
SOURCES=main.c tweetnacl.c randombytes.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tweetnacl

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm  *.o

cleanall:
	rm  *.o $(EXECUTABLE)

cstart: all start

start:
	./$(EXECUTABLE)

rebuild: cleanall all
