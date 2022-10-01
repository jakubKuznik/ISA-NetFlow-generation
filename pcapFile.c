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
 * @brief This function will find all the packets information that are need for
 *  netflow and return  
 * 
 * @param pcap  
 * @return packetInfo struct 
 * @return NULL if error 
 */
packetInfo proccessPacket(pcap_t *pcap){

    const u_char        *frame;             // packet
    struct pcap_pkthdr  pacHeader;        // packet header
    struct ether_header *ethHeader;        // ethernet   
    packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, true};

    frame = pcap_next(pcap, &pacHeader);
    if (frame == NULL){
      pacInfo.ok = false;
      return pacInfo;
    }
    ethHeader = (struct ether_header *)frame;


    // ip + icmp
    if (ntohs(ethHeader->ether_type) == ETHERTYPE_IP){
      switch (protocolType(frame)){
        case ICMP:
          printf("ICMP\n"); 
          break;
        case TCP:
          printf("TCP\n"); 
          break;
        case UDP:
          printf("UDP\n"); 
          break;
        default:
          break;
      }
    }



    return pacInfo;
}

/**
 * @brief find out packet protocol 
 * 
 * @param frame 
 * @return UNKNOWN_PROTOCOL if not icmp tcp or udp  
 * @return ICMP 
 * @return TCP
 * @return UDP 
 */
int protocolType(const u_char *frame){
  
  struct ip *ip_header = (struct ip*)(frame + ETH_HEAD_SIZE);

  switch (ip_header->ip_p){
    case ICMP:
      return ICMP;
    case TCP:
      return TCP;
    case UDP:
      return UDP;
    default:
      return UNKNOWN_PROTOCOL;
    }
}