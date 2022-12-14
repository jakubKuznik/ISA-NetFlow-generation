// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        netflow.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// header file for netflow.c 

#ifndef NETFLOW_GEN_H
#define NETFLOW_GEN_H


#include "pcapFile.h"
#include "structurik.h"
#include "udp.h"

#define NF_VERSION 5
#define UNKNOWN 0
#define FLOWS_IN_PACKETS 1

#define NF_HEADER_SIZE 24
#define NF_PAYLOA_SIZE 48



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
bool createFlow(flowList *flowL, struct packetInfo *pacInfo, uint32_t sysUpTime);

/**
 * @brief Free initFlowList struct 
 */
void freeInitFlowList(flowList *flowL);

/**
 * @brief Update flow payload 
 * 
 * @param payload 
 * @param packet 
 */
void updatePayload(NFPayload *payload, struct packetInfo packet, uint32_t sysUpTime);

/**
 * @brief Create a netflow Payload 
 * 
 * @
 * 
 * @return NFPayload* 
 * @return NULL if not succ 
 */
NFPayload *createPayload(struct packetInfo packet, uint32_t sysUpTime);

/**
 * @brief Create a flow header
 * 
 * @return NFHeader* 
 * @return NULL if malloc error 
 */
NFHeader *createHeader(struct packetInfo packet);

/**
 * @brief Use this function before flow send!
 * 
 * @param header 
 */
void updateHeader(NFHeader *header, uint32_t totalFlows, uint32_t sysUpTime, packetInfo currentPacket);

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
 * @brief Check if some of flows are to old, if yes export and delete them  
 * 
 * @param flowL list of all flows 
 * @param packetTimeSec time for checking 
 * @param timer -a 
 * @param set all the settings 
 * @param collector colector ip add 
 * @param totalFlows each time flow is exported increment this  
 * @param clientSoc  
 * @return true if ok
 * @return false if error 
 */
bool applyActiveTimer(flowList *flowL, uint32_t timer, \
        uint32_t *totalFlows, int clientSoc, uint32_t sysUpTime, packetInfo currentPacket);

/**
 * @brief Check if some of flows are to old, if yes export and delete them  
 * 
 * @param flowL list of all flows 
 * @param packetTimeSec time for checking 
 * @param timer -a 
 * @param set all the settings 
 * @param collector colector ip add 
 * @param totalFlows each time flow is exported increment this  
 * @param clientSoc  
 * @return true if ok
 * @return false if error 
 */
bool applyInactiveTimer(flowList *flowL, uint32_t timer, \
    uint32_t *totalFlows, int clientSoc, uint32_t sysUpTime, packetInfo currentPacket);

/**
 * @brief delete oldest flow in list
 * @param flowL list of all flows 
 * @param collector collector ip 
 * @param totalFlows number of flows already exported 
 * @param clientSoc  
 * @return true if ok 
 */
bool deleteOldest(flowList *flowL, \
     uint32_t *totalFlows, int clientSoc, uint32_t sysUpTime, packetInfo currentPacket);

/**
 * @brief Create a new node (flow) from packet and return pointer to it 
 * 
 * @param pacInfo 
 * @return node * 
 * @return NULL if error 
 */
node *createNode(struct packetInfo *pacInfo, uint32_t sysUpTime);


/**
 * @brief htons every variable in flow  
 * 
 * @param nf 
 */
void htonsFlow(netFlow *nf);

/**
 * @brief Delete node from flowList 
 * 
 * @param flowL 
 * @param node 
 */
void deleteNode(flowList *flowL, node *node);

/**
 * @brief Delete all nodes without exporting 
 */
void deleteAllNodes(flowList *fl);

/**
 * @brief Delete and send the node  
 */
bool deleteAndSend(flowList *flowL, \
     uint32_t *totalFlows, node *delete, int clientSoc, uint32_t sysUpTime, packetInfo currentPacket);


#endif