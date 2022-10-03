// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        pTime.h 
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// pTime.c header file  

#pragma once

#include "flow.h"
#include "pcapFile.h"



/**
 * @brief Get the time in miliseconds since device booted  
 * @return long time in miliseconds 
 * @return -1 if error;
 */
long getBootTime();