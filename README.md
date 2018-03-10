
# Li-Ion Battery Test Bench

This is a school project aimed to build system that can automatically charge and discharge Li-ion batteries at variable load. The purpose of this system is to study different battery types by monitoring cell voltage, temperature and current.

### Compiling

To build this project run Makefile. Make sure you have the GNU Arm Embedded Toolchain installed.

```sh
$ make -f .\Makefile
```

### Flashing with ST-Link Utlity software

- Click on "External Loader" from the menu and select N25Q128A_STM32F746G-DISCO
- Click on Target->Program and select "Li-Ion-Battery-Test-Bench.hex" under BUILD directory
- Hit on "Start" button to flash internal and external memory

### Pinout

| STM32F746G-DISCO PIN| Description |
| ------ | ------ |
| D13 | LTC6811 SPI SCK |  
| D12 | LTC6811 SPI MISO |
| D11 | LTC6811 SPI MOSI |
| D10 | LTC6811 SPI CS |
| D9 | Cell discharge relay 1 |
| D8 | Cell discharge relay 2 |
| D7 | Cell discharge relay 3 |
| D6 | Cell discharge relay 4 |
| D5 | Charging relay |

### Project pictures

- Web interface used to control test bench

<img src="https://github.com/Velli20/Li-Ion-Battery-Test-Bench/blob/master/project_pictures/web_interface.jpg?raw=true?" width="800" >

- Test bench


<img src="https://github.com/Velli20/Li-Ion-Battery-Test-Bench/blob/master/project_pictures/test_bench.jpg?raw=true" width="800" >

# Libraries used in this project

#### lwIP – A Lightweight TCP/IP stack 
- https://savannah.nongnu.org/projects/lwip/
- Licence: https://github.com/Velli20/Li-Ion-Battery-Test-Bench/blob/master/Middlewares/Third_Party/LwIP/LWIP_LICENCE

#### FatFS – Generic FAT Filesystem Module 
- http://elm-chan.org/fsw/ff/00index_e.html
- Licence: https://github.com/Velli20/Li-Ion-Battery-Test-Bench/blob/master/Middlewares/Third_Party/FatFs/src/FATFS_LICENCE

#### FreeRTOS – RTOS Kernel
- https://www.freertos.org/
- Licence: https://github.com/Velli20/Li-Ion-Battery-Test-Bench/blob/master/Middlewares/Third_Party/FreeRTOS/License/license.txt

#### STM32 HAL Driver
- Licence: https://github.com/Velli20/Li-Ion-Battery-Test-Bench/blob/master/Drivers/STM32F7xx_HAL_Driver/STM32_HAL_LICENCE

#### Linear Technology – LTC6811-1 Multicell Battery Monitor
- http://www.linear.com/product/LTC6811-1
- Licence: https://github.com/Velli20/Li-Ion-Battery-Test-Bench/blob/master/LTC6811/LTC6811_LICENCE
