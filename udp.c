// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        udp.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Fileprovide functions, for sending udp packets  

#include "udp.h"


// function is inspirated from:  (c) Petr Matousek, 2016
bool sendUdpFlow(struct set settings, struct netFlow * nf, struct sockaddr_in *server){
    int msg_size, i;
    struct sockaddr_in from; // address structures of the server and the client
    socklen_t len, fromlen;        
    char buffer[BUFFER];            

    len     = sizeof(server);
    fromlen = sizeof(from);

    int clientSock = startConnection(*server);


    char message[NF_HEADER_SIZE + NF_PAYLOA_SIZE] = "";
    char * pt = &message; // pointer to first element of array 


    printf(".... %d %d %d %d %d %d %d %d %d ... ",nf->nfheader->version, nf->nfheader->count, nf->nfheader->sysUpTime, nf->nfheader->unixSecs, nf->nfheader->unixNSecs, nf->nfheader->flowSequence, nf->nfheader->engineType, nf->nfheader->engineId, nf->nfheader->samplingInterval);
    htonsFlow(nf);
    printf(".... %d %d %d %d %d %d %d %d %d ... ",nf->nfheader->version, nf->nfheader->count, nf->nfheader->sysUpTime, nf->nfheader->unixSecs, nf->nfheader->unixNSecs, nf->nfheader->flowSequence, nf->nfheader->engineType, nf->nfheader->engineId, nf->nfheader->samplingInterval);
    //nf->nfheader->count = htons(1);

    memcpy(pt, &nf->nfheader->version, NF_HEADER_SIZE);
    pt = pt + NF_HEADER_SIZE;
    memcpy(pt, &nf->nfpayload->srcAddr, NF_PAYLOA_SIZE);

    /*
    printf("\n......  ");
    for (int i = 0; i < NF_HEADER_SIZE + NF_PAYLOA_SIZE; i++)
        printf("%d",message[i]);
    printf("  ........\n");
    */


    i = send(clientSock, &message, NF_HEADER_SIZE + NF_PAYLOA_SIZE, 0);
    if (i == -1)
        fprintf(stderr, "ERROR: send data to collector failed\n");
    else if (i != NF_HEADER_SIZE + NF_PAYLOA_SIZE)
        fprintf(stderr, "ERROR: send(): buffer written partially\n");


  return true; 
}



/**
 * @brief establish connection with server using  connect() 
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
    server->sin_addr.s_addr = settings.collectorIp;

    // server port (network byte order)
    server->sin_port = htons(settings.collectorPort);
    return server;
}