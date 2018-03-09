
ifeq ($(shell echo $$OS),$$OS)
    MAKEDIR = if not exist "$(1)" mkdir "$(1)"
    RM = rmdir /S /Q "$(1)"
else
    MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM = '$(SHELL)' -c "rm -rf \"$(1)\""
endif

OBJDIR := BUILD

ifeq (,$(filter $(OBJDIR),$(notdir $(CURDIR))))
.SUFFIXES:
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKETARGET = '$(MAKE)' --no-print-directory -C $(OBJDIR) -f '$(mkfile_path)' \
		'SRCDIR=$(CURDIR)' $(MAKECMDGOALS)
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

VPATH = ..
PROJECT := Li-Ion-Battery-Test-Bench

OBJECTS += Drivers/BSP/Components/ft5336/ft5336.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_qspi.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sd.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.o
OBJECTS += Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cec.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dfsdm.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2s.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_iwdg.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_lptim.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_mdios.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_mmc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_nand.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_nor.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_smbus.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sram.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_usart.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_wwdg.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma2d.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_exti.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_gpio.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_i2c.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_lptim.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_pwr.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rcc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rtc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_spi.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_tim.o
OBJECTS += Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usart.o
OBJECTS += LTC6811/LTC6811.o
OBJECTS += LTC681x/LTC681x.o
OBJECTS += LTC681x/bms_hardware.o
OBJECTS += Middlewares/Third_Party/FatFs/src/diskio.o
OBJECTS += Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.o
OBJECTS += Middlewares/Third_Party/FatFs/src/ff.o
OBJECTS += Middlewares/Third_Party/FatFs/src/ff_gen_drv.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/croutine.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/event_groups.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/list.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/queue.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/tasks.o
OBJECTS += Middlewares/Third_Party/FreeRTOS/Source/timers.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/api_lib.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/api_msg.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/err.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/netbuf.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/netdb.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/netifapi.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/sockets.o
OBJECTS += Middlewares/Third_Party/LwIP/src/api/tcpip.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/httpd/httpd.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/lwiperf/lwiperf.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/mdns/mdns.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/netbiosns/netbiosns.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/sntp/sntp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/apps/tftp/tftp_server.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/def.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/dns.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/inet_chksum.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/init.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ip.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/mem.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/memp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/netif.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/pbuf.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/raw.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/stats.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/sys.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/tcp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/tcp_in.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/tcp_out.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/timeouts.o
OBJECTS += Middlewares/Third_Party/LwIP/src/core/udp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ethernet.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/lowpan6.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/auth.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/ccp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/chap-md5.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/chap-new.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/chap_ms.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/demand.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/eap.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/ecp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/eui64.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/fsm.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/ipcp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/ipv6cp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/lcp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/magic.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/mppe.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/multilink.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/des.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md4.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md5.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/sha1.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/ppp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/pppapi.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/pppcrypt.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/pppoe.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/pppol2tp.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/pppos.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/upap.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/utils.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/ppp/vj.o
OBJECTS += Middlewares/Third_Party/LwIP/src/netif/slipif.o
OBJECTS += Middlewares/Third_Party/LwIP/system/OS/sys_arch.o
OBJECTS += Src/bms_main.o
OBJECTS += Src/dhcp_connect_thread.o
OBJECTS += Src/ethernetif.o
OBJECTS += Src/log.o
OBJECTS += Src/main.o
OBJECTS += Src/main_thread.o
OBJECTS += Src/server.o
OBJECTS += Src/server_write_bms_json_data.o
OBJECTS += Src/server_write_cycle_result_text_file.o
OBJECTS += Src/server_write_file_list_json_data.o
OBJECTS += Src/stm32f7xx_hal_msp.o
OBJECTS += Src/stm32f7xx_hal_timebase_tim.o
OBJECTS += Src/stm32f7xx_it.o
OBJECTS += Src/stm32f7xx_rtc.o
OBJECTS += Src/system_stm32f7xx.o
OBJECTS += Src/ui.o
OBJECTS += Src/ui_widget_list.o
OBJECTS += startup_stm32f746xx.o


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

LIBRARY_PATHS :=
LIBRARIES :=
LINKER_SCRIPT ?= ../arm-gcc-link.ld

AS      = 'arm-none-eabi-gcc' '-x' 'assembler-with-cpp' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-g1' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'
CC      = 'arm-none-eabi-gcc' '-std=gnu99' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-g1' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'
CPP     = 'arm-none-eabi-g++' '-std=gnu++98' '-fno-rtti' '-Wvla' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-g1' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'
LD      = 'arm-none-eabi-gcc'
ELF2BIN = 'arm-none-eabi-objcopy'
PREPROC = 'arm-none-eabi-cpp' '-E' '-P' '-Wl,--gc-sections' '-Wl,--wrap,main' '-Wl,--wrap,_malloc_r' '-Wl,--wrap,_free_r' '-Wl,--wrap,_realloc_r' '-Wl,--wrap,_memalign_r' '-Wl,--wrap,_calloc_r' '-Wl,--wrap,exit' '-Wl,--wrap,atexit' '-Wl,-n' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'

C_FLAGS += -std=gnu99
C_FLAGS += -include
C_FLAGS += config.h

LD_FLAGS :=-Wl,--gc-sections -Wl, -n -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp 
LD_SYS_LIBS :=-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group

.PHONY: all lst size

all: $(PROJECT).bin $(PROJECT)-combined.hex size

.s.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
  
	@$(AS) -c $(ASM_FLAGS) -o $@ $<
  
.S.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
  
	@$(AS) -c $(ASM_FLAGS) -o $@ $<

.c.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CPP) $(CXX_FLAGS) $(INCLUDE_PATHS) -o $@ $<


$(PROJECT).link_script.ld: $(LINKER_SCRIPT)
	@$(PREPROC) $< -o $@


$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS) $(PROJECT).link_script.ld 
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(filter-out %.o, $^) $(LIBRARY_PATHS) --output $@ $(filter %.o, $^) $(LIBRARIES) $(LD_SYS_LIBS)

$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	$(ELF2BIN) -O ihex $< $@

$(PROJECT)-combined.hex: $(PROJECT).hex

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)
endif
