// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        udp.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Fileprovide functions, for sending udp packets  

#include "udp.h"


// function is inspirated from:  (c) Petr Matousek, 2016
bool sendUdpFlow(struct set settings, netFlow nf, int clientSock, struct sockaddr_in *server){
    int msg_size, i;
    struct sockaddr_in from; // address structures of the server and the client
    socklen_t len, fromlen;        
    char buffer[BUFFER];            

    int sock = 
   
     
    len     = sizeof(server);
    fromlen = sizeof(from);

    //i = send(sock, )
    /*
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
  */
  // reading data until end-of-file (CTRL-D)

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
 */
struct sockaddr_in * initServer(struct set settings){
    struct sockaddr_in *server;

    server->sin_family = AF_INET;                   
    server->sin_addr.s_addr = settings.collectorIp;
    
    // server port (network byte order)
    server->sin_port = settings.collectorPort;
    return server;    
}