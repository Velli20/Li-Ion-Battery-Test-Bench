

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

# Libraries used in this project
#### lwIP – A Lightweight TCP/IP stack 
- https://savannah.nongnu.org/projects/lwip/
```sh
http://lwip.wikia.com/wiki/License
lwIP is licenced under the BSD licence: 

Copyright (c) 2001-2004 Swedish Institute of Computer Science. 
All rights reserved. 

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution. 
3. The name of the author may not be used to endorse or promote products 
derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
```
#### FatFS – Generic FAT Filesystem Module 
- http://elm-chan.org/fsw/ff/00index_e.html
```sh
/*----------------------------------------------------------------------------/
/  FatFs - Generic FAT Filesystem Module  Rx.xx                               /
/-----------------------------------------------------------------------------/
/
/ Copyright (C) 2018, ChaN, all right reserved.
/
/ FatFs module is an open source software. Redistribution and use of FatFs in
/ source and binary forms, with or without modification, are permitted provided
/ that the following condition is met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright owner or contributors be NOT LIABLE for any damages caused
/ by use of this software.
/----------------------------------------------------------------------------*/
```
#### FreeRTOS – RTOS Kernel
- https://www.freertos.org/
```sh
The FreeRTOS open source license (MIT) covers the FreeRTOS source files,
which are located in the /FreeRTOS/Source directory of the official FreeRTOS
download.  It also covers most of the source files in the demo application
projects, which are located in the /FreeRTOS/Demo directory of the official
FreeRTOS download.  The demo projects may also include third party software that
is not part of FreeRTOS and is licensed separately to FreeRTOS.  Examples of
third party software includes header files provided by chip or tools vendors,
linker scripts, peripheral drivers, etc.  All the software in subdirectories of
the /FreeRTOS directory is either open source or distributed with permission,
and is free for use.  For the avoidance of doubt, refer to the comments at the
top of each source file.

The FreeRTOS MIT license follows:

--------------------------------------------------------------------------------
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. If you wish to use  Amazon
FreeRTOS name, please do so in a fair use way that does not cause confusion.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------------
```
#### STM32 HAL Driver
```sh
COPYRIGHT(c) 2016 STMicroelectronics</center></h2>

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
   3. Neither the name of STMicroelectronics nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  ```
