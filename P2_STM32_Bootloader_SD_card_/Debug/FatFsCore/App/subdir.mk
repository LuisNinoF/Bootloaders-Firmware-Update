################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FatFsCore/App/fatfs_app.c 

OBJS += \
./FatFsCore/App/fatfs_app.o 

C_DEPS += \
./FatFsCore/App/fatfs_app.d 


# Each subdirectory must supply rules for building sources it contributes
FatFsCore/App/%.o FatFsCore/App/%.su FatFsCore/App/%.cyclo: ../FatFsCore/App/%.c FatFsCore/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xC -c -I../Inc -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/F4_chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/F4_chip_headers/CMSIS/Include" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/FatFsCore/App" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/Middlewares/Third_Party/FatFs/src" -I"C:/Users/lfnin/OneDrive/Documents/Studies/Embedded Systems/8. Embedded_Expert_IO/6. Phase2/8. SD_Card/P2_STM32_Bootloader_SD_card/FatFsCore/Target" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FatFsCore-2f-App

clean-FatFsCore-2f-App:
	-$(RM) ./FatFsCore/App/fatfs_app.cyclo ./FatFsCore/App/fatfs_app.d ./FatFsCore/App/fatfs_app.o ./FatFsCore/App/fatfs_app.su

.PHONY: clean-FatFsCore-2f-App

