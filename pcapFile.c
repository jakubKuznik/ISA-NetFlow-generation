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
    packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0,  true};

    frame = pcap_next(pcap, &pacHeader);
    if (frame == NULL){
      pacInfo.ok = false;
      return pacInfo;
    }
    ethHeader = (struct ether_header *)frame;
    

    // get time and interface from pcap 
    time_t frameTime = pacHeader.ts.tv_sec;

    debugPktStruct(pacInfo);

    // ip + icmp
    if (ntohs(ethHeader->ether_type) == ETHERTYPE_IP){
      switch (protocolType(frame)){
        case ICMP:
          fprintf(stderr,"ICMP\n"); 
          pacInfo = icmpPacketInfo(frame);
          break;
        case TCP:
          fprintf(stderr, "TCP\n"); 
          pacInfo = udpPacketInfo(frame);
          break;
        case UDP:
          fprintf(stderr, "UDP\n"); 
          pacInfo = tcpPacketInfo(frame);
          break;
        default:
          break;
      }
    }
    pacInfo.pacTime = frameTime;
    pacInfo.ok = true; 
    debugPktStruct(pacInfo);



    return pacInfo;
}


/**
 * @brief Find information about ICMP packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo icmpPacketInfo(const u_char *frame){
  
  packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, true};
  struct ip *ip_header = (struct ip*)(frame + ETH_HEAD_SIZE);

  pacInfo.protocol = ICMP;
  pacInfo.tos = ip_header->ip_tos;
  pacInfo.srcAddr = inet_ntoa(ip_header->ip_src);
  pacInfo.dstAddr = inet_ntoa(ip_header->ip_dst);
  pacInfo.srcPort = 0;
  pacInfo.dstPort = 0;
  pacInfo.layer3Size = ip_header->ip_hl;
  pacInfo.packetSize = ip_header->ip_len;

  return pacInfo; 
}

/**
 * @brief Find information about tcp packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo tcpPacketInfo(const u_char *frame){
  packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, true};

  struct ip *ipHeader = (struct ip*)(frame + ETH_HEAD_SIZE);
  struct tcphdr *tcpHeader = (struct tcphdr*)(frame + ETH_HEAD_SIZE + ipHeader->ip_hl);

  pacInfo.protocol = UDP;
  pacInfo.tos = ipHeader->ip_tos;
  pacInfo.srcAddr = inet_ntoa(ipHeader->ip_src);
  pacInfo.dstAddr = inet_ntoa(ipHeader->ip_dst);
  pacInfo.layer3Size = ipHeader->ip_hl;
  pacInfo.packetSize = ipHeader->ip_len;
  pacInfo.srcPort = tcpHeader->source;
  pacInfo.dstPort = tcpHeader->dest;

  return pacInfo; 
}

/**
 * @brief Find information about udp packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo udpPacketInfo(const u_char *frame){
  packetInfo pacInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, true};

  struct ip *ipHeader = (struct ip*)(frame + ETH_HEAD_SIZE);
  struct udphdr *udpHeader = (struct udphdr*)(frame + ETH_HEAD_SIZE + ipHeader->ip_hl);

  pacInfo.protocol = UDP;
  pacInfo.tos = ipHeader->ip_tos;
  pacInfo.srcAddr = inet_ntoa(ipHeader->ip_src);
  pacInfo.dstAddr = inet_ntoa(ipHeader->ip_dst);
  pacInfo.layer3Size = ipHeader->ip_hl;
  pacInfo.packetSize = ipHeader->ip_len;
  pacInfo.srcPort = udpHeader->uh_sport;
  pacInfo.dstPort = udpHeader->uh_dport;

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


// TODO smazat 
void debugPktStruct(packetInfo pktInfo){
  fprintf(stderr, "'''' srcAddr '''''' %d\n", pktInfo.srcAddr);
  fprintf(stderr, "'''' dstAddr '''''' %d\n", pktInfo.dstAddr);
  fprintf(stderr, "'''' srcPort '''''' %d\n", pktInfo.srcPort);
  fprintf(stderr, "'''' dstPort '''''' %d\n", pktInfo.dstPort);
  fprintf(stderr, "'''' protocol '''''' %d\n", pktInfo.protocol);
  fprintf(stderr, "'''' tos '''''' %d\n", pktInfo.tos);
  fprintf(stderr, "'''' layer3size '''''' %d\n", pktInfo.layer3Size);
  fprintf(stderr, "'''' pacSize  '''''' %d\n", pktInfo.packetSize);
  fprintf(stderr, "'''' pacTime '''''' %ld\n", pktInfo.pacTime);
  fprintf(stderr, "''''''''''''''''''' \n");
}