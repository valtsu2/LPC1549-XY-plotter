################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CommandHandler.cpp \
../src/Debug.cpp \
../src/DigitalIoPin.cpp \
../src/ITM_output.cpp \
../src/Motor.cpp \
../src/MotorUser.cpp \
../src/PWMcontroller.cpp \
../src/Parser.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc15xx.cpp \
../src/main.cpp 

C_SRCS += \
../src/ITM_write.c \
../src/cdc_desc.c \
../src/cdc_main.c \
../src/cdc_vcom.c \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/CommandHandler.o \
./src/Debug.o \
./src/DigitalIoPin.o \
./src/ITM_output.o \
./src/ITM_write.o \
./src/Motor.o \
./src/MotorUser.o \
./src/PWMcontroller.o \
./src/Parser.o \
./src/cdc_desc.o \
./src/cdc_main.o \
./src/cdc_vcom.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc15xx.o \
./src/crp.o \
./src/main.o \
./src/sysinit.o 

CPP_DEPS += \
./src/CommandHandler.d \
./src/Debug.d \
./src/DigitalIoPin.d \
./src/ITM_output.d \
./src/Motor.d \
./src/MotorUser.d \
./src/PWMcontroller.d \
./src/Parser.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc15xx.d \
./src/main.d 

C_DEPS += \
./src/ITM_write.d \
./src/cdc_desc.d \
./src/cdc_main.d \
./src/cdc_vcom.d \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=c++11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_chip_15xx\inc" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\FreeRTOS\inc" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\FreeRTOS\src\include" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\FreeRTOS\src\portable\GCC\ARM_CM3" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_chip_15xx\inc\usbd" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\DrawMachine\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_chip_15xx\inc" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\FreeRTOS\inc" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\FreeRTOS\src\include" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\FreeRTOS\src\portable\GCC\ARM_CM3" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_chip_15xx\inc\usbd" -I"C:\Users\Valtsu\Documents\MCUXpressoIDE_11.0.0_2516\workspace\DrawMachine\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


