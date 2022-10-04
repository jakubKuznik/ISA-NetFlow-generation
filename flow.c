// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        flow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Read file arguments 

// Execution:
// ./flow [-f <file>] [-c <netflow_collector>[:<port>]] [-a <active_timer>] 
//        [-i <inactive_timer>] [-m <count>]

#include "flow.h"

// GLOBAL settings variable
set settings;


int main(int argc, char *argv[]) {

    // flow counter every time flow is send ++
    uint32_t totalFlows = 0;

    debugStruct();
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

    node *temp;
    while(true){
        // read packet 
        *pacInfo = proccessPacket(pcap);
        if (pacInfo->ok == false)
            break;

        // apply active timer -a -> clean flows 
        if (applyActiveTimer(flowL, pacInfo->pacTime, settings.timerActive) == false)
            goto error4;

        // apply inactive timer -i -> clean flows
        if (appplyInactiveTimer(flowL, pacInfo->pacTime, settings.interval))
            goto error4;


        // if flow for that already exist     
        if ((temp = findIfExists(flowL, pacInfo)) != NULL){
            updatePayload(&temp->data.nfpayload, *pacInfo);
            break;
        }        

        // delete the oldest one 
        if (flowL->size >= settings.cacheSize){
            printf("1");
        }

        if (createFlow(flowL, pacInfo) == false)
            goto error3;
        

        
        
        // parse data from packet to struct 
        /*
        proccess_packet();
        generate_netflow();
        send_netflow();
        */

    }

    free(pacInfo);
    freeInitFlowList(flowL);
    debugStruct();
    
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

//TODO delete 
void debugStruct(){
    fprintf(stderr, ".....file....... %p \n",settings.inputFile);
    fprintf(stderr, ".....cIp........ %u \n",settings.collectorIp);
    fprintf(stderr, ".....cPor....... %u \n",settings.collectorPort);
    fprintf(stderr, ".....interval... %u \n",settings.interval);
    fprintf(stderr, ".....timer...... %u \n",settings.timerActive);
    fprintf(stderr, ".....cache...... %u \n",settings.cacheSize);


}

