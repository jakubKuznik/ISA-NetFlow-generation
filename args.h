// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        args.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Read file arguments 

#pragma once

#include "flow.h"
#include "pcapFile.h"

/**
 * @brief Parse arguments and set settings variable 
 * 
 * @param argc agrs sum
 * @param argv args array 
 * @param settings struct with all the program settings  
 */
struct set parseArgs(int argc, char *argv[]);

/**
 * @brief Print hello message and ends a program with exit(1)
 */
void printHelp();


/**
 * @brief parse string to UINT16. using strtol
 * 
 * @return false if not sucesfull
 */
bool parseNumUINT16(char *s, uint16_t *out);


/**
 * @brief parse string to UINT32. using strtol
 * 
 * @return false if not sucesfull
 */
bool parseNumUINT32(char *s, uint32_t *out);