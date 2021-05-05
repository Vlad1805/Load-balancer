CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
LOAD=load_balancer
SERVER=server
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

$(LL).o: $(LL).c $(LL).h
	$(CC) $(CFLAGS) $^ -c

$(DLL).o: $(DLL).c $(DLL).h
	$(CC) $(CFLAGS) $^ -c

$(HTB).o: $(HTB).c $(HTB).h
	$(CC) $(CFLAGS) $^ -c

Pack:
	zip -FSr 313CA_Stanciu_Vlad_tema2.zip README Makefile *.c *.h

clean:
	rm -f *.o tema2 *.h.gch
