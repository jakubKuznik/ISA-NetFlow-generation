// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        flow.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Main file that also has udp  

// Execution:
// ./flow [-f <file>] [-c <netflow_collector>[:<port>]] [-a <active_timer>] 
//        [-i <inactive_timer>] [-m <count>]

#include "flow.h"

// GLOBAL settings variable
set settings;


int main(int argc, char *argv[]) {

    // flow counter every time flow is send ++
    uint32_t totalFlows = 0;

    debugStruct();
    // set program settings 
    settings = parseArgs(argc, argv);
    // buffer that is used in pcap struct  
    char pcapBuff[MY_PCAP_BUFF_SIZE];
    pcap_t *pcap;
    // basic info about packet that is need for netflow 
    packetInfo * pacInfo = malloc(sizeof(packetInfo));
    // list with all the flows 
    flowList * flowL; 

    if ((pcap = openPcapFile(settings.inputFile, pcapBuff)) == NULL){
        goto error1;
    }

    // init flow list  
    if((flowL = initFlowList()) == NULL)
        goto error2;

    node *temp;
    while(true){
        // read packet 
        *pacInfo = proccessPacket(pcap);
        if (pacInfo->ok == false)
            break;

        // apply active timer -a -> clean flows 
        if (applyActiveTimer(flowL, pacInfo->pacTime, settings.timerActive) == false)
            goto error4;

        // apply inactive timer -i -> clean flows
        if (appplyInactiveTimer(flowL, pacInfo->pacTime, settings.interval) == false)
            goto error4;


        // if flow for that already exist     
        if ((temp = findIfExists(flowL, pacInfo)) != NULL){
            printf("\n*****HIHI*******\n");
            updatePayload(&temp->data->nfpayload, *pacInfo);
            continue;;
        }        

        // delete the oldest one if cache is full  
        if (flowL->size >= settings.cacheSize){
            printf("1");
        }

        if (createFlow(flowL, pacInfo) == false)
            goto error3;
        
        
        // parse data from packet to struct 
        /*
        proccess_packet();
        generate_netflow();
        send_netflow();
        */

    }


    free(pacInfo);
    freeInitFlowList(flowL);
    debugStruct();
    
    pcap_close(pcap);
    return 0;

error1:
    free(pacInfo);
    closeFile(settings.inputFile);
    fprintf(stderr, "ERROR, Invalid pcap file\n");
    return 1;
error2:
    free(pacInfo);
    pcap_close(pcap);
    fprintf(stderr, "ERROR, malloc error\n");
    return 2;

error3:
    free(pacInfo);
    freeInitFlowList(flowL);
    pcap_close(pcap);
    fprintf(stderr, "ERROR, malloc error\n");
    return 2;

error4:
    free(pacInfo);
    freeInitFlowList(flowL);
    pcap_close(pcap);
    fprintf(stderr, "ERROR\n");
    return 2;


}


// TODO CREATE UDP FILE 
// TODO FUNKCE NA POSILANI DAT 
// TODO FUNKCE NA OTEVRENI SPOJENI SE SERVEREM 
// function is inspirated from:  (c) Petr Matousek, 2016
bool sendUdpFlow(){
    int sock;                        // socket descriptor
    int msg_size, i;
    struct sockaddr_in server, from; // address structures of the server and the client
    socklen_t len, fromlen;        
    char buffer[BUFFER];            

    server = initServer();

   
    //create a client socket
    if ((sock = socket(AF_INET , SOCK_DGRAM , 0)) == -1)
        goto errorUDP1;
     
    len     = sizeof(server);
    fromlen = sizeof(from);

    // create a connected UDP socket
    if (connect(sock, (struct sockaddr *)&server, sizeof(server))  == -1)
        goto errorUDP2;

    //send data to the server
    while((msg_size=read(STDIN_FILENO,buffer,BUFFER)) > 0) 
      // read input data from STDIN (console) until end-of-line (Enter) is pressed
      // when end-of-file (CTRL-D) is received, n == 0
    { 
        i = send(sock,buffer,msg_size,0);     // send data to the server
        if (i == -1)                   // check if data was sent correctly
            err(1,"send() failed");
        else if (i != msg_size)
            err(1,"send(): buffer written partially");

        // obtain the local IP address and port using getsockname()
        if (getsockname(sock,(struct sockaddr *) &from, &len) == -1)
            err(1,"getsockname() failed");

        printf("* Data sent from %s, port %d (%d) to %s, port %d (%d)\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port), from.sin_port, inet_ntoa(server.sin_addr),ntohs(server.sin_port), server.sin_port);
        
        // read the answer from the server 
        if ((i = recv(sock,buffer, BUFFER,0)) == -1)   
            err(1,"recv() failed");
        else if (i > 0){
            // obtain the remote IP adddress and port from the server (cf. recfrom())
            if (getpeername(sock, (struct sockaddr *)&from, &fromlen) != 0) 
                err(1,"getpeername() failed\n");

            printf("* UDP packet received from %s, port %d\n",inet_ntoa(from.sin_addr),ntohs(from.sin_port));
            printf("%.*s",i,buffer);                   // print the answer
        }
  } 
  // reading data until end-of-file (CTRL-D)

  if (msg_size == -1)
    err(1,"reading failed");
  close(sock);
  printf("* Closing the client socket ...\n");
  
  return true; 


errorUDP1:
    fprintf(stderr,"ERORR socket failed\n");
    return false;
    
errorUDP2:
    fprintf(stderr, "ERORR connect() failed\n");
    return false;
}

/**
 * @brief Init server where flow will be send. sets ip and port 
 * 
 * @return sockaddr_in 
 */
struct sockaddr_in initServer(){
    struct sockaddr_in server;

    memset(&server,0,sizeof(server)); // erase the server structure
    server.sin_family = AF_INET;                   

    // copy the first parameter to the server.sin_addr structure
    memcpy(&server.sin_addr, settings.collectorIp , sizeof(uint32_t)); 
    
    // server port (network byte order)
    server.sin_port = settings.collectorPort;
    return server;    
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

