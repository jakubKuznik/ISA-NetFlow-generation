// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        pTime.h 
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// pTime.c header file  

#pragma once

#include "flow.h"
#include "pcapFile.h"

/**
 * @brief get time in seconds since 0000 UTC 1970 
 * 
 * @return long time
 */
long getUTCinSec();

/**
 * @brief get time in Nseconds since 0000 UTC 1970 
 * 
 * @return long 
 */
long getUTCinNsec();

/**
 * @brief Get the time in miliseconds since device booted  
 * @return long time in miliseconds 
 * @return -1 if error;
 */
long getBootTime();