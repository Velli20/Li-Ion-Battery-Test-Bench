
// Includes

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server.h"
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
    "\r\n\r\n"

// server_write_cycle_result_text_file

uint8_t server_write_cycle_result_text_file(struct netconn* conn,
                                            uint32_t        cycle)
{
    FATFS    SDFatFs; 
    FIL      file;
    char     sd_path[4];
    char     file_path[15];
    char     out_buffer[128];
    uint32_t bytes_read;
    uint32_t bytes_written;
    uint32_t bytes_to_read;
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

    result= SERVER_RESULT_OK;
    
    // Register the file system object to the FatFs module.
    
    if ( f_mount(&SDFatFs, (const TCHAR*)sd_path, 0) != FR_OK )
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

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


    // Write header.

    if ( netconn_write(conn, HTTP_BATTERY_DATA_RESPONSE_HEADER, sizeof(HTTP_BATTERY_DATA_RESPONSE_HEADER)-1, NETCONN_NOCOPY) != ERR_OK )
    {
        result= SERVER_RESULT_ERROR;
        goto end;
    }

    // Split large files in parts.

    bytes_written= 0;
    bytes_read=    0;

    do
    {
        // Seek file to current position.

        if ( bytes_written > 0 )
            f_lseek(&file, (DWORD)bytes_written);
            
        memset(out_buffer, 0, sizeof(out_buffer));

        // Read file.
        
        bytes_to_read= (bytes_written + (sizeof(out_buffer)-1)) > file_size ? (file_size - bytes_written) : (sizeof(out_buffer)-1);
        if ( f_read(&file, out_buffer, bytes_to_read, (UINT*) &bytes_read) != FR_OK )
        {
            result= SERVER_RESULT_ERROR;
            goto end;
        }
        
        // Write file.
        
        if ( netconn_write(conn, out_buffer, bytes_to_read, NETCONN_COPY) != ERR_OK )
        {
            result= SERVER_RESULT_ERROR;
            goto end;
        }

        bytes_written+= bytes_read;

        DEBUG_LOG("Bytes written=%lu", bytes_written);
    }
    while ( bytes_written < file_size );

    end:

    // Close the data file.

    f_close(&file);

    // Unlink driver.
    
    FATFS_UnLinkDriver(sd_path);
    
    return (result);
}
