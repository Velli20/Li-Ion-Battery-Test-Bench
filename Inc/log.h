#ifndef __LOG_H
#define __LOG_H

// Includes

#include <stdio.h>
#include <string.h>
#include "stm32f7xx_hal.h"

// Macros and defines

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if defined(WITH_DEBUG_LOG)
#define DEBUG_LOG(MESSAGE)                                                     \
(                                                                              \
    printf("in file=%s line=%d message=%s\n", __FILENAME__, __LINE__, MESSAGE) \
)
#endif

#define DEBUG_ASSERT(CONDITION)                                                             \
    do                                                                                      \
    {                                                                                       \
        if ( !(CONDITION) )                                                                 \
        {                                                                                   \
            printf("Error=!(%s) in file=%s line=%d\n", #CONDITION, __FILENAME__, __LINE__); \
            while ( 1 ) { ;; }                                                              \
        }                                                                                   \
    } while ( 0 )


// Log_Config

void Log_Config(void);

#endif
