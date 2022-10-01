// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        pcapFile.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Works with pcap input file or stdin.

#include "pcapFile.h"

/**
 * @brief If file is not NULL, then close it. 
 * @param f 
 */
void closeFile(FILE *f){
    if (f != NULL){
        fclose(f);
    }
}

/**
 * @brief Open pcap_file using pcap_fopen_offline and return it 
 * 
 * @param f pcap file that will be opened (if NULL open stdin) 
 * @param pcapBuff 
 * @return pcap_t or NULL if fail 
 */
pcap_t * openPcapFile(FILE *f, char *pcapBuff){
    pcap_t *pcap; 
    if (f == NULL){
        pcap = pcap_fopen_offline(stdin, pcapBuff);
    }
    else{
        pcap = pcap_fopen_offline(f, pcapBuff);
    }
    return pcap;
}

/**
 * @brief 
 * 
 * I NEED: 
 *  srcAddr
 *  dstAddr
 *  nxtHoop
 * 
 * @param pcap 
 * @return true 
 * @return false 
 */
bool proccessPacket(pcap_t *pcap){

    const u_char        *frame;             // packet
    struct pcap_pkthdr  pacHeader;        // packet header
    struct ether_header *ethHeader;        // ethernet   

    frame = pcap_next(pcap, &pacHeader);
    if (frame == NULL)
        return false;
    ethHeader = (struct ether_header *)frame;

    // find frame type 
    switch (ntohs(ethHeader->ether_type)){
        // ip + icmp
        case ETHERTYPE_IP:
            printf("ip paket\n");
            break;
        // ipv6
        case ETHERTYPE_IPV6:
            break;
        default: //skip unknown protocol 
            break;
    }
    return true;
}