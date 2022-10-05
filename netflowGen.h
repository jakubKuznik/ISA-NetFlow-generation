// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        netflow.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// header file for netflow.c 

#pragma once

#include "flow.h"
#include "pcapFile.h"

#define NF_VERSION 5
#define UNKNOWN 0
#define FLOWS_IN_PACKETS 1

#define NF_HEADER_SIZE 24
#define NF_PAYLOA_SIZE 48


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
  uint32_t flowSequence; // this has to be updated 
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
  uint32_t firts;    
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
  int size; 
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
 * @brief Inicialize flow list sets first, last, current to null 
 * 
 * @return flowList* 
 * @return NULL if malloc error;
 */
flowList * initFlowList();

/**
 * @brief Create a Flow object
 *  can set first and last flow and increment size; 
 * 
 * @param flowL list of all flows  
 * @param pacInfo packet info 
 * @return true 
 * @return false if error  
 */
bool createFlow(flowList *flowL, struct packetInfo *pacInfo);

/**
 * @brief Free initFlowList struct 
 */
void freeInitFlowList(flowList *flowL);

/**
 * @brief Create a flow header
 * 
 * @return NFHeader* 
 * @return NULL if malloc error 
 */
NFHeader *createHeader();

/**
 * @brief Update flow payload 
 * 
 * @param payload 
 * @param packet 
 */
void updatePayload(NFPayload *payload, struct packetInfo packet);

/**
 * @brief Create a netflow Payload 
 * 
 * @
 * 
 * @return NFPayload* 
 * @return NULL if not succ 
 */
NFPayload *createPayload(struct packetInfo packet);

/**
 * @brief Create a flow header
 * 
 * @return NFHeader* 
 * @return NULL if malloc error 
 */
NFHeader *createHeader();

/**
 * @brief Use this function before flow send!
 * 
 * @param header 
 */
void updateHeader(NFHeader *header, uint32_t totalFlows);

/**
 * @brief find if packet is related to some existing flow 
 * 
 * @param flowL 
 * @param pacInfo 
 * @return node that is related to packet 
 * @return NULL if there is no related packet 
 */
node *findIfExists(flowList * flowL, struct packetInfo * pacInfo);


/**
 * @brief 
 * 
 * @param flowL 
 * @param packetTime 
 * @param timer  
 * @return true if ok
 * @return false if error 
 */
bool appplyActiveTimer(flowList *flowL, time_t packetTime, uint32_t timer);

/**
 * @brief 
 * 
 * @param flowL 
 * @param packetTime
 * @param timer  
 * @return true if okay 
 * @return false if error 
 */
bool appplyInactiveTimer(flowList *flowL, time_t packetTime, uint32_t timer);

/**
 * @brief Create a new node (flow) from packet and return pointer to it 
 * 
 * @param pacInfo 
 * @return node * 
 * @return NULL if error 
 */
node *createNode(struct packetInfo *pacInfo);


/**
 * @brief htons every variable in flow  
 * 
 * @param nf 
 */
void htonsFlow(netFlow *nf);