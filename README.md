

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
