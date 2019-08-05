
# Project name and build direcotory name

OBJDIR           := BUILD
PROJECT          := Li-Ion-Battery-Test-Bench
COM_PORT         := COM7

# STM32 Cube Programmer

CUBE_PROGRAMMER  := 'STM32_Programmer_CLI.exe'
EXT_FLASH_LOADER := 'N25Q128A_STM32F746G-DISCO.stldr'

# Use GCC

AS               := 'arm-none-eabi-gcc'
CC               := 'arm-none-eabi-gcc'
LD               := 'arm-none-eabi-gcc'
ELF2BIN          := 'arm-none-eabi-objcopy'

# Linker script

LINKER_SCRIPT    := arm-gcc-link.ld

# Compiler flags

C_FLAGS          := -std=gnu99 -c -O3 -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp
C_FLAGS          += -include config.h

# Linker flags

LD_FLAGS         :=-Wl,--gc-sections -Wl, -n -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp
LD_SYS_LIBS      :=-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group

# Source files

SOURCE_FILES += Drivers/BSP/Components/ft5336/ft5336.c
SOURCE_FILES += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c
SOURCE_FILES += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c
SOURCE_FILES += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_qspi.c
SOURCE_FILES += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sd.c
SOURCE_FILES += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c
SOURCE_FILES += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.c
SOURCE_FILES += LTC6811/LTC6811.c
SOURCE_FILES += LTC681x/LTC681x.c
SOURCE_FILES += LTC681x/bms_hardware.c
SOURCE_FILES += Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
SOURCE_FILES += Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
SOURCE_FILES += Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/apps/httpd/httpd.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/apps/lwiperf/lwiperf.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/apps/mdns/mdns.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/apps/netbiosns/netbiosns.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/apps/sntp/sntp.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/apps/tftp/tftp_server.c

SOURCE_FILES += Middlewares/Third_Party/LwIP/src/netif/ethernet.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/netif/lowpan6.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/src/netif/slipif.c
SOURCE_FILES += Middlewares/Third_Party/LwIP/system/OS/sys_arch.c
SOURCE_FILES += startup_stm32f746xx.c

SOURCE_FILES += $(wildcard Middlewares/Third_Party/FatFs/src/*.c)
SOURCE_FILES += $(wildcard Middlewares/Third_Party/FatFs/src/drivers/*.c)
SOURCE_FILES += $(wildcard Middlewares/Third_Party/FreeRTOS/Source/*.c)
SOURCE_FILES += $(wildcard Middlewares/Third_Party/LwIP/src/core/ipv4/*.c)
SOURCE_FILES += $(wildcard Middlewares/Third_Party/LwIP/src/core/ipv6/*.c)
SOURCE_FILES += $(wildcard Middlewares/Third_Party/LwIP/src/core/*.c)
SOURCE_FILES += $(wildcard Middlewares/Third_Party/LwIP/src/api/*.c)
SOURCE_FILES += $(wildcard Drivers/STM32F7xx_HAL_Driver/Src/*.c)
SOURCE_FILES += $(wildcard Src/*.c)

OBJECT_FILES := $(patsubst %.o, $(OBJDIR)/%.o, $(SOURCE_FILES:.c=.o))

# Include directories

INCLUDE_DIRS += -I./Drivers
INCLUDE_DIRS += -I./Drivers/BSP/Components
INCLUDE_DIRS += -I./Drivers/BSP/Components/Common
INCLUDE_DIRS += -I./Drivers/BSP/STM32746G-Discovery
INCLUDE_DIRS += -I./Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCLUDE_DIRS += -I./Drivers/CMSIS/Include
INCLUDE_DIRS += -I./Drivers/STM32F7xx_HAL_Driver
INCLUDE_DIRS += -I./Drivers/STM32F7xx_HAL_Driver/Inc
INCLUDE_DIRS += -I./Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
INCLUDE_DIRS += -I./Drivers/STM32F7xx_HAL_Driver/Src
INCLUDE_DIRS += -I./Inc
INCLUDE_DIRS += -I./LTC6811
INCLUDE_DIRS += -I./LTC681x
INCLUDE_DIRS += -I./Middlewares/Third_Party/FatFs/src
INCLUDE_DIRS += -I./Middlewares/Third_Party/FatFs/src/drivers
INCLUDE_DIRS += -I./Middlewares/Third_Party/FreeRTOS/Source
INCLUDE_DIRS += -I./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
INCLUDE_DIRS += -I./Middlewares/Third_Party/FreeRTOS/Source/include
INCLUDE_DIRS += -I./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
INCLUDE_DIRS += -I./Middlewares/Third_Party/LwIP/src
INCLUDE_DIRS += -I./Middlewares/Third_Party/LwIP/src/apps/httpd
INCLUDE_DIRS += -I./Middlewares/Third_Party/LwIP/src/include
INCLUDE_DIRS += -I./Middlewares/Third_Party/LwIP/system
INCLUDE_DIRS += -I./Src
INCLUDE_DIRS += -I./Utilities

.PHONY: all lst

clean:
	@rm -rf $(OBJDIR)

flash:
	$(CUBE_PROGRAMMER) -c port=SWD reset=HWrst -w $(PROJECT).hex -rst

flash-ext:
	$(CUBE_PROGRAMMER) -c port=SWD reset=HWrst -el $(EXT_FLASH_LOADER) -w $(PROJECT).hex -rst

log:
	$(CUBE_PROGRAMMER) -c port=COM7 br=57600 P=NONE sb=1 console

all: $(PROJECT).bin $(PROJECT).hex

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	+@echo 'Compile $<'
	+@mkdir -p $(@D)
	@$(CC) $(C_FLAGS) $(INCLUDE_DIRS) -o $@ $<

$(OBJDIR)/%.o: %.s | $(OBJDIR)
	+@echo 'Assemble $<'
	+@mkdir -p $(@D)
	@$(AS) -c $(ASM_FLAGS) -o $@ $<

$(OBJDIR):
	+@mkdir $@

$(PROJECT).elf: $(OBJECT_FILES)
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(LINKER_SCRIPT) $(LIBRARY_PATHS) --output $@ $(OBJECT_FILES) $(LIBRARIES) $(LD_SYS_LIBS)

$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	$(ELF2BIN) -O ihex $< $@

DEPS = $(OBJECT_FILES:.o=.d)
-include $(DEPS)
