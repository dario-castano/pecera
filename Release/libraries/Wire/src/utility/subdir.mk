################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/dario/bin/eclipse/embedcpp-2022-032/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.5/libraries/Wire/src/utility/twi.c 

C_DEPS += \
./libraries/Wire/src/utility/twi.c.d 

LINK_OBJ += \
./libraries/Wire/src/utility/twi.c.o 


# Each subdirectory must supply rules for building sources it contributes
libraries/Wire/src/utility/twi.c.o: /home/dario/bin/eclipse/embedcpp-2022-032/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.5/libraries/Wire/src/utility/twi.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"/home/dario/bin/eclipse/embedcpp-2022-032/eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-gcc" -c -g -Os -w -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"/home/dario/bin/eclipse/embedcpp-2022-032/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.5/variants/standard" -I"/home/dario/bin/eclipse/embedcpp-2022-032/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.5/cores/arduino" -I"/home/dario/bin/eclipse/embedcpp-2022-032/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.5/libraries/Wire/src" -I"/home/dario/bin/eclipse/embedcpp-2022-032/eclipse/arduinoPlugin/libraries/LiquidCrystal/1.0.7/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


