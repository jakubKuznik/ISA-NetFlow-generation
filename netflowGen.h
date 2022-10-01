// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        netflow.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// header file for netflow.c 

#pragma once

#include "flow.h"



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
  time_t sysUpTime;
  time_t unixSecs;
  time_t unixNSecs;
  uint32_t flowSequence;
  char engineType;
  char engineId;
  uint16_t samplingInterval;
};
typedef struct NFHeader NFHeader;


/******************  RECORD FORMAT ************************
  0-3       4-7      8-11    12-13   14-15   16-19    20-23    
 srcaddr  dst-addr  nexthop  input   output  dPkts  dOctents

  24-27    28-31     32-33    34-35     36       37      38
  firts    Last     srcport  dstport   pad1   tcpflags  port

   39    40-41    42-43      44        45       46-47
  tos    src_as   dst_as  src_mask  dst_mask    pad2 
************************************************************/

/**
 * @brief XXX - packets with the same xxx are in one flow 
 * 
 */
struct NFPayload{
  // todo review if anything can be const 
  uint32_t srcAddr; // XXX - 
  uint32_t dstAddr; // XXX -
  uint32_t nextHop; // 0
  uint16_t input;   // 0
  uint16_t output;  // 0
  uint32_t dPkts;   
  uint32_t dOctents;
  uint32_t firts;
  uint32_t last;
  uint16_t srcPort; // XXX
  uint16_t dstPort;
  char pad1;      // 0
  char tcpFlags;  
  char port;      
  char tos;       // XXX - 
  uint16_t srcAs; // todo can be asked
  uint16_t dstAs; // todo can be asked 
  char srcMask;   // XXX - 
  char dstMask;   // XXX - 
  uint16_t pad2;
};
typedef struct NFPayload NFPayload;


struct netFlow{
  NFHeader nfheader;
  NFPayload nfpayload;
};
typedef struct netFlow netFlow;

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



void NFGeneratePayload();
void NFGenerateHeader();

