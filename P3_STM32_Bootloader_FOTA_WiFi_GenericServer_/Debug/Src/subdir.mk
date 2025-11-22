################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/bsp.c \
../Src/circular_buffer.c \
../Src/esp82xx_driver.c \
../Src/esp82xx_lib.c \
../Src/flash_driver.c \
../Src/fota_processor.c \
../Src/fpu.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timebase.c 

OBJS += \
./Src/adc.o \
./Src/bsp.o \
./Src/circular_buffer.o \
./Src/esp82xx_driver.o \
./Src/esp82xx_lib.o \
./Src/flash_driver.o \
./Src/fota_processor.o \
./Src/fpu.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timebase.o 

C_DEPS += \
./Src/adc.d \
./Src/bsp.d \
./Src/circular_buffer.d \
./Src/esp82xx_driver.d \
./Src/esp82xx_lib.d \
./Src/flash_driver.d \
./Src/fota_processor.d \
./Src/fpu.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timebase.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xC -c -I../Inc -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/9. FOTA_Wifi/P3_STM32_Bootloader_FOTA_WiFi_GenericServer/F4_chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/9. FOTA_Wifi/P3_STM32_Bootloader_FOTA_WiFi_GenericServer/F4_chip_headers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc.cyclo ./Src/adc.d ./Src/adc.o ./Src/adc.su ./Src/bsp.cyclo ./Src/bsp.d ./Src/bsp.o ./Src/bsp.su ./Src/circular_buffer.cyclo ./Src/circular_buffer.d ./Src/circular_buffer.o ./Src/circular_buffer.su ./Src/esp82xx_driver.cyclo ./Src/esp82xx_driver.d ./Src/esp82xx_driver.o ./Src/esp82xx_driver.su ./Src/esp82xx_lib.cyclo ./Src/esp82xx_lib.d ./Src/esp82xx_lib.o ./Src/esp82xx_lib.su ./Src/flash_driver.cyclo ./Src/flash_driver.d ./Src/flash_driver.o ./Src/flash_driver.su ./Src/fota_processor.cyclo ./Src/fota_processor.d ./Src/fota_processor.o ./Src/fota_processor.su ./Src/fpu.cyclo ./Src/fpu.d ./Src/fpu.o ./Src/fpu.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timebase.cyclo ./Src/timebase.d ./Src/timebase.o ./Src/timebase.su

.PHONY: clean-Src

