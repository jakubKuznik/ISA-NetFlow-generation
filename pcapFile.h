// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        pcapFile.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// header file for pcapFile.c 

#ifndef PCAP_H
#define PCAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>  
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcap/pcap.h>

#include "struct.h"

#define ICMP 1 // wiki ip paket sekce data 
#define TCP 6
#define UDP 17 
#define UNKNOWN_PROTOCOL -1

#define ETH_HEAD_SIZE 14


/**
 * @brief If file is not NULL, then close it. 
 * @param f 
 */
void closeFile(FILE *f);


/**
 * @brief Open pcap_file using pcap_fopen_offline and return it 
 * 
 * @param f pcap file that will be opened (if NULL open stdin) 
 * @param pcapBuff 
 * @return pcap_t or NULL if fail 
 */
pcap_t * openPcapFile(FILE *f, char *pcapBuff);

/**
 * @brief This function will find all the packets information that are need for
 *  netflow and return  
 * 
 * @param pcap  
 * @return packetInfo struct 
 * @return NULL if error 
 */
packetInfo proccessPacket(pcap_t *pcap);

/**
 * @brief find out packet protocol 
 * 
 * @param frame 
 * @return -1 if unknown protocol  
 * @return ICMP 
 * @return TCP
 * @return UDP 
 */
int protocolType(const u_char *frame);



/**
 * @brief Find information about ICMP packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo icmpPacketInfo(const u_char *frame);

/**
 * @brief Find information about tcp packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo tcpPacketInfo(const u_char *frame);

/**
 * @brief Find information about udp packet and write it to packetInfo struct 
 * 
 * @param frame 
 * @return packetInfo struct 
 */
packetInfo udpPacketInfo(const u_char *frame);


#endif