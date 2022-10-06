// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        udp.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Fileprovide functions, for sending udp packets  

#pragma once

#include "flow.h"
#include "netflowGen.h"

/**
 * @brief Init server where flow will be send. sets ip and port 
 * 
 * @return sockaddr_in 
 */
struct sockaddr_in * initServer(struct set settings);


bool sendUdpFlow(struct netFlow *nf, struct sockaddr_in *server);

/**
 * @brief establish connection with server using  connect() 
 * 
 * @param server collector  
 * @return client socket number
 * @retunr -1 if error 
 */
int startConnection(struct sockaddr_in server);