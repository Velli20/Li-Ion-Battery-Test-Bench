#ifndef __LOG_H
#define __LOG_H

// Includes

#include <stdio.h>
#include <string.h>
#include "stm32f7xx_hal.h"

// Macros and defines

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Debug log macro. Outputs messages to the UART console.

#if defined(WITH_DEBUG_LOG)                                                    
#define DEBUG_LOG(FORMAT, ...)                                                 \
    do                                                                         \
    {                                                                          \
        printf("in file=%s line=%d message=", __FILENAME__, __LINE__);         \
        printf((FORMAT), ##__VA_ARGS__);                                       \
        printf("\n\r");                                                          \
    }                                                                          \
    while (0)
#else
#define DEBUG_LOG(FORMAT, ...)(((void*)FORMAT), (void*)##__VA_ARGS__)
#endif

#define DEBUG_ASSERT(CONDITION)                                                             \
    do                                                                                      \
    {                                                                                       \
        if ( !(CONDITION) )                                                                 \
        {                                                                                   \
            printf("Error=!(%s) in file=%s line=%d\n", #CONDITION, __FILENAME__, __LINE__); \
            while ( 1 ) { ;; }                                                              \
        }                                                                                   \
    }                                                                                       \
    while (0)


// Log_Config

void Log_Config(void);

#endif
