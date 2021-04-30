CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
LOAD=load_balancer
SERVER=server
DS=data_structures
LL=LinkedList
DLL=CircularDoublyLinkedList
HTB=Hashtable

.PHONY: build clean

#build: tema2

build: main.o $(LOAD).o $(SERVER).o $(LL).o $(DLL).o $(HTB).o
	$(CC) $^ -o tema2

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(LL).o: $(DS)/$(LL).c $(DS)/$(LL).h
	$(CC) $(CFLAGS) $^ -c

$(DLL).o: $(DS)/$(DLL).c $(DS)/$(DLL).h
	$(CC) $(CFLAGS) $^ -c

$(HTB).o: $(DS)/$(HTB).c $(DS)/$(HTB).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o tema2 *.h.gch
