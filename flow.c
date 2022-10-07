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
set settings;


int main(int argc, char *argv[]) {

    // flow counter every time flow is send ++
    uint32_t i = 0;
    uint32_t *totalFlows = &i;

    // set program settings 
    settings = parseArgs(argc, argv);
    // buffer that is used in pcap struct  
    char pcapBuff[MY_PCAP_BUFF_SIZE];
    pcap_t *pcap;
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
    
    

    node *temp;

    packetInfo lastValid;
    while(true){
        // read packet 
        *pacInfo = proccessPacket(pcap);
        if (pacInfo->ok == false){
            break;
        }
        if(pacInfo != NULL)
            lastValid = *pacInfo;

        // apply active timer -a -> clean flows 
        /*
        if (applyActiveTimer(flowL, pacInfo->timeSec, settings.timerActive, collector, totalFlows, *pacInfo) == false){
            printf("ac lulik");
            goto error4;
        }

        // apply inactive timer -i -> clean flows
        if (applyInactiveTimer(flowL, pacInfo->timeSec, settings.interval, collector, totalFlows, *pacInfo) == false){
            printf("in lulik");
            goto error4;
        }
        */

        // if flow for that already exist     
        if ((temp = findIfExists(flowL, pacInfo)) != NULL){
            updatePayload(temp->data->nfpayload, *pacInfo);
            continue;
        }

        // if (todo check tcp fin flag)        

        // delete the oldest one if cache is full  
        if (flowL->size >= settings.cacheSize)
            deleteOldest(flowL, collector, totalFlows, lastValid);


        if (createFlow(flowL, pacInfo) == false)
            goto error3;     


    }

    //export all 
    while (flowL->first != NULL){
        deleteOldest(flowL, collector, totalFlows, lastValid);
        printf("\nwuiii %d\n",i);
    }

    free(pacInfo);
    freeInitFlowList(flowL);
    
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
    fprintf(stderr, "ERROR\n");
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
set defaultSettings(){
    set setNew;
    setNew.inputFile      = NULL;
    setNew.collectorIp    = DEFAULT_IP;
    setNew.collectorPort  = DEFAULT_PORT;
    setNew.timerActive    = DEFAULT_TIMER;
    setNew.interval       = DEFAULT_INTERVAL;
    setNew.cacheSize      = DEFAULT_CACHE;
    return setNew;
}

