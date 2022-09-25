CC = gcc
LD = gcc

CFLAGS = -g -std=c11 -pedantic -Wall -Wextra -D_BSD_SOURCE -D_DEFAULT_SOURCE 
all: flow 

##########################################################################

flow: args.o flow.o 
	gcc $(CFLAGS) args.o flow.o -o flow 
#gcc $(CFLAGS) args.o flow.o -o ipk-sniffer -lpcap

flow.o: flow.c flow.h 
	gcc $(CFLAGS) -c flow.c -o flow.o 
#gcc $(CFLAGS) -c flow.c -o flow.o -lpcal

args.o: args.c args.h 
	gcc $(CFLAGS) -c args.c -o args.o 
#gcc $(CFLAGS) -c args.c -o args.o -lpcap

#interfaces.o: interfaces.c interfaces.h 
#	gcc $(CFLAGS) -c interfaces.c -o interfaces.o -lpcap

#frame.o: frame.c frame.h 
#	gcc $(CFLAGS) -c frame.c -o frame.o -lpcap

clean:
	rm *.o flow 

run: ipk-sniffer
	./flow