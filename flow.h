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

#include "args.h"

  