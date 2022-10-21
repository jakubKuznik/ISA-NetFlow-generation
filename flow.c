// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        flow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Main file that also has udp  

// Execution:
// ./flow [-f <file>] [-c <netflow_collector>[:<port>]] [-a <active_timer>] 
//        [-i <inactive_timer>] [-m <count>]

#include "flow.h"

// GLOBAL settings variable
struct set settings;


int main(int argc, char *argv[]) {

    // flow counter every time flow is send ++
    uint32_t i = 0;
    uint32_t *totalFlows = &i;

    // set program settings 
    settings = parseArgs(argc, argv);
    // buffer that is used in pcap struct  
    char pcapBuff[MY_PCAP_BUFF_SIZE];
    
    pcap_t *pcap;
    
    // socket on client side 
    int clientSock = 0;

    // basic info about packet that is need for netflow 
    packetInfo * pacInfo = malloc(sizeof(packetInfo));
    // list with all the flows 
    flowList * flowL; 

    if ((pcap = openPcapFile(settings.inputFile, pcapBuff)) == NULL){
        goto error1;
    }

    // init flow list  
    if((flowL = initFlowList()) == NULL)
        goto error2;

    // init collector 
    struct sockaddr_in * collector = initServer(settings);
    if (collector == NULL)
        goto error3;

    // start connection between sender and collector 
    // socket will be stored in global variable clientSock  
    if ((clientSock = startConnection(*collector)) == SOCKET_ERROR)
        goto error5;

    node *temp;

    while(true){
        // if flow already exist story it to temp
        temp =  NULL;

        // read packet 
        *pacInfo = proccessPacket(pcap);
        if (pacInfo->ok == false)
            break;
        if (pacInfo->protocol == UNKNOWN_PROTOCOL)
            continue;
        
        // apply active timer -a -> clean flows 
        if (applyActiveTimer(flowL, pacInfo->timeSec, settings.timerActive, \
            totalFlows, clientSock) == false)
            goto error4;

        // apply inactive timer -i -> clean flows
        if (applyInactiveTimer(flowL, pacInfo->timeSec, settings.interval, \
            totalFlows, clientSock) == false)
            goto error4;

        // if flow for that already exist     
        if ((temp = findIfExists(flowL, pacInfo)) != NULL){
            flowL->current = temp;
            updatePayload(temp->data->nfpayload, *pacInfo);
        }

        // if check tcp fin flag)        
        if (pacInfo->protocol == TCP){
            if (temp != NULL){
                // 0000 0000
                // 0000 0100  // reset 
                // 0000 0001  // sin 
                 if (((temp->data->nfpayload->tcpFlags & 4 ) > 0) \
                   || ((temp->data->nfpayload->tcpFlags & 1) > 0)){
                    deleteAndSend(flowL, totalFlows, temp, clientSock);
                }
            }
        }

        // if flows already exists  
        if (temp != NULL)
            continue;

        // delete the oldest one if cache is full  
        if (flowL->size >= settings.cacheSize)
            deleteOldest(flowL, totalFlows, clientSock);

        if (createFlow(flowL, pacInfo) == false)
            goto error3;     
    }

    //export all 
    while (flowL->first != NULL){
        deleteOldest(flowL, totalFlows, clientSock);
    }

    free(pacInfo);
    freeInitFlowList(flowL);
    free(collector);
    
    pcap_close(pcap);
    return 0;

error1:
    free(pacInfo);
    closeFile(settings.inputFile);
    fprintf(stderr, "ERROR, Invalid pcap file\n");
    return 1;
error2:
    free(pacInfo);
    pcap_close(pcap);
    fprintf(stderr, "ERROR, malloc error\n");
    return 2;

error3:
    free(pacInfo);
    freeInitFlowList(flowL);
    pcap_close(pcap);
    fprintf(stderr, "ERROR, malloc error\n");
    return 2;

error4:
    free(pacInfo);
    freeInitFlowList(flowL);
    pcap_close(pcap);
    free(collector);
    fprintf(stderr, "ERROR\n");
    return 2;

error5:
    free(pacInfo);
    pcap_close(pcap);
    fprintf(stderr, "ERROR can't create socket\n");
    return 2;


}

/**
 * @brief set default program settings in global var. settings 
 * -f input file    [DEFAULT NULL (STDIN)] {inputFile}
 * -c collector ip  [DEFAULT 127.0.0.1:2055] {collectorIp, collectorPort}
 * -a acctive timer [DEFAULT 60s] {timerActive}
 * -i interval      [DEFAULT 10s] {interval}
 * -m flow cache    [DEFAULT 1024] {cacheSize}
 */
struct set defaultSettings(){
    struct set setNew;
    setNew.inputFile      = NULL;
    setNew.collectorIp    = DEFAULT_IP;
    setNew.collectorPort  = DEFAULT_PORT;
    setNew.timerActive    = DEFAULT_TIMER;
    setNew.interval       = DEFAULT_INTERVAL;
    setNew.cacheSize      = DEFAULT_CACHE;
    return setNew;
}

