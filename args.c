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
set parseArgs(int argc, char *argv[]) {
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
            printf("....i %d argc %d....",i,argc);
            printf("-f\n");
        } // -c
        else if (strcmp(argv[i], "-c") == 0){
            printf("-c\n");
        } // -a 
        else if (strcmp(argv[i], "-a") == 0){
            printf("-a\n");
        } // -i
        else if (strcmp(argv[i], "-i") == 0){
            printf("-i\n");
        } // -m
        else if (strcmp(argv[i], "-m") == 0){
            printf("-m\n");
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