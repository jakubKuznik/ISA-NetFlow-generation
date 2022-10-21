// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        udp.h
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Fileprovide functions, for sending udp packets  

#ifndef UDP_H
#define UDP_H

#include "flow.h"
#include "netflowGen.h"

#define SOCKET_ERROR -1


/**
 * @brief Init server where flow will be send. sets ip and port 
 * 
 * @return sockaddr_in 
 */
struct sockaddr_in * initServer(struct set settings);


// function is inspirated from:  (c) Petr Matousek, 2016
bool sendUdpFlow(netFlow * nf, int clientSock);


/**
 * @brief establish connection with server using  connect()
 *  will set the global variable clientSock 
 * 
 * @param server collector  
 * @return client socket number
 * @retunr -1 if error 
 */
int startConnection(struct sockaddr_in server);

#endif