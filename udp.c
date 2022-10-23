// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        udp.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Fileprovide functions, for sending udp packets  

#include "udp.h"


// function is inspirated from:  (c) Petr Matousek, 2016
bool sendUdpFlow(netFlow * nf, int clientSock){
    int i;

    char message[NF_HEADER_SIZE + NF_PAYLOA_SIZE] = "";
    char * pt = &message[0]; // pointer to first element of array 

    htonsFlow(nf);
    //nf->nfheader->count = htons(1);

    memcpy(pt, &nf->nfheader->version, NF_HEADER_SIZE);
    pt = pt + NF_HEADER_SIZE;
    memcpy(pt, &nf->nfpayload->srcAddr, NF_PAYLOA_SIZE);

    i = send(clientSock, &message, NF_HEADER_SIZE + NF_PAYLOA_SIZE, 0);
    if (i == -1)
        fprintf(stderr, "ERROR: send data to collector failed\n");
    else if (i != NF_HEADER_SIZE + NF_PAYLOA_SIZE)
        fprintf(stderr, "ERROR: send(): buffer written partially\n");


  return true; 
}

/**
 * @brief establish connection with server using  connect()
 *  will set the global variable clientSock 
 * 
 * @param server collector  
 * @return client socket number
 * @retunr -1 if error 
 */
int startConnection(struct sockaddr_in server){
    int sock;   // socket descriptor
    
    //create a client socket
    if ((sock = socket(AF_INET , SOCK_DGRAM , 0)) == -1)
        goto errorUDP1;

    // create a connected UDP socket
    if (connect(sock, (struct sockaddr *)&server, sizeof(server))  == -1)
        goto errorUDP2;
    
    return sock;


errorUDP1:
    fprintf(stderr,"ERORR socket creation failed\n");
    return -1;

errorUDP2:
    fprintf(stderr, "ERORR connect() failed\n");
    return -1;
}

/**
 * @brief Init server where flow will be send. sets ip and port 
 * 
 * @return sockaddr_in 
 * @return NULL if error  
 */
struct sockaddr_in * initServer(struct set settings){
    struct sockaddr_in *server = malloc(sizeof(struct sockaddr_in));
    if (server == NULL)
        return NULL;

    server->sin_family = AF_INET;
    server->sin_addr.s_addr = ntohl(settings.collectorIp);

    // server port (network byte order)
    server->sin_port = ntohs(settings.collectorPort);
    return server;
}