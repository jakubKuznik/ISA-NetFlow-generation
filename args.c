// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        args.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Read file arguments 

#include "args.h"

/**
 * @brief Parse arguments and set settings variable 
 * 
 * @param argc agrs sum
 * @param argv args array
 * @param settings struct with all the program settings  
 */
struct set parseArgs(int argc, char *argv[]) {
    set setNew;
    setNew = defaultSettings();

    fprintf(stderr, "%d %s", argc, argv[0]);
    for (int i=1; i < argc; i++){
        // help message 
        if ((strcmp(argv[i], "-h") == 0 ) || (strcmp(argv[i], "--help") == 0)) {
            printHelp();
        } // -f 
        else if (strcmp(argv[i], "-f") == 0){
            if (++i >= argc)
                goto error1;
            setNew.inputFile = fopen(argv[i], "r");
            if (setNew.inputFile == NULL)
                goto error2;
        } // -c
        else if (strcmp(argv[i], "-c") == 0){
            if (++i >= argc)
                goto error1;
            // parse ip and port 
            if (parseCollector(argv[i], &setNew.collectorIp, &setNew.collectorPort) == false)
                goto error4;
        } // -a 
        else if (strcmp(argv[i], "-a") == 0){
            if (++i >= argc)
                goto error3;

            // parse num 
            if (parseNumUINT32(argv[i], &setNew.timerActive) == false)
                goto error4;

        } // -i
        else if (strcmp(argv[i], "-i") == 0){
            if (++i >= argc)
                goto error3;

            // parse num 
            if (parseNumUINT32(argv[i], &setNew.interval) == false)
                goto error4;

        } // -m 
        else if (strcmp(argv[i], "-m") == 0){
            if (++i >= argc)
                goto error3;

            // parse num 
            if (parseNumUINT32(argv[i], &setNew.cacheSize) == false)
                goto error4;
        } 
        else{
            fprintf(stderr, "\nERROR: invalid parrameter %s\n",argv[i]);
            exit(1);
        }

    }

    return setNew;

error1:
    fprintf(stderr, "\nERROR: missing file name\n");
    exit(1);

error2:
    fprintf(stderr, "\nERROR: Can't open input file\n");
    exit(1);
error3:
    fprintf(stderr, "\nERROR: missing time after -i\n");
    closeFile(setNew.inputFile);
    exit(1);
error4:
    fprintf(stderr, "\nERROR: not a valid number\n");
    closeFile(setNew.inputFile);
    exit(1);

}

/**
 * @brief Parse -c argument to ip and port.
 *      [-c <netflow_collector>[:<port>]] 
 *      netflow_collector can be either ip or hostname 
 * @param in input variable
 * @param colIp output variable
 * @param colPort output variable 
 * 
 * @return false if not succesfull.
 */
bool parseCollector(char *in, uint32_t *colIp, uint16_t *colPort){


    printf("%s, %d %d",in, *colIp, *colPort);
    printf("\n%ld\n",strlen(in));
    
    char *tokenIp = strtok(in, ":");
    char *tokenPort = strtok(NULL, ":");

    // error multiple ':'
    if(strtok(NULL, ":") != NULL)
        return false;
    
    *colIp = inet_addr(in);
    if(*colIp == (in_addr_t)(-1)){ // if not succ try parse hostname 
        struct hostent *lh = gethostbyname(tokenIp);
        if (lh == NULL) // non valid hostname 
            return false;
        
        // TODO there can be multiple addresses behind hostname
        memcpy(&(*colIp), lh->h_addr, lh->h_length);
    }

    // bad port num 
    if(parseNumUINT16(tokenPort, colPort) == false)
        return false;

    
    return true;
}

/**
 * @brief parse string to UINT16. using strtol
 * 
 * @return false if not sucesfull
 */
bool parseNumUINT16(char *s, uint16_t *out){
    char *temp;
    long number; 

    number = strtol(s, &temp, 10);
    if (*s != '\0' && *temp == '\0'){ // valid strtol
        if (number > UINT16_MAX || number < 0){
            return false;
        }
        else{
            *out = (u_int16_t) number;
            return true;
        }
    } 

    return false;
}

/**
 * @brief parse string to UINT32. using strtol
 * 
 * @return false if not sucesfull
 */
bool parseNumUINT32(char *s, uint32_t *out){
    char *temp;
    long number; 
    

    number = strtol(s, &temp, 10);
    if (*s != '\0' && *temp == '\0'){ // valid strtol
        if (number > UINT32_MAX || number < 0){
            return false;
        }
        else{
            *out = (u_int32_t) number;
            return true;
        }
    } 
    return false;
}

/**
 * @brief Print hello message and ends a program with exit(1)
 * 
 */
void printHelp(){
    printf("./flow [-f <file>] [-c <netflow_collector>[:<port>]] [-a \n");
    printf("       <active_timer>] [-i <inactive_timer>] [-m <count>]\n");
    exit(1);
}