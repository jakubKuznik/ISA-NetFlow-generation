// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        netflow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Generate netflow from packet.

#include "netflowGen.h"


void proccessPacket(pcap_t *pcap){

    const u_char        *frame;             // packet
    struct pcap_pkthdr  pac_header;        // packet header
    struct ether_header *eth_header;        // ethernet   

    frame = pcap_next(pcap, &pac_header);



}


void genNetflow(){
    printf(".");
}

