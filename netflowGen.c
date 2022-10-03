// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        netflow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Generate netflow from packet.

#include "netflowGen.h"



/******************   HEADER  ********************************
 0-1     2-3      4-7       8-11       12-15         16-19          
version count  SysUptime  unix_secs  unix_nsecs  flow_sequence  

       20            21            22-23
   engine_type   engine_id   sampling_interval 
***************************************************************/
void NFGenerateHeader(){
  printf(" ");
}


/******************  RECORD FORMAT ************************
  0-3       4-7      8-11    12-13   14-15   16-19    20-23    
 srcaddr  dst-addr  nexthop  input   output  dPkts  dOctents

  24-27    28-31     32-33    34-35     36       37      38
  firts    Last     srcport  dstport   pad1   tcpflags  port

   39    40-41    42-43      44        45       46-47
  tos    src_as   dst_as  src_mask  dst_mask    pad2 
************************************************************/
void NFGeneratePayload(){
    printf(" ");
}

/**
 * @brief Free initFlowList struct 
 */
void freeInitFlowList(flowList *flowL){


    if (flowL->first != NULL){
        free(flowL->first);
    }
    /*
    if (flowL->first != NULL){
        free(flowL->first);
    }
    if (flowL->first != NULL){
        free(flowL->first);
    }
    */
    free(flowL);
}

/**
 * @brief Inicialize flow list sets first, last, current to null 
 * 
 * @return flowList* 
 * @return NULL if malloc error;
 */
flowList * initFlowList(){
    flowList *flowL = malloc(sizeof(flowList));
    if (flowL == NULL)
        return NULL;

    /*
    flowL->current = malloc(sizeof(node *));
    if (flowL->current == NULL){
        free(flowL);
        return NULL;
    }
    
    flowL->first = malloc(sizeof(node *));
    if (flowL->current == NULL){
        free(flowL);
        free(flowL->current);
        return NULL;
    }
    
    flowL->last = malloc(sizeof(node *));
    if (flowL->current == NULL){
        free(flowL);
        free(flowL->current);
        free(flowL->first);
        return NULL;
    }
    */

    flowL->current = NULL;
    flowL->first   = NULL;
    flowL->last    = NULL;
    flowL->size    = 0;

    return flowL;
}

/**
 * @brief Create a flow header
 * 
 * @return NFHeader* 
 * @return NULL if malloc error 
 */
NFHeader *createHeader(){
    NFHeader *header = malloc(sizeof(NFHeader));
    if (header == NULL)
        return NULL;
    
    header->version     = NF_VERSION;
    header->count       = FLOWS_IN_PACKETS;
    header->sysUpTime   = getBootTime();
    header->unixSecs    = getUTCinSec();
    header->unixNSecs   = getUTCinNsec();
    return header; 
}

/**
 * @brief Create a netflow Payload 
 * 
 * @
 * 
 * @return NFPayload* 
 * @return NULL if not succ 
 */
NFPayload *createPayload(struct packetInfo packet){
    NFPayload *payload = malloc(sizeof(NFPayload));
    if (payload == NULL)
        return NULL;

    payload->srcAddr  = packet.srcAddr;
    payload->dstAddr  = packet.dstAddr;
    payload->nextHop  = UNKNOWN;
    payload->input    = UNKNOWN;
    payload->output   = UNKNOWN;
    payload->dPkts    = 1; 
    payload->dOctents = packet.layer3Size; // TODO maybe wrong 
    payload->firts    = packet.pacTime;    
    payload->last     = packet.pacTime;    // has to update every packet 
    payload->srcPort  = packet.srcPort;
    payload->dstPort  = packet.dstPort;
    payload->pad1     = 0;
    payload->tcpFlags = packet.cumulTcpOr;
    payload->prot     = packet.protocol;
    payload->tos      = packet.tos;
    payload->srcAs    = UNKNOWN;
    payload->dstAs    = UNKNOWN;
    payload->srcMask  = UNKNOWN;
    payload->dstMask  = UNKNOWN;
    payload->pad2     = 0;
    
    return payload;
}

/**
 * @brief Update flow payload 
 * 
 * @param payload 
 * @param packet 
 */
void updatePayload(NFPayload *payload, struct packetInfo packet){
    payload->dPkts++;
    payload->dOctents += packet.layer3Size;
    payload->last = packet.pacTime;
}


/**
 * @brief Create a Flow object
 *  can set first and last flow and increment size; 
 * 
 * @param flowL list of all flows  
 * @param pacInfo packet info 
 * @return true 
 * @return false if error  
 */
bool createFlow(flowList *flowL, struct packetInfo *pacInfo){
    node *newNode;

    // if empty 
    if (flowL->first == NULL){
        newNode = malloc(sizeof(node));
        if (newNode == NULL)
            return false;
        newNode->next = NULL;
        newNode->prev = NULL;

        
        

    }

    // if there is just one 

    // if full 

    return true;

}



void freeFlowList(flowList *flowL){
    //node *firs 
    return;
}


void genNetflow(){
    printf(".");
}

/**
 * @brief Get the Last Flow object
 * 
 * @return int 
 */
int getLastFlow(){
    return 2;
}

/**
 * @brief Get the Last Flow object
 * 
 * @return int 
 */
int getFirstFlow(){
    return 2;
}

/**
 * @brief 
 * 
 */
void removeOldestFlow(){
    printf("..");
}
