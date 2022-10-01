// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        pcapFile.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// header file for pcapFile.c 

#pragma once

#include "flow.h"

#define ICMP 1 // wiki ip paket sekce data 
#define TCP 6
#define UDP 17 
#define UNKNOWN_PROTOCOL -1

#define ETH_HEAD_SIZE 14


/**
 * @brief Basic info about packet, that define netflow 
 */
struct packetInfo
{
  uint32_t srcAddr;   // XXX 
  uint32_t dstAddr;   // XXX
  uint16_t srdPort;   // XXX
  char tos;           // XXX
  char srcMask;       // XXX
  char dstMask;       // XXX
  uint32_t layer3Size; // from IHL 
  time_t pacTime;
  bool ok;            // if ok set to true 
};
typedef struct packetInfo packetInfo;

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
 * @return UNKNOWN_PROTOCOL if not icmp tcp or udp  
 * @return ICMP 
 * @return TCP
 * @return UDP 
 */
int protocolType(const u_char *frame);

