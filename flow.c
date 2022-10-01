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

    debugStruct();
    // set program settings 
    settings = parseArgs(argc, argv);

    // buffer that is used in pcap struct  
    char pcapBuff[MY_PCAP_BUFF_SIZE];
    pcap_t *pcap;
    // array that has -m size //todo verify -1 +1 error  
    netFlow flowArray[settings.cacheSize];
    // information about array {firs, last, size, current}
    flowArrayInfo fArrayInfo = {0, 0, settings.cacheSize, 0};

    if ((pcap = openPcapFile(settings.inputFile, pcapBuff)) == NULL){
        goto error1;
    }
    
    while(true){

        // read packet 
        if((proccessPacket(pcap)) == NULL){
            break;
        } 
    
        

        //header.

        /*
        // get next packet 
        read_packet_from_pcap();

        // parse data from packet to struct 
        proccess_packet();
        generate_netflow();
        send_netflow();
        */

    }

    debugStruct();
    
    closeFile(settings.inputFile);
    return 0;

error1:
    closeFile(settings.inputFile);
    fprintf(stderr, "ERROR, Invalid pcap file\n");
    return 1;

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

