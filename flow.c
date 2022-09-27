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
    
    /*
    char ipCh[20] = "127.0.0.1";
    uint32_t  ipI = 0;
    ipI = inet_addr(ipCh);
    printf("\n%s ... %d\n", ipCh, ipI); 
    */

    debugStruct();
    // set default 
    defaultSettings();
    settings = parseArgs(argc, argv);
    
    debugStruct();

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
    settings.inputFile      = NULL;
    settings.collectorIp    = DEFAULT_IP;
    settings.collectorPort  = DEFAULT_PORT;
    settings.timerActive    = DEFAULT_TIMER;
    settings.interval       = DEFAULT_INTERVAL;
    settings.cacheSize      = DEFAULT_CACHE;
    
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

