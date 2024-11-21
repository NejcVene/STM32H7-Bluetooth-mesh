################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/libs/comm_uart.c \
../Application/User/libs/command.c \
../Application/User/libs/communication_ex.c \
../Application/User/libs/fsm_queue.c \
../Application/User/libs/hash_table.c \
../Application/User/libs/node_config.c \
../Application/User/libs/sensors.c 

C_DEPS += \
./Application/User/libs/comm_uart.d \
./Application/User/libs/command.d \
./Application/User/libs/communication_ex.d \
./Application/User/libs/fsm_queue.d \
./Application/User/libs/hash_table.d \
./Application/User/libs/node_config.d \
./Application/User/libs/sensors.d 

OBJS += \
./Application/User/libs/comm_uart.o \
./Application/User/libs/command.o \
./Application/User/libs/communication_ex.o \
./Application/User/libs/fsm_queue.o \
./Application/User/libs/hash_table.o \
./Application/User/libs/node_config.o \
./Application/User/libs/sensors.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/libs/%.o Application/User/libs/%.su Application/User/libs/%.cyclo: ../Application/User/libs/%.c Application/User/libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32H750xx -DUSE_HAL_DRIVER -DUSE_BPP=16 -DVECT_TAB_QSPI -DDEBUG -c -I../../Core/Inc -I"F:/Nejc/Projekti/BLEMeshHomeAutomation/H7MeshProvisioner/STM32CubeIDE/Application/User/libs" -I../../Drivers/CMSIS/Include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/target -I../../TouchGFX/App -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/target/generated -I../../Drivers/BSP/STM32H750B-DK -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../TouchGFX/gui/include -I../../TouchGFX/generated/texts/include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../TouchGFX/generated/images/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../TouchGFX/generated/fonts/include -I../../LIBJPEG/App -I../../LIBJPEG/Target -I../../Utilities/JPEG -I../../Middlewares/Third_Party/LibJPEG/include -I../../TouchGFX/generated/videos/include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-libs

clean-Application-2f-User-2f-libs:
	-$(RM) ./Application/User/libs/comm_uart.cyclo ./Application/User/libs/comm_uart.d ./Application/User/libs/comm_uart.o ./Application/User/libs/comm_uart.su ./Application/User/libs/command.cyclo ./Application/User/libs/command.d ./Application/User/libs/command.o ./Application/User/libs/command.su ./Application/User/libs/communication_ex.cyclo ./Application/User/libs/communication_ex.d ./Application/User/libs/communication_ex.o ./Application/User/libs/communication_ex.su ./Application/User/libs/fsm_queue.cyclo ./Application/User/libs/fsm_queue.d ./Application/User/libs/fsm_queue.o ./Application/User/libs/fsm_queue.su ./Application/User/libs/hash_table.cyclo ./Application/User/libs/hash_table.d ./Application/User/libs/hash_table.o ./Application/User/libs/hash_table.su ./Application/User/libs/node_config.cyclo ./Application/User/libs/node_config.d ./Application/User/libs/node_config.o ./Application/User/libs/node_config.su ./Application/User/libs/sensors.cyclo ./Application/User/libs/sensors.d ./Application/User/libs/sensors.o ./Application/User/libs/sensors.su

.PHONY: clean-Application-2f-User-2f-libs

