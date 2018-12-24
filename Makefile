
ifeq ($(shell echo $$OS),$$OS)
    MAKEDIR = if not exist "$(1)" mkdir "$(1)"
    RM      = rmdir /S /Q "$(1)"
else
    MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM      = '$(SHELL)' -c "rm -rf \"$(1)\""
endif

OBJDIR := BUILD

ifeq (,$(filter $(OBJDIR),$(notdir $(CURDIR))))
.SUFFIXES:
mkfile_path :=  $(abspath $(lastword $(MAKEFILE_LIST)))
MAKETARGET   = '$(MAKE)' --no-print-directory -C $(OBJDIR) -f '$(mkfile_path)' 'SRCDIR=$(CURDIR)' $(MAKECMDGOALS)

.PHONY: $(OBJDIR) clean
all:
	+@$(call MAKEDIR,$(OBJDIR))
	+@$(MAKETARGET)
$(OBJDIR): all
Makefile : ;
% :: $(OBJDIR) ; :
clean :
	$(call RM,$(OBJDIR))

else

AS      = 'arm-none-eabi-gcc'
CC      = 'arm-none-eabi-gcc'
LD      = 'arm-none-eabi-gcc'
ELF2BIN = 'arm-none-eabi-objcopy'

LINKER_SCRIPT := ../arm-gcc-link.ld
PROJECT       := Li-Ion-Battery-Test-Bench

OBJECTS += Drivers/BSP/Components/ft5336/ft5336.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_qspi.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sd.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.o
OBJECTS += LTC6811/LTC6811.o
OBJECTS += LTC681x/LTC681x.o
OBJECTS += LTC681x/bms_hardware.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/httpd/httpd.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/lwiperf/lwiperf.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/mdns/mdns.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/netbiosns/netbiosns.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/sntp/sntp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/tftp/tftp_server.o

OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ethernet.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/lowpan6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/slipif.o
OBJECTS += Middlewares/Third_Party/LwIP/system/OS/sys_arch.o
OBJECTS += startup_stm32f746xx.o

SOURCES += $(wildcard ../Middlewares/Third_Party/FatFs/src/*.c)
SOURCES += $(wildcard ../Middlewares/Third_Party/FatFs/src/drivers/*.c)
SOURCES += $(wildcard ../Middlewares/Third_Party/FreeRTOS/Source/*.c)
SOURCES += $(wildcard ../Middlewares/Third_Party/LwIP/src/core/ipv4/*.c)
SOURCES += $(wildcard ../Middlewares/Third_Party/LwIP/src/core/ipv6/*.c)
SOURCES += $(wildcard ../Middlewares/Third_Party/LwIP/src/core/*.c)
SOURCES += $(wildcard ../Middlewares/Third_Party/LwIP/src/api/*.c)

SOURCES += $(wildcard ../Drivers/STM32F7xx_HAL_Driver/Src/*.c)
SOURCES += $(wildcard ../Src/*.c)

OBJECTS += $(SOURCES:.c=.o)

VPATH = ..

INCLUDE_PATHS += -I../
INCLUDE_PATHS += -I../.
INCLUDE_PATHS += -I../Drivers
INCLUDE_PATHS += -I../Drivers/BSP/Components
INCLUDE_PATHS += -I../Drivers/BSP/Components/Common
INCLUDE_PATHS += -I../Drivers/BSP/STM32746G-Discovery
INCLUDE_PATHS += -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCLUDE_PATHS += -I../Drivers/CMSIS/Include
INCLUDE_PATHS += -I../Drivers/STM32F7xx_HAL_Driver
INCLUDE_PATHS += -I../Drivers/STM32F7xx_HAL_Driver/Inc
INCLUDE_PATHS += -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
INCLUDE_PATHS += -I../Drivers/STM32F7xx_HAL_Driver/Src
INCLUDE_PATHS += -I../Inc
INCLUDE_PATHS += -I../LTC6811
INCLUDE_PATHS += -I../LTC681x
INCLUDE_PATHS += -I../Middlewares/Third_Party/FatFs/src
INCLUDE_PATHS += -I../Middlewares/Third_Party/FatFs/src/drivers
INCLUDE_PATHS += -I../Middlewares/Third_Party/FreeRTOS/Source
INCLUDE_PATHS += -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
INCLUDE_PATHS += -I../Middlewares/Third_Party/FreeRTOS/Source/include
INCLUDE_PATHS += -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
INCLUDE_PATHS += -I../Middlewares/Third_Party/LwIP/src
INCLUDE_PATHS += -I../Middlewares/Third_Party/LwIP/src/apps/httpd
INCLUDE_PATHS += -I../Middlewares/Third_Party/LwIP/src/include
INCLUDE_PATHS += -I../Middlewares/Third_Party/LwIP/system
INCLUDE_PATHS += -I../Src
INCLUDE_PATHS += -I../Utilities


C_FLAGS     := -std=gnu99 -c -O3 -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp
C_FLAGS     += -include config.h
 
LD_FLAGS    :=-Wl,--gc-sections -Wl, -n -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp 
LD_SYS_LIBS :=-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group

.PHONY: all lst

all: $(PROJECT).bin $(PROJECT)-combined.hex

.s.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) -o $@ $<

.c.o:
	+@$(call MAKEDIR ,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INCLUDE_PATHS) -o $@ $<

$(PROJECT).elf: $(OBJECTS) 
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(LINKER_SCRIPT) $(LIBRARY_PATHS) --output $@ $(filter %.o, $^) $(LIBRARIES) $(LD_SYS_LIBS)

$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	$(ELF2BIN) -O ihex $< $@

$(PROJECT)-combined.hex: $(PROJECT).hex

DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)
endif
