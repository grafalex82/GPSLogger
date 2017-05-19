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
	STM32duino/libraries/Adafruit_SSD1306/Adafruit_SSD1306_STM32.h
)
TARGET_INCLUDE_DIRECTORIES(AdafruitSSD1306 PRIVATE
#	STM32duino/libraries/Wire
#	STM32duino/libraries/SPI/src  	#In fact it should not depend on it
	STM32generic/libraries/Wire/src
	STM32generic/libraries/SPI/src
	AdafruitGFX
	
)


###################
# Various Arduino libraries
###################

SET(ARDUINO_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STM32generic/libraries)

SET(ARDUINO_LIBS_SRC
	${ARDUINO_LIBS_DIR}/Wire/src/Wire.cpp
)

ADD_LIBRARY(ArduinoLibs STATIC ${ARDUINO_LIBS_SRC})


###################
# STM32duino, libmaple and system layer
# Has to be as a single library, otherwise linker does not resolve all crossreferences
# Unused files are commented on the list
###################

#SET(LIBMAPLE_SRC
#	STM32duino/variants/generic_stm32f103c/board.cpp
#	STM32duino/variants/generic_stm32f103c/wirish/boards.cpp
#	STM32duino/variants/generic_stm32f103c/wirish/boards_setup.cpp
#	STM32duino/variants/generic_stm32f103c/wirish/start.S
#	STM32duino/variants/generic_stm32f103c/wirish/start_c.c
# 	STM32duino/variants/generic_stm32f103c/wirish/syscalls.c
#
#	STM32duino/cores/maple/cxxabi-compat.cpp
##	STM32duino/cores/maple/ext_interrupts.cpp
#	STM32duino/cores/maple/HardwareSerial.cpp
##	STM32duino/cores/maple/HardwareTimer.cpp
##	STM32duino/cores/maple/IPAddress.cpp
#	STM32duino/cores/maple/main.cpp
##	STM32duino/cores/maple/new.cpp
#	STM32duino/cores/maple/Print.cpp
##	STM32duino/cores/maple/pwm.cpp
##	STM32duino/cores/maple/Stream.cpp
##	STM32duino/cores/maple/tone.cpp
#	STM32duino/cores/maple/usb_serial.cpp
##	STM32duino/cores/maple/wirish_analog.cpp
#	STM32duino/cores/maple/wirish_digital.cpp
##	STM32duino/cores/maple/wirish_math.cpp
##	STM32duino/cores/maple/wirish_shift.cpp
#	STM32duino/cores/maple/wirish_time.cpp
##	STM32duino/cores/maple/WString.cpp
#
##	STM32duino/cores/maple/hooks.c
#	STM32duino/cores/maple/itoa.c
#
##	STM32duino/cores/maple/stm32f1/util_hooks.c
##	STM32duino/cores/maple/stm32f1/wiring_pulse_f1.cpp
#	STM32duino/cores/maple/stm32f1/wirish_debug.cpp
#	STM32duino/cores/maple/stm32f1/wirish_digital_f1.cpp
#
#	STM32duino/cores/maple/libmaple/adc.c
#	STM32duino/cores/maple/libmaple/adc_f1.c
##	STM32duino/cores/maple/libmaple/bkp_f1.c
##	STM32duino/cores/maple/libmaple/dac.c
##	STM32duino/cores/maple/libmaple/dma.c
##	STM32duino/cores/maple/libmaple/dma_f1.c
##	STM32duino/cores/maple/libmaple/exc.S
##	STM32duino/cores/maple/libmaple/exti.c
##	STM32duino/cores/maple/libmaple/exti_f1.c
#	STM32duino/cores/maple/libmaple/flash.c
##	STM32duino/cores/maple/libmaple/fsmc_f1.c
#	STM32duino/cores/maple/libmaple/gpio.c
#	STM32duino/cores/maple/libmaple/gpio_f1.c
#	STM32duino/cores/maple/libmaple/i2c.c
#	STM32duino/cores/maple/libmaple/i2c_f1.c
#	STM32duino/cores/maple/libmaple/iwdg.c
#	STM32duino/cores/maple/libmaple/nvic.c
##	STM32duino/cores/maple/libmaple/pwr.c
#	STM32duino/cores/maple/libmaple/rcc.c
#	STM32duino/cores/maple/libmaple/rcc_f1.c
##	STM32duino/cores/maple/libmaple/spi.c
##	STM32duino/cores/maple/libmaple/spi_f1.c
#	STM32duino/cores/maple/libmaple/systick.c
#	STM32duino/cores/maple/libmaple/timer.c
##	STM32duino/cores/maple/libmaple/timer_f1.c
#	STM32duino/cores/maple/libmaple/usart.c
#	STM32duino/cores/maple/libmaple/usart_f1.c
#	STM32duino/cores/maple/libmaple/usart_private.c
#	STM32duino/cores/maple/libmaple/util.c
#
#	STM32duino/cores/maple/libmaple/stm32f1/performance/isrs.S
#	STM32duino/cores/maple/libmaple/stm32f1/performance/vector_table.S
#
#	STM32duino/cores/maple/libmaple/usb/stm32f1/usb.c
#	STM32duino/cores/maple/libmaple/usb/stm32f1/usb_cdcacm.c
#	STM32duino/cores/maple/libmaple/usb/stm32f1/usb_reg_map.c
#
#	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_core.c
#	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_init.c
#	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_mem.c
#	STM32duino/cores/maple/libmaple/usb/usb_lib/usb_regs.c
#)
#
#ADD_LIBRARY(STM32duino STATIC ${LIBMAPLE_SRC})
#
#TARGET_INCLUDE_DIRECTORIES(STM32duino PRIVATE
#	STM32duino/system/libmaple/usb/stm32f1
#	STM32duino/system/libmaple/usb/usb_lib
#)
#
#TARGET_COMPILE_DEFINITIONS(STM32duino PRIVATE 
#	-DVECT_TAB_ADDR=${VECT_TAB_ADDR} 
#	-DGENERIC_BOOTLOADER 
#	-DBOARD_maple
#)



###################
# STM32 generic - HAL based Arduino API implementation
###################

SET(STM32GENERIC_SRC
	STM32generic/cores/arduino/abi.cpp
	STM32generic/cores/arduino/Arduino.h
#	STM32generic/cores/arduino/hooks.c
#	STM32generic/cores/arduino/itoa.c
#	STM32generic/cores/arduino/itoa.h
	STM32generic/cores/arduino/main.cpp
#	STM32generic/cores/arduino/new.cpp
#	STM32generic/cores/arduino/new.h
	STM32generic/cores/arduino/pins_arduino.h
	STM32generic/cores/arduino/Print.cpp
	STM32generic/cores/arduino/Print.h
	STM32generic/cores/arduino/Printable.h
	STM32generic/cores/arduino/Stream.cpp
	STM32generic/cores/arduino/Stream.h
	STM32generic/cores/arduino/syscalls.c
	STM32generic/cores/arduino/syscalls.h
#	STM32generic/cores/arduino/wiring_private.h
#	STM32generic/cores/arduino/WMath.cpp
#	STM32generic/cores/arduino/WString.cpp
#	STM32generic/cores/arduino/WString.h

	STM32generic/cores/arduino/stm32/SerialUART.cpp
	STM32generic/cores/arduino/stm32/SerialUART.h
	STM32generic/cores/arduino/stm32/SerialUSB.cpp
	STM32generic/cores/arduino/stm32/SerialUSB.h
#	STM32generic/cores/arduino/stm32/stm32_ADC.c
#	STM32generic/cores/arduino/stm32/stm32_build_defines.h
	STM32generic/cores/arduino/stm32/stm32_clock.h
#	STM32generic/cores/arduino/stm32/stm32_def.h
	STM32generic/cores/arduino/stm32/stm32_gpio.c
	STM32generic/cores/arduino/stm32/stm32_gpio.h
	STM32generic/cores/arduino/stm32/stm32_gpio_af.c
	STM32generic/cores/arduino/stm32/stm32_gpio_af.h
#	STM32generic/cores/arduino/stm32/stm32_gpio_af_F0F2F3F4F7L0L1L4.c
	STM32generic/cores/arduino/stm32/stm32_gpio_af_F1.c
	STM32generic/cores/arduino/stm32/stm32_gpio_exti.c
	STM32generic/cores/arduino/stm32/stm32_init.c
	STM32generic/cores/arduino/stm32/stm32_PWM.c
	STM32generic/cores/arduino/stm32/USBDevice.cpp
	STM32generic/cores/arduino/stm32/USBDevice.h

#	STM32generic/cores/arduino/usb/usb_device.c
#	STM32generic/cores/arduino/usb/usb_device.h
#	STM32generic/cores/arduino/usb/usbd_conf.h
	STM32generic/cores/arduino/usb/usbd_conf_F1.c
#	STM32generic/cores/arduino/usb/usbd_conf_F4.c
#	STM32generic/cores/arduino/usb/usbd_conf_F7.c
#	STM32generic/cores/arduino/usb/usbd_conf_L0.c
#	STM32generic/cores/arduino/usb/usbd_conf_L4.c
	STM32generic/cores/arduino/usb/usbd_core.c
	STM32generic/cores/arduino/usb/usbd_core.h
	STM32generic/cores/arduino/usb/usbd_ctlreq.c
	STM32generic/cores/arduino/usb/usbd_ctlreq.h
#	STM32generic/cores/arduino/usb/usbd_def.h
	STM32generic/cores/arduino/usb/usbd_desc.c
	STM32generic/cores/arduino/usb/usbd_desc.h
	STM32generic/cores/arduino/usb/usbd_ioreq.c
	STM32generic/cores/arduino/usb/usbd_ioreq.h

	STM32generic/cores/arduino/usb/cdc/usbd_cdc.c
	STM32generic/cores/arduino/usb/cdc/usbd_cdc.h
	STM32generic/cores/arduino/usb/cdc/usbd_cdc_if.c
	STM32generic/cores/arduino/usb/cdc/usbd_cdc_if.h

	STM32generic/system/STM32F1/CMSIS_Inc/stm32f103xb.h
	STM32generic/system/STM32F1/CMSIS_Inc/stm32f1xx.h
	STM32generic/system/STM32F1/CMSIS_Inc/system_stm32f1xx.h
	STM32generic/system/STM32F1/CMSIS_Src/startup_stm32f103xb.s

	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_adc.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_adc_ex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_can.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_cec.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_cortex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_crc.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_dac.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_dac_ex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_dma.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_eth.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_flash.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_flash_ex.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_gpio.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_gpio_ex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_hcd.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_i2c.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_i2s.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_irda.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_iwdg.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_msp_template.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_nand.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_nor.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pccard.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pcd.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pcd_ex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pwr.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rcc.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rcc_ex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rtc.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rtc_ex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_sd.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_smartcard.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_spi.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_spi_ex.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_sram.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_tim.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_tim_ex.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_uart.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_usart.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_wwdg.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_fsmc.c
#	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_sdmmc.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_usb.c
	STM32generic/system/STM32F1/HAL_Src/system_stm32f1xx.c

	STM32generic/variants/BLUEPILL/systemclock_config.c
	STM32generic/variants/BLUEPILL/variant.cpp
	STM32generic/variants/BLUEPILL/variant.h
)

ADD_LIBRARY(STM32generic STATIC ${STM32GENERIC_SRC})

TARGET_INCLUDE_DIRECTORIES(STM32generic PRIVATE
	STM32generic/system/STM32F1/stm32_chip
#	STM32duino/system/libmaple/usb/usb_lib
)

TARGET_COMPILE_DEFINITIONS(STM32generic PRIVATE 
	-DMENU_USB_SERIAL
	-DVECT_TAB_OFFSET=${VECT_TAB_ADDR} 
#	-DGENERIC_BOOTLOADER 
#	-DBOARD_maple
)

