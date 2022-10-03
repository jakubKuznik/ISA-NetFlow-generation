// Solution for ISA-NetFlow-generation, 25.9.2022
// File:        pTime.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0
// Do the time stuff  

#include "pTime.h"

/**
 * @brief get time in seconds since 0000 UTC 1970 
 * 
 * @return long 
 */
long getUTCinSec(){
    return time(NULL); 
}

/**
 * @brief get time in Nseconds since 0000 UTC 1970 
 * 
 * @return long 
 */
long getUTCinNsec(){
    return (1000000000 * time(NULL)); 
}

/**
 * @brief Get the time in miliseconds since device booted  
 * @return long time in miliseconds 
 * @return -1 if error;
 */
long getBootTime(){
    FILE *f = fopen("/proc/uptime", "r");
    if (f == NULL)
        return -1;

    int c; 
    char tim[100]; 
    //~ cat /proc/uptime 
    // 81600.25 273863.24
    int i = 0;
    for (; (c = fgetc(f)) != EOF; i++){
        tim[i] = c;
        if (c == '.'){
            tim[++i] = fgetc(f);
            tim[++i] = fgetc(f);
            tim[++i] = '\0';
            break;
        }
    }
    long out = atol(tim);
    out = out * 1000; // sec to milisec 
    return out; 
}