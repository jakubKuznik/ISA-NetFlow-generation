// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        netflow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Generate netflow from packet.

#include "netflowGen.h"



/******************   HEADER  ********************************
 0-1     2-3      4-7       8-11       12-15         16-19          
version count  SysUptime  unix_secs  unix_nsecs  flow_sequence  

       20            21            22-23
   engine_type   engine_id   sampling_interval 
***************************************************************/
void NFGenerateHeader(){
  printf(" ");
}


/******************  RECORD FORMAT ************************
  0-3       4-7      8-11    12-13   14-15   16-19    20-23    
 srcaddr  dst-addr  nexthop  input   output  dPkts  dOctents

  24-27    28-31     32-33    34-35     36       37      38
  firts    Last     srcport  dstport   pad1   tcpflags  port

   39    40-41    42-43      44        45       46-47
  tos    src_as   dst_as  src_mask  dst_mask    pad2 
************************************************************/
void NFGeneratePayload(){
    printf(" ");
}

void genNetflow(){
    printf(".");
}

/**
 * @brief Get the Last Flow object
 * 
 * @return int 
 */
int getLastFlow(){
    return 2;
}

/**
 * @brief Get the Last Flow object
 * 
 * @return int 
 */
int getFirstFlow(){
    return 2;
}

/**
 * @brief 
 * 
 */
void removeOldestFlow(){
    printf("..");
}
