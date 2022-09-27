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
 */
void parseArgs(int argc, char *argv[]) {
    fprintf(stderr, "%d %s", argc, argv[0]);
    for (int i=0; i < argc; i++){
        fprintf(stderr, "%s\n",argv[i]);
    }

}