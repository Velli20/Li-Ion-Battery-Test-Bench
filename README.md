

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

| STM32F746G-DISCO | DC2259A |
| ------ | ------ |
| D13 | SPI SCK |  
| D12 | SPI MISO |
| D11 | SPI MOSI |
| D10 | SPI CS |
