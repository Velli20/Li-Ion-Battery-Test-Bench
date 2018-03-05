
// Includes

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server.h"
#include "FreeRTOS.h"
#include "log.h"

// SD card interface includes

#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sd.h"

// HTTP header

#define HTTP_BATTERY_DATA_RESPONSE_HEADER \
    "HTTP/1.1 200 OK\r\n"                 \
    "Access-Control-Allow-Origin: *\r\n"  \
    "Content-Type: application/text\r\n"  \
    "\r\n"

#define MESSAGE_OUT_OF_MEMORY             \
    "Error: Running out of memory."       \
    "Required heap %d available heap=%d"
    
// server_write_cycle_result_text_file

uint8_t server_write_cycle_result_text_file(struct netconn* conn,
                                            uint32_t        cycle)
{
    FATFS    SDFatFs; 
    FIL      file;
    char     sd_path[4];
    char     file_path[15];
    char     err_message_body[128];
    char*    file_text;
    uint32_t bytes_read;
    uint8_t  result;
    size_t   file_size;

    if ( !conn )
        return (SERVER_RESULT_ERROR);
        
    // Check that SD card is inserted.

    if ( BSP_SD_IsDetected() != SD_PRESENT ) 
        return (SERVER_RESULT_ERROR);

    // Link the SD card I/O driver.

	memset(sd_path, 0, sizeof(sd_path));
	if ( FATFS_LinkDriver(&SD_Driver, sd_path) != FR_OK ) 
		return (SERVER_RESULT_ERROR);

    // Register the file system object to the FatFs module.

	if ( f_mount(&SDFatFs, (const TCHAR*)sd_path, 0) != FR_OK )
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

    result=    SERVER_RESULT_OK;
    file_text= 0;

    // Format file path string.

    memset(file_path, 0, sizeof(file_path));
    snprintf(file_path, sizeof(file_path), "c%lu.txt", cycle);

    // Try to open data file.

    if ( f_open(&file, (const TCHAR*)file_path, FA_READ) != FR_OK ) 
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

    // Get file size.

    file_size= f_size(&file);
	if ( file_size == 0 )
	{
        result= SERVER_RESULT_ERROR;
        goto end;
    }
    
    // Check if we have enough memory available.

    if ( (file_size*2) >= xPortGetFreeHeapSize() )
    {
        // Return error message.

        memset(err_message_body, 0, sizeof(err_message_body));
        snprintf(err_message_body, sizeof(err_message_body), MESSAGE_OUT_OF_MEMORY, file_size*2, xPortGetFreeHeapSize());
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG(err_message_body);
#endif
        // Write header.

        if ( netconn_write(conn, HTTP_BATTERY_DATA_RESPONSE_HEADER, sizeof(HTTP_BATTERY_DATA_RESPONSE_HEADER)-1, NETCONN_NOCOPY) != ERR_OK )
        {
            result= SERVER_RESULT_ERROR;
            goto end;
        }

        // Write error message.

        if ( netconn_write(conn, err_message_body, strnlen(err_message_body, sizeof(err_message_body)), NETCONN_COPY) != ERR_OK )
        {
            result= SERVER_RESULT_ERROR;
        }

        goto end;
    }

    // Allocate memory for file.

    file_text= calloc(file_size, sizeof(char));
    if ( !file_text )
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

    // Read file.

	if ( f_read(&file, file_text, file_size, (UINT*) &bytes_read) != FR_OK )
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

    // Write header.

    if ( netconn_write(conn, HTTP_BATTERY_DATA_RESPONSE_HEADER, sizeof(HTTP_BATTERY_DATA_RESPONSE_HEADER)-1, NETCONN_NOCOPY) != ERR_OK )
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

    // Write file.

    if ( netconn_write(conn, file_text, file_size-1, NETCONN_COPY) != ERR_OK )
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

    end:

    // Deallocate memory.

    if ( file_text )
        free(file_text);

    // Close the data file.

    f_close(&file);

    // Unlink driver.

	FATFS_UnLinkDriver(sd_path);
    
    return (result);
}
