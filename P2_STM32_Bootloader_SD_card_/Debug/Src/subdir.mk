################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/bsp.c \
../Src/flash_driver.c \
../Src/fpu.c \
../Src/main.c \
../Src/sd_card_driver.c \
../Src/sd_card_spi.c \
../Src/spi.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timebase.c \
../Src/uart.c 

OBJS += \
./Src/adc.o \
./Src/bsp.o \
./Src/flash_driver.o \
./Src/fpu.o \
./Src/main.o \
./Src/sd_card_driver.o \
./Src/sd_card_spi.o \
./Src/spi.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timebase.o \
./Src/uart.o 

C_DEPS += \
./Src/adc.d \
./Src/bsp.d \
./Src/flash_driver.d \
./Src/fpu.d \
./Src/main.d \
./Src/sd_card_driver.d \
./Src/sd_card_spi.d \
./Src/spi.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timebase.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xC -c -I../Inc -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/F4_chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/F4_chip_headers/CMSIS/Include" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/FatFsCore/App" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/Middlewares/Third_Party/FatFs/src" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/FatFsCore/Target" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc.cyclo ./Src/adc.d ./Src/adc.o ./Src/adc.su ./Src/bsp.cyclo ./Src/bsp.d ./Src/bsp.o ./Src/bsp.su ./Src/flash_driver.cyclo ./Src/flash_driver.d ./Src/flash_driver.o ./Src/flash_driver.su ./Src/fpu.cyclo ./Src/fpu.d ./Src/fpu.o ./Src/fpu.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/sd_card_driver.cyclo ./Src/sd_card_driver.d ./Src/sd_card_driver.o ./Src/sd_card_driver.su ./Src/sd_card_spi.cyclo ./Src/sd_card_spi.d ./Src/sd_card_spi.o ./Src/sd_card_spi.su ./Src/spi.cyclo ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timebase.cyclo ./Src/timebase.d ./Src/timebase.o ./Src/timebase.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

