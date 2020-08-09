################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/src/system_stm32f4xx.c 

OBJS += \
./CMSIS/src/system_stm32f4xx.o 

C_DEPS += \
./CMSIS/src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/src/system_stm32f4xx.o: ../CMSIS/src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DDEBUG -c -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/CMSIS/inc" -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"CMSIS/src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

