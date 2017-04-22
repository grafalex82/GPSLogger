# Not pushing this file down to libraries in order to keep source tree as is (not populating with extra files, such as CMakeList.txt)
#
# Below each section represents a library with its own settings



###################
# NeoGPS
###################

SET(NEOGPS_SRC
	NeoGPS/DMS.cpp
	NeoGPS/GPSTime.cpp
	NeoGPS/Location.cpp
	NeoGPS/NeoTime.cpp
	NeoGPS/NMEAGPS.cpp
)
ADD_LIBRARY(NeoGPS STATIC ${NEOGPS_SRC})



###################
# FreeRTOS
###################

SET(FREERTOS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STM32duino/libraries/FreeRTOS821)

SET(FREERTOS_SRC
	${FREERTOS_SRC_DIR}/MapleFreeRTOS821.cpp
	${FREERTOS_SRC_DIR}/utility/heap_1.c
	${FREERTOS_SRC_DIR}/utility/list.c
	${FREERTOS_SRC_DIR}/utility/port.c
	${FREERTOS_SRC_DIR}/utility/queue.c
	${FREERTOS_SRC_DIR}/utility/tasks.c
)
ADD_LIBRARY(FreeRTOS STATIC ${FREERTOS_SRC})



###################
# Adafruit GFX library
###################

ADD_LIBRARY(AdafruitGFX STATIC 
	AdafruitGFX/Adafruit_GFX.cpp 
)



###################
# Adafruit SSD1306 library
###################

ADD_LIBRARY(AdafruitSSD1306 STATIC 
	STM32duino/libraries/Adafruit_SSD1306/Adafruit_SSD1306_STM32.cpp
)
TARGET_INCLUDE_DIRECTORIES(AdafruitSSD1306 PRIVATE
	STM32duino/libraries/Wire
	STM32duino/libraries/SPI/src  	#In fact it should not depend on it
	AdafruitGFX
	
)


###################
# Various Arduino libraries
###################

SET(ARDUINO_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STM32duino/libraries)

SET(ARDUINO_LIBS_SRC
	${ARDUINO_LIBS_DIR}/Wire/HardWire.cpp
	${ARDUINO_LIBS_DIR}/Wire/WireBase.cpp
)

ADD_LIBRARY(ArduinoLibs STATIC ${ARDUINO_LIBS_SRC})


###################
# STM32duino, libmaple and system layer
# Has to be as a single library, otherwise linker does not resolve all crossreferences
# Unused files are commented on the list
###################

SET(LIBMAPLE_SRC
	STM32duino/variants/generic_stm32f103c/board.cpp
	STM32duino/variants/generic_stm32f103c/wirish/boards.cpp
	STM32duino/variants/generic_stm32f103c/wirish/boards_setup.cpp
	STM32duino/variants/generic_stm32f103c/wirish/start.S
	STM32duino/variants/generic_stm32f103c/wirish/start_c.c
 	STM32duino/variants/generic_stm32f103c/wirish/syscalls.c

	STM32duino/cores/maple/cxxabi-compat.cpp
#	STM32duino/cores/maple/ext_interrupts.cpp
	STM32duino/cores/maple/HardwareSerial.cpp
#	STM32duino/cores/maple/HardwareTimer.cpp
#	STM32duino/cores/maple/IPAddress.cpp
	STM32duino/cores/maple/main.cpp
#	STM32duino/cores/maple/new.cpp
	STM32duino/cores/maple/Print.cpp
#	STM32duino/cores/maple/pwm.cpp
#	STM32duino/cores/maple/Stream.cpp
#	STM32duino/cores/maple/tone.cpp
	STM32duino/cores/maple/usb_serial.cpp
#	STM32duino/cores/maple/wirish_analog.cpp
	STM32duino/cores/maple/wirish_digital.cpp
#	STM32duino/cores/maple/wirish_math.cpp
#	STM32duino/cores/maple/wirish_shift.cpp
	STM32duino/cores/maple/wirish_time.cpp
#	STM32duino/cores/maple/WString.cpp

#	STM32duino/cores/maple/hooks.c
	STM32duino/cores/maple/itoa.c

#	STM32duino/cores/maple/stm32f1/util_hooks.c
#	STM32duino/cores/maple/stm32f1/wiring_pulse_f1.cpp
	STM32duino/cores/maple/stm32f1/wirish_debug.cpp
	STM32duino/cores/maple/stm32f1/wirish_digital_f1.cpp

	STM32duino/cores/maple/libmaple/adc.c
	STM32duino/cores/maple/libmaple/adc_f1.c
#	STM32duino/cores/maple/libmaple/bkp_f1.c
#	STM32duino/cores/maple/libmaple/dac.c
#	STM32duino/cores/maple/libmaple/dma.c
#	STM32duino/cores/maple/libmaple/dma_f1.c
#	STM32duino/cores/maple/libmaple/exc.S
#	STM32duino/cores/maple/libmaple/exti.c
#	STM32duino/cores/maple/libmaple/exti_f1.c
	STM32duino/cores/maple/libmaple/flash.c
#	STM32duino/cores/maple/libmaple/fsmc_f1.c
	STM32duino/cores/maple/libmaple/gpio.c
	STM32duino/cores/maple/libmaple/gpio_f1.c
	STM32duino/cores/maple/libmaple/i2c.c
	STM32duino/cores/maple/libmaple/i2c_f1.c
	STM32duino/cores/maple/libmaple/iwdg.c
	STM32duino/cores/maple/libmaple/nvic.c
#	STM32duino/cores/maple/libmaple/pwr.c
	STM32duino/cores/maple/libmaple/rcc.c
	STM32duino/cores/maple/libmaple/rcc_f1.c
#	STM32duino/cores/maple/libmaple/spi.c
#	STM32duino/cores/maple/libmaple/spi_f1.c
	STM32duino/cores/maple/libmaple/systick.c
	STM32duino/cores/maple/libmaple/timer.c
#	STM32duino/cores/maple/libmaple/timer_f1.c
	STM32duino/cores/maple/libmaple/usart.c
	STM32duino/cores/maple/libmaple/usart_f1.c
	STM32duino/cores/maple/libmaple/usart_private.c
	STM32duino/cores/maple/libmaple/util.c

	STM32duino/cores/maple/libmaple/stm32f1/performance/isrs.S
	STM32duino/cores/maple/libmaple/stm32f1/performance/vector_table.S

	STM32duino/cores/maple/libmaple/usb/stm32f1/usb.c
	STM32duino/cores/maple/libmaple/usb/stm32f1/usb_cdcacm.c
	STM32duino/cores/maple/libmaple/usb/stm32f1/usb_reg_map.c

	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_core.c
	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_init.c
	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_mem.c
	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_regs.c
)

ADD_LIBRARY(STM32duino STATIC ${LIBMAPLE_SRC})

TARGET_INCLUDE_DIRECTORIES(STM32duino PRIVATE
	STM32duino/system/libmaple/usb/stm32f1
	STM32duino/system/libmaple/usb/usb_lib
)

TARGET_COMPILE_DEFINITIONS(STM32duino PRIVATE 
	-DVECT_TAB_ADDR=${VECT_TAB_ADDR} 
	-DGENERIC_BOOTLOADER 
	-DBOARD_maple
)