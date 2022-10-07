// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        netflow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Generate netflow from packet.

#include "netflowGen.h"

/**
 * @brief Delete node from flowList 
 * 
 * @param flowL 
 * @param node 
 */
void deleteNode(flowList *flowL, node *node){
    if (flowL->size == 0)
        return;
    
    if (node == NULL)
        return;


    if (flowL->first == node){
        flowL->first = node->next;
    }
    else if (flowL->last == node){
        flowL->last = node->prev;
    }
    else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    free(node->data->nfheader);
    free(node->data->nfpayload);
    free(node->data);
    flowL->size--;
}


/**
 * @brief Free initFlowList struct 
 */
void freeInitFlowList(flowList *flowL){


    if (flowL->first != NULL){
        free(flowL->first);
    }
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


    flowL->current = NULL;
    flowL->first   = NULL;
    flowL->last    = NULL;
    flowL->size    = 0;

    return flowL;
}

/******************   HEADER  ********************************
 0-1     2-3      4-7       8-11       12-15         16-19          
version count  SysUptime  unix_secs  unix_nsecs  flow_sequence  

       20            21            22-23
   engine_type   engine_id   sampling_interval 
***************************************************************/
/**
 * @brief Create a flow header
 * 
 * @return NFHeader* 
 * @return NULL if malloc error 
 */
NFHeader *createHeader(struct packetInfo packet){
    NFHeader *header = malloc(sizeof(NFHeader));
    if (header == NULL)
        return NULL;
    
    header->version          = NF_VERSION;
    header->count            = FLOWS_IN_PACKETS;

    header->sysUpTime        = packet.timeSec; 
    header->unixSecs         = packet.timeSec;
    header->unixNSecs        = packet.timeNano;
    header->flowSequence     = 0;
    header->engineType       = UNKNOWN;
    header->engineId         = UNKNOWN;
    header->samplingInterval = UNKNOWN;
    return header;
}

/**
 * @brief Update header for existing flow 
 * 
 * @param header 
 */
void updateHeaderExists(NFHeader *header, struct packetInfo packet){
    header->sysUpTime        = packet.timeSec; 
    header->unixSecs         = packet.timeSec; 
    header->unixNSecs        = packet.timeNano; 
}


/**
 * @brief Use this function before flow send!
 * 
 * @param header 
 */
void updateHeader(NFHeader *header, uint32_t totalFlows, struct packetInfo packet){
    header->sysUpTime        = packet.timeSec; 
 /*
 
    header->unixSecs         = packet.timeSec; 
    header->unixNSecs        = packet.timeNano; 
*/
 
    header->flowSequence     = totalFlows;
}


/******************  RECORD FORMAT ************************
  0-3       4-7      8-11    12-13   14-15   16-19    20-23    
 srcaddr  dst-addr  nexthop  input   output  dPkts  dOctents

  24-27    28-31     32-33    34-35     36       37      38
  firts    Last     srcport  dstport   pad1   tcpflags  port

   39    40-41    42-43      44        45       46-47
  tos    src_as   dst_as  src_mask  dst_mask    pad2 
************************************************************/
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
    payload->dOctents = packet.packetSize; // packet size without eth header  
    payload->first    = packet.timeSec;    
    payload->last     = packet.timeSec;    // has to update every packet 
    payload->srcPort  = packet.srcPort;
    payload->dstPort  = packet.dstPort;
    payload->pad1     = 0;
    payload->tcpFlags = packet.cumulTcpOr ;
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
    payload->dOctents = payload->dOctents + packet.packetSize;
    payload->last = packet.timeSec;
    payload->tcpFlags = payload->tcpFlags | packet.cumulTcpOr ;
}

/**
 * @brief Check if some of flows are to old, if yes export and delete them  
 * 
 * @param flowL list of all flows 
 * @param packetTimeSec time for checking 
 * @param timer -a 
 * @param set all the settings 
 * @param collector colector ip add 
 * @param totalFlows each time flow is exported increment this  
 * @return true if ok
 * @return false if error 
 */
bool applyActiveTimer(flowList *flowL, uint32_t packetTimeSec, uint32_t timer, \
                    struct sockaddr_in *collector, uint32_t *totalFlows, packetInfo packet){
    node* node = flowL->first;
    while (node != NULL){
        
        if ((packetTimeSec - node->data->nfpayload->first) > timer){
            *totalFlows = *totalFlows + 1;
            updateHeader(node->data->nfheader, *totalFlows, packet);
            if(sendUdpFlow(node->data, collector) == false){
                deleteAllNodes(flowL);
                return false;

            }
            deleteNode(flowL, node);
        }
        node = node->next;
    }
    return true;
}


/**
 * @brief Check if some of flows are to old, if yes export and delete them  
 * 
 * @param flowL list of all flows 
 * @param packetTimeSec time for checking 
 * @param timer -a 
 * @param set all the settings 
 * @param collector colector ip add 
 * @param totalFlows each time flow is exported increment this  
 * @return true if ok
 * @return false if error 
 */
bool applyInactiveTimer(flowList *flowL, uint32_t packetTimeSec ,uint32_t timer, \
                    struct sockaddr_in *collector, uint32_t *totalFlows, packetInfo packet){
    node* node = flowL->first;
    while (node != NULL){
        
        if ((packetTimeSec - node->data->nfpayload->last) > timer){
            *totalFlows = *totalFlows + 1;
            updateHeader(node->data->nfheader, *totalFlows, packet);
            if(sendUdpFlow(node->data, collector) == false){
                deleteAllNodes(flowL);
                return false;
            }
            deleteNode(flowL, node);
        }
        node = node->next;
    }
    return true;
}

/**
 * @brief delete oldest flow in list
 * @param flowL list of all flows 
 * @param collector collector ip 
 * @param totalFlows number of flows already exported 
 * @return true if ok 
 * 
 */
bool deleteOldest(flowList *flowL, struct sockaddr_in *collector, \
     uint32_t *totalFlows, packetInfo packet ){
    if (flowL->first == NULL)
        return true;


    node *oldNode   = flowL->first;
    node *node      = flowL->first; 
    uint32_t oldest = flowL->first->data->nfpayload->first; 

    // find the oldest one 
    while (node != NULL){
        if (node->data->nfpayload->last < oldest){
            oldest = node->data->nfpayload->last;
            oldNode = node;
        }

        node = node->next;
    }
    (*totalFlows)++;
    updateHeader(oldNode->data->nfheader, *totalFlows, packet);
    if(sendUdpFlow(oldNode->data, collector) == false){
        deleteAllNodes(flowL);
        return false;
    }
    deleteNode(flowL, oldNode);
    
    return true; 
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

    // create node 
    node *newNode = createNode(pacInfo);
    if (newNode == NULL)
        return false;


    // if empty 
    if (flowL->first == NULL){
        flowL->first = newNode;
        flowL->last  = newNode;
        newNode->prev = NULL;
    }
    // if first == last 
    else if (flowL->first == flowL->last){
        flowL->last = newNode;
        flowL->first->next = newNode;
        newNode->prev = flowL->first;
    }
    else{
        node *temp;
        temp = flowL->last;
        flowL->last = newNode;
        temp->next = newNode;
        newNode->prev = temp;
    }

    newNode->next = NULL;
    flowL->size++;
    return true;
}

/**
 * @brief Create a new node (flow) from packet and return pointer to it 
 * 
 * @param pacInfo 
 * @return node * 
 * @return NULL if error 
 */
node *createNode(struct packetInfo *pacInfo){
    node *temp = malloc(sizeof(node));
    if (temp == NULL)
        return NULL;

    temp->data = malloc(sizeof(netFlow) + sizeof(NFPayload) + sizeof(NFHeader));
    if (temp->data == NULL){
        free (temp);
        return NULL;
    }

    temp->data->nfpayload = createPayload(*pacInfo);
    if (temp->data->nfpayload == NULL){
        free(temp->data);
        free(temp);
        return NULL;
    }

    temp->data->nfheader = createHeader(*pacInfo);
    if (temp->data->nfheader == NULL){
        free(temp->data->nfpayload);
        free(temp->data);
        free(temp);
        return NULL;
    }

    return temp;
}


/**
 * @brief find if packet is related to some existing flow 
 * 
 * @param flowL 
 * @param pacInfo 
 * @return node that is related to packet 
 * @return NULL if there is no related packet 
 */
node *findIfExists(flowList * flowL, struct packetInfo * pacInfo){
    node * temp = flowL->first;
    //printf("%d ", temp->data->nfpayload);
    if (temp == NULL)
        return NULL;
    while (temp != NULL){
        if (pacInfo->srcAddr  == temp->data->nfpayload->srcAddr  
        &&  pacInfo->dstAddr  == temp->data->nfpayload->dstAddr \
        &&  pacInfo->srcPort  == temp->data->nfpayload->srcPort \
        &&  pacInfo->dstPort  == temp->data->nfpayload->dstPort \
        &&  pacInfo->protocol == temp->data->nfpayload->prot )
            return temp;
        if (temp->next == NULL)
            break;
        else
            temp = temp->next;
    }

    return NULL;
}

/**
 * @brief htons every variable in flow  
 * 
 * @param nf 
 */
void htonsFlow(netFlow *nf){
    nf->nfheader->version           = htons(nf->nfheader->version);
    nf->nfheader->count             = htons(nf->nfheader->count);
    nf->nfheader->sysUpTime         = htonl(nf->nfheader->sysUpTime);
    nf->nfheader->unixSecs          = htonl(nf->nfheader->unixSecs);
    nf->nfheader->unixNSecs         = htonl(nf->nfheader->unixNSecs);
    nf->nfheader->flowSequence      = htonl(nf->nfheader->flowSequence);
    nf->nfheader->engineType        = htons(nf->nfheader->engineType);
    nf->nfheader->engineId          = htons(nf->nfheader->engineId);
    nf->nfheader->samplingInterval  = htons(nf->nfheader->samplingInterval);

    nf->nfpayload->srcAddr          = htonl(nf->nfpayload->srcAddr);
    nf->nfpayload->dstAddr          = htonl(nf->nfpayload->dstAddr);
    nf->nfpayload->nextHop          = htonl(nf->nfpayload->nextHop);
    nf->nfpayload->input            = htons(nf->nfpayload->input);
    nf->nfpayload->output           = htons(nf->nfpayload->output);
    nf->nfpayload->dPkts            = htonl(nf->nfpayload->dPkts);
    nf->nfpayload->dOctents         = htonl(nf->nfpayload->dOctents);
    nf->nfpayload->first            = htonl(nf->nfpayload->first);
    nf->nfpayload->last             = htonl(nf->nfpayload->last);
    nf->nfpayload->srcPort          = htons(nf->nfpayload->srcPort);
    nf->nfpayload->dstPort          = htons(nf->nfpayload->dstPort);
    nf->nfpayload->pad1             = htons(nf->nfpayload->pad1);
    nf->nfpayload->tcpFlags         = nf->nfpayload->tcpFlags;
    nf->nfpayload->prot             = htons(nf->nfpayload->prot);
    nf->nfpayload->tos              = htons(nf->nfpayload->tos);
    nf->nfpayload->srcAs            = htons(nf->nfpayload->srcAs);
    nf->nfpayload->dstAs            = htons(nf->nfpayload->dstAs);
    nf->nfpayload->srcMask          = htons(nf->nfpayload->srcMask);
    nf->nfpayload->dstMask          = htons(nf->nfpayload->dstMask);
    nf->nfpayload->pad2             = htons(nf->nfpayload->pad2);
}

/**
 * @brief Delete all nodes without exporting 
 */
void deleteAllNodes(flowList *fl){
    while (true){
        deleteNode(fl, fl->first);
        if (fl->size == 0)
            return;
    }
}

