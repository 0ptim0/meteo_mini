################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/BME280.c \
../Src/SSD1306.c \
../Src/clock.c \
../Src/font.c \
../Src/main.c 

OBJS += \
./Src/BME280.o \
./Src/SSD1306.o \
./Src/clock.o \
./Src/font.o \
./Src/main.o 

C_DEPS += \
./Src/BME280.d \
./Src/SSD1306.d \
./Src/clock.d \
./Src/font.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/BME280.o: ../Src/BME280.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DDEBUG -c -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/CMSIS/inc" -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/BME280.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/SSD1306.o: ../Src/SSD1306.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DDEBUG -c -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/CMSIS/inc" -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/SSD1306.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/clock.o: ../Src/clock.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DDEBUG -c -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/CMSIS/inc" -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/clock.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/font.o: ../Src/font.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DDEBUG -c -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/CMSIS/inc" -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/font.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DDEBUG -c -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/CMSIS/inc" -I"C:/Users/Mojo Risin'/Documents/cube/projects/test/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

