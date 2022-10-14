// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        flow.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Read file arguments 

#ifndef FLOW_H 
#define FLOW_H

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
#include <pcap/pcap.h>

#include "args.h"
#include "flow.h"
#include "netflowGen.h"
#include "udp.h"

/* Default program settings */
#define DEFAULT_PORT 2055
// TODO maybe change 
#define DEFAULT_IP 16777343 // 127.0.0.1 
#define DEFAULT_TIMER 60
#define DEFAULT_INTERVAL 10
#define DEFAULT_CACHE 1024

// errbuf is assumed to be able to hold at least PCAP_ERRBUF_SIZE chars.
#define MY_PCAP_BUFF_SIZE 2048
#define BUFFER 4096


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
struct set defaultSettings();


#endif 
