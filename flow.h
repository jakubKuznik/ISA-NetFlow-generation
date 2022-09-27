// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        flow.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Read file arguments 


#pragma once
// normal libraries 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

// network libraries
#include <arpa/inet.h>
#include <netinet/if_ether.h> //ethernet and arp frame 
#include <netinet/ip_icmp.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netdb.h>

#include "args.h"
#include "flow.h"

/* Default program settings */
#define DEFAULT_PORT 2055
#define DEFAULT_IP 1270001
#define DEFAULT_TIMER 60
#define DEFAULT_INTERVAL 10
#define DEFAULT_CACHE 1024

/**
 * @brief Settings struct for the program 
 * 
 * -f input file    [DEFAULT NULL (STDIN)] {inputFile}
 * -c collector ip  [DEFAULT 127.0.0.1:2055] {collectorIp, collectorPort}
 * -a acctive timer [DEFAULT 60s] {timerActive}
 * -i interval      [DEFAULT 10s] {interval}
 * -m flow cache    [DEFAULT 1024] {cacheSize}
 */
struct set{
    //TODO ASSIGN DEFAULT VALUES 
    FILE *inputFile;            // input file if NULL we will use STDIN 
    in_addr_t collectorIp;      // netflow collector ip address 
    in_port_t collectorPort;    // netflow collector ip address 
    uint32_t timerActive;       // timmer in sec 
    uint32_t cacheSize;         // 
    uint32_t interval;          // after end non-active flows will be 
                                // exported to collector 
};

typedef struct set set;


//TODO delete 
void debugStruct();

/**
 * @brief set default program settings in global var. settings 
 * -f input file    [DEFAULT NULL (STDIN)] {p}
 * -c collector ip  [DEFAULT 127.0.0.1:2055] {collectorIp}
 * -a acctive timer [DEFAULT 60s] {timerActive}
 * -i interval      [DEFAULT 10s] {interval}
 * -m flow cache    [DEFAULT 1024] {cacheSize}
 * 
 * @return Settings structure
 */
set defaultSettings();


  