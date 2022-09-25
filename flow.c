// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        flow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Read file arguments 

// Execution:
// ./ipk-sniffer [-i rozhraní | --interface rozhraní] {-p ­­port} 
//               {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}

#include "flow.h"


int main(int argc, char *argv[]) {
    printf("%s, %d", argv[0], argc);
}

