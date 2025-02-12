################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Core/Libs/comm_uart.c \
../Application/Core/Libs/communication_ex.c \
../Application/Core/Libs/hw_spi.c \
../Application/Core/Libs/queue.c \
../Application/Core/Libs/serial_fun.c 

OBJS += \
./Application/Core/Libs/comm_uart.o \
./Application/Core/Libs/communication_ex.o \
./Application/Core/Libs/hw_spi.o \
./Application/Core/Libs/queue.o \
./Application/Core/Libs/serial_fun.o 

C_DEPS += \
./Application/Core/Libs/comm_uart.d \
./Application/Core/Libs/communication_ex.d \
./Application/Core/Libs/hw_spi.d \
./Application/Core/Libs/queue.d \
./Application/Core/Libs/serial_fun.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Core/Libs/%.o Application/Core/Libs/%.su Application/Core/Libs/%.cyclo: ../Application/Core/Libs/%.c Application/Core/Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32WBXX_NUCLEO -DENABLE_USART -DSTM32WB55xx -DCLIENT -c -I../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I"F:/Nejc/Projekti/BLEMeshHomeAutomation/BLE_MeshLightingProvisioner/STM32CubeIDE/Application/Core/Libs" -I../../Drivers/CMSIS/Include -I../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -I../../Middlewares/ST/STM32_WPAN/ble/mesh/Inc -I../../Core/Inc -I../../Middlewares/ST/STM32_WPAN -I../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../Middlewares/ST/STM32_WPAN/utilities -I../../Utilities/lpm/tiny_lpm -I../../Utilities/sequencer -I../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../Middlewares/ST/STM32_WPAN/ble/core -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../Middlewares/ST/STM32_WPAN/ble/mesh/MeshModel/Inc -I../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../STM32_WPAN/app -I../../Middlewares/ST/STM32_WPAN/ble -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Core-2f-Libs

clean-Application-2f-Core-2f-Libs:
	-$(RM) ./Application/Core/Libs/comm_uart.cyclo ./Application/Core/Libs/comm_uart.d ./Application/Core/Libs/comm_uart.o ./Application/Core/Libs/comm_uart.su ./Application/Core/Libs/communication_ex.cyclo ./Application/Core/Libs/communication_ex.d ./Application/Core/Libs/communication_ex.o ./Application/Core/Libs/communication_ex.su ./Application/Core/Libs/hw_spi.cyclo ./Application/Core/Libs/hw_spi.d ./Application/Core/Libs/hw_spi.o ./Application/Core/Libs/hw_spi.su ./Application/Core/Libs/queue.cyclo ./Application/Core/Libs/queue.d ./Application/Core/Libs/queue.o ./Application/Core/Libs/queue.su ./Application/Core/Libs/serial_fun.cyclo ./Application/Core/Libs/serial_fun.d ./Application/Core/Libs/serial_fun.o ./Application/Core/Libs/serial_fun.su

.PHONY: clean-Application-2f-Core-2f-Libs

