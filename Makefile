CC = gcc
LD = gcc

CFLAGS = -g -std=c11 -pedantic -Wall -Wextra -D_BSD_SOURCE -D_DEFAULT_SOURCE 
all: flow 

##########################################################################

flow: args.o flow.o pcapFile.o netflowGen.o
	gcc $(CFLAGS) args.o flow.o pcapFile.o netflowGen.o -o flow -lpcap
#gcc $(CFLAGS) args.o flow.o -o ipk-sniffer -lpcap

flow.o: flow.c flow.h 
	gcc $(CFLAGS) -c flow.c -o flow.o -lpcap
#gcc $(CFLAGS) -c flow.c -o flow.o -lpcal

args.o: args.c args.h 
	gcc $(CFLAGS) -c args.c -o args.o -lpcap

pcapFile.o: pcapFile.c pcapFile.h
	gcc $(CFLAGS) -c pcapFile.c -o pcapFile.o -lpcap

netflowGen.o: netflowGen.c netflowGen.h
	gcc $(CFLAGS) -c netflowGen.c -o netflowGen.o -lpcap

clean:
	rm *.o flow 

run: ipk-sniffer
	./flow