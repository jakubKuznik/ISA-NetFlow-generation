// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        struct.h 
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// contain structs  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define __FAVOR_BSD

#pragma once

/**
 * @brief Basic info about packet, that define netflow 
 * packets with the same XXX are in the same flow 
 */
struct packetInfo
{
  uint32_t srcAddr;        // XXX 
  uint32_t dstAddr;        // XXX
  uint16_t srcPort;        // XXX
  uint16_t dstPort;        // XXX
  uint8_t protocol;        // TCP UDP ICMP 
  uint8_t tos;             // XXX
  unsigned int layer3Size; // from IHL 
  uint16_t packetSize;     // entire packet size without et_header 
  uint32_t timeSec;
  uint32_t timeNano;
  uint8_t cumulTcpOr;      // cumulative tcp flags or 
  bool ok;            // if ok set to true 
};
typedef struct packetInfo packetInfo;


/******************   HEADER  ********************************
 0-1     2-3      4-7       8-11       12-15         16-19          
version count  SysUptime  unix_secs  unix_nsecs  flow_sequence  

       20            21            22-23
   engine_type   engine_id   sampling_interval 
***************************************************************/
struct NFHeader{ 
  // todo review if anything can be const 
  uint16_t version;
  uint16_t count;
  uint32_t sysUpTime;      // this has to be updated 
  uint32_t unixSecs;       // this has to be updated 
  uint32_t unixNSecs;      // this has to be updated 
  uint32_t flowSequence;   // this has to be updated 
  uint8_t engineType;
  uint8_t engineId;
  uint16_t samplingInterval;
};
typedef struct NFHeader NFHeader;


/******************  RECORD FORMAT ************************
  0-3       4-7      8-11    12-13   14-15   16-19    20-23    
 srcaddr  dst-addr  nexthop  input   output  dPkts  dOctents

  24-27    28-31     32-33    34-35     36       37      38
  firts    Last     srcport  dstport   pad1   tcpflags   prot 

   39    40-41    42-43      44        45       46-47
  tos    src_as   dst_as  src_mask  dst_mask    pad2 
************************************************************/

/**
 * @brief XXX - packets with the same xxx are in one flow 
 * 
 */
struct NFPayload{
  // todo review if anything can be const 
  uint32_t srcAddr;  // XXX - 
  uint32_t dstAddr;  // XXX -
  uint32_t nextHop;  // 0
  uint16_t input;    // 0
  uint16_t output;   // 0
  uint32_t dPkts;    // update after every packet  
  uint32_t dOctents; // update after every packet 
  uint32_t first;    
  uint32_t last;     // update 
  uint16_t srcPort;  // XXX
  uint16_t dstPort;  // XXX 
  uint8_t pad1;      // 0
  uint8_t tcpFlags;  
  uint8_t prot;      
  uint8_t tos;       // XXX - 
  uint16_t srcAs;    // todo can be asked
  uint16_t dstAs;    // todo can be asked 
  uint8_t srcMask;   // XXX - should be /32 
  uint8_t dstMask;   // XXX - should be /32
  uint16_t pad2;
};
typedef struct NFPayload NFPayload;


struct netFlow{
  NFHeader * nfheader;
  NFPayload * nfpayload;
};
typedef struct netFlow netFlow;


/**
 * @brief Double linked list with flows;
 */
struct flowList{
  uint32_t size; 
  struct node *first;   
  struct node *last;
  struct node *current;   
};
typedef struct flowList flowList;

/**
 * @brief linked list one node
 * 
 * |-NULL----node1---node2---nodeN---NULL-|
 */
struct node{
  netFlow * data; 
  struct node *next;
  struct node *prev;
};
typedef struct node node;


/**
 * @brief Information about flow array as we can delete the oldest flow 
 * with this data struct we dont have to reindex everything. 
 * 
 */
struct flowArrayInfo{
  int first;   // first one can be any number 
  int last;    // last one 
  int size;    // array size  
  int current; // curent 
};
typedef struct flowArrayInfo flowArrayInfo;

/**
 * @brief Settings struct for the program 
 * 
 * -f input file    [DEFAULT NULL (STDIN)] {inputFile}
 * -c collector ip  [DEFAULT 127.0.0.1:2055] {collectorIp, collectorPort}
 * -a acctive timer [DEFAULT 60s] {timerActive}
 * -i interval      [DEFAULT 10s] {interval}
 * -m flow cache    [DEFAULT 1024] {cacheSize}
 */
struct set{
    //TODO ASSIGN DEFAULT VALUES 
    FILE *inputFile;            // input file if NULL we will use STDIN 
    in_addr_t collectorIp;      // netflow collector ip address 
    in_port_t collectorPort;    // netflow collector ip address 
    uint32_t timerActive;       // timmer in sec 
    uint32_t cacheSize;         // 
    uint32_t interval;          // after end non-active flows will be 
                                // exported to collector 
};

//typedef struct set set;