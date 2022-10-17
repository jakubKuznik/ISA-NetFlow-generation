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
    struct pcap_pkthdr  pacHeader;          // packet header
    struct ether_header *ethHeader;        // ethernet   
    packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true};

    frame = pcap_next(pcap, &pacHeader);
    if (frame == NULL){
      pacInfo.ok = false;
      return pacInfo;
    }
    ethHeader = (struct ether_header *)frame;
    

    // get time and interface from pcap 
    time_t frameTime = pacHeader.ts.tv_sec;

    // ip + icmp
    if (ntohs(ethHeader->ether_type) == ETHERTYPE_IP){
      switch (protocolType(frame)){
        case ICMP:
          pacInfo = icmpPacketInfo(frame);
          break;
        case TCP:
          pacInfo = tcpPacketInfo(frame);
          break;
        case UDP:
          pacInfo = udpPacketInfo(frame);
          break;
        default:
          break;
      }
    }
    pacInfo.timeSec  =  frameTime;
    pacInfo.timeNano = pacHeader.ts.tv_usec * 1000;
    pacInfo.ok = true; 


    return pacInfo;
}


/**
 * @brief Find information about ICMP packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo icmpPacketInfo(const u_char *frame){
  packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true};
  struct ip *ip_header = (struct ip*)(frame + ETH_HEAD_SIZE);

  pacInfo.protocol    = ICMP;
  pacInfo.tos         = ntohs(ip_header->ip_tos);
  pacInfo.srcAddr     = ntohl(ip_header->ip_src.s_addr);
  pacInfo.dstAddr     = ntohl(ip_header->ip_dst.s_addr);
  pacInfo.srcPort     = 0;
  pacInfo.dstPort     = 0;
  pacInfo.layer3Size  = ntohl(ip_header->ip_hl);
  pacInfo.packetSize  = ntohs(ip_header->ip_len);
  pacInfo.cumulTcpOr  = 0;

  return pacInfo; 
}

/**
 * @brief Find information about tcp packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo tcpPacketInfo(const u_char *frame){
  packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true};

  struct ip *ipHeader = (struct ip*)(frame + ETH_HEAD_SIZE);
  struct tcphdr *tcpHeader = (struct tcphdr*)(frame + ETH_HEAD_SIZE + (ipHeader->ip_hl*4));

  pacInfo.protocol    = TCP;
  pacInfo.tos         = ntohs(ipHeader->ip_tos);
  pacInfo.srcAddr     = ntohl(ipHeader->ip_src.s_addr);
  pacInfo.dstAddr     = ntohl(ipHeader->ip_dst.s_addr);
  pacInfo.layer3Size  = ntohl(ipHeader->ip_hl);
  pacInfo.packetSize  = ntohs(ipHeader->ip_len);

  pacInfo.srcPort     = ntohs(tcpHeader->source);
  pacInfo.dstPort     = ntohs(tcpHeader->dest);

  pacInfo.cumulTcpOr = tcpHeader->th_flags;


  return pacInfo; 
}

/**
 * @brief Find information about udp packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo udpPacketInfo(const u_char *frame){
  packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true};

  struct ip *ipHeader = (struct ip*)(frame + ETH_HEAD_SIZE);
  struct udphdr *udpHeader = (struct udphdr*)(frame + ETH_HEAD_SIZE + (ipHeader->ip_hl*4));

  
  pacInfo.protocol    = UDP;
  pacInfo.tos         = ntohs(ipHeader->ip_tos);
  pacInfo.srcAddr     = ntohl(ipHeader->ip_src.s_addr);
  pacInfo.dstAddr     = ntohl(ipHeader->ip_dst.s_addr);
  pacInfo.layer3Size  = ntohl(ipHeader->ip_hl);
  pacInfo.packetSize  = ntohs(ipHeader->ip_len);
  
  pacInfo.srcPort     = ntohs(udpHeader->source);
  pacInfo.dstPort     = ntohs(udpHeader->dest);
  pacInfo.cumulTcpOr  = 0;

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
