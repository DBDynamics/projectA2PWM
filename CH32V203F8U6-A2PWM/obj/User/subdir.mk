################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/adc.c \
../User/ch32v20x_it.c \
../User/led.c \
../User/main.c \
../User/pwm.c \
../User/system_ch32v20x.c \
../User/timer.c 

OBJS += \
./User/adc.o \
./User/ch32v20x_it.o \
./User/led.o \
./User/main.o \
./User/pwm.o \
./User/system_ch32v20x.o \
./User/timer.o 

C_DEPS += \
./User/adc.d \
./User/ch32v20x_it.d \
./User/led.d \
./User/main.d \
./User/pwm.d \
./User/system_ch32v20x.d \
./User/timer.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\Thinkpad\Documents\GitHub\projectA2PWM\CH32V203F8U6-A2PWM\Debug" -I"C:\Users\Thinkpad\Documents\GitHub\projectA2PWM\CH32V203F8U6-A2PWM\Core" -I"C:\Users\Thinkpad\Documents\GitHub\projectA2PWM\CH32V203F8U6-A2PWM\User" -I"C:\Users\Thinkpad\Documents\GitHub\projectA2PWM\CH32V203F8U6-A2PWM\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

