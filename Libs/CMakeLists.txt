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

SET(FREERTOS_SRC
	STM32generic/libraries/FreeRTOS/src/Arduino_FreeRTOS.c
	STM32generic/libraries/FreeRTOS/src/Arduino_FreeRTOS.h
	STM32generic/libraries/FreeRTOS/src/FreeRTOS.h
	STM32generic/libraries/FreeRTOS/src/FreeRTOSConfig.h
	STM32generic/libraries/FreeRTOS/src/heap_1.c
	STM32generic/libraries/FreeRTOS/src/list.c
	STM32generic/libraries/FreeRTOS/src/list.h
	STM32generic/libraries/FreeRTOS/src/mpu_prototypes.h
	STM32generic/libraries/FreeRTOS/src/mpu_wrappers.h
	STM32generic/libraries/FreeRTOS/src/port_CM3.c
	STM32generic/libraries/FreeRTOS/src/portable.h
	STM32generic/libraries/FreeRTOS/src/portmacro_CM3.h
	STM32generic/libraries/FreeRTOS/src/projdefs.h
	STM32generic/libraries/FreeRTOS/src/queue.c
	STM32generic/libraries/FreeRTOS/src/queue.h
	STM32generic/libraries/FreeRTOS/src/semphr.h
	STM32generic/libraries/FreeRTOS/src/StackMacros.h
	STM32generic/libraries/FreeRTOS/src/task.h
	STM32generic/libraries/FreeRTOS/src/tasks.c
	STM32generic/libraries/FreeRTOS/src/timers.c
	STM32generic/libraries/FreeRTOS/src/timers.h
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
	Adafruit_SSD1306/Adafruit_SSD1306.cpp
	Adafruit_SSD1306/Adafruit_SSD1306.h
	Adafruit_SSD1306/ssd1306_i2c_driver.cpp
	Adafruit_SSD1306/ssd1306_i2c_driver.h
#	Adafruit_SSD1306/ssd1306_spi_driver.cpp
#	Adafruit_SSD1306/ssd1306_spi_driver.h
#	Adafruit_SSD1306/ssd1306_sw_spi_driver.cpp
#	Adafruit_SSD1306/ssd1306_sw_spi_driver.h
)

TARGET_INCLUDE_DIRECTORIES(AdafruitSSD1306 PRIVATE
	STM32generic/libraries/Wire/src
#	STM32generic/libraries/SPI/src
	AdafruitGFX
)


###################
# Various Arduino libraries
###################

SET(ARDUINO_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STM32generic/libraries)

SET(ARDUINO_LIBS_SRC
	${ARDUINO_LIBS_DIR}/Wire/src/Wire.cpp
	${ARDUINO_LIBS_DIR}/Wire/src/Wire.h
#	${ARDUINO_LIBS_DIR}/SPI/src/SPI.cpp
#	${ARDUINO_LIBS_DIR}/SPI/src/SPI.h
)

ADD_LIBRARY(ArduinoLibs STATIC ${ARDUINO_LIBS_SRC})



###################
# STM32 generic - HAL based Arduino API implementation
###################

SET(STM32GENERIC_SRC
	STM32generic/cores/arduino/abi.cpp
#	STM32generic/cores/arduino/Arduino.h
##	STM32generic/cores/arduino/hooks.c
##	STM32generic/cores/arduino/itoa.c
##	STM32generic/cores/arduino/itoa.h
#	STM32generic/cores/arduino/main.cpp
##	STM32generic/cores/arduino/new.cpp
##	STM32generic/cores/arduino/new.h
#	STM32generic/cores/arduino/pins_arduino.h
	STM32generic/cores/arduino/Print.cpp
	STM32generic/cores/arduino/Print.h
	STM32generic/cores/arduino/Printable.h
	STM32generic/cores/arduino/Stream.cpp
	STM32generic/cores/arduino/Stream.h
#	STM32generic/cores/arduino/syscalls.c
#	STM32generic/cores/arduino/syscalls.h
##	STM32generic/cores/arduino/wiring_private.h
##	STM32generic/cores/arduino/WMath.cpp
##	STM32generic/cores/arduino/WString.cpp
##	STM32generic/cores/arduino/WString.h

#	STM32generic/cores/arduino/stm32/SerialUART.cpp
#	STM32generic/cores/arduino/stm32/SerialUART.h
	STM32generic/cores/arduino/stm32/SerialUSB.cpp
	STM32generic/cores/arduino/stm32/SerialUSB.h
##	STM32generic/cores/arduino/stm32/stm32_ADC.c
##	STM32generic/cores/arduino/stm32/stm32_build_defines.h
#	STM32generic/cores/arduino/stm32/stm32_clock.h
##	STM32generic/cores/arduino/stm32/stm32_def.h
	STM32generic/cores/arduino/stm32/stm32_gpio.c
	STM32generic/cores/arduino/stm32/stm32_gpio.h
	STM32generic/cores/arduino/stm32/stm32_gpio_af.c
	STM32generic/cores/arduino/stm32/stm32_gpio_af.h
##	STM32generic/cores/arduino/stm32/stm32_gpio_af_F0F2F3F4F7L0L1L4.c
	STM32generic/cores/arduino/stm32/stm32_gpio_af_F1.c
#	STM32generic/cores/arduino/stm32/stm32_gpio_exti.c
	STM32generic/cores/arduino/stm32/stm32_init.c
	STM32generic/cores/arduino/stm32/stm32_pin_list.c
	STM32generic/cores/arduino/stm32/stm32_PWM.c
	STM32generic/cores/arduino/stm32/USBDevice.cpp
	STM32generic/cores/arduino/stm32/USBDevice.h
 
#	STM32generic/cores/arduino/usb/usb_device.c
#	STM32generic/cores/arduino/usb/usb_device.h
	STM32generic/cores/arduino/usb/usbd_conf.h
	STM32generic/cores/arduino/usb/usbd_conf_F1.c
#	STM32generic/cores/arduino/usb/usbd_conf_F4.c
#	STM32generic/cores/arduino/usb/usbd_conf_F7.c
#	STM32generic/cores/arduino/usb/usbd_conf_L0.c
#	STM32generic/cores/arduino/usb/usbd_conf_L4.c
	STM32generic/cores/arduino/usb/usbd_core.c
	STM32generic/cores/arduino/usb/usbd_core.h
	STM32generic/cores/arduino/usb/usbd_ctlreq.c
	STM32generic/cores/arduino/usb/usbd_ctlreq.h
	STM32generic/cores/arduino/usb/usbd_def.h
	STM32generic/cores/arduino/usb/usbd_desc.c
	STM32generic/cores/arduino/usb/usbd_desc.h
	STM32generic/cores/arduino/usb/usbd_ioreq.c
	STM32generic/cores/arduino/usb/usbd_ioreq.h
 
	STM32generic/cores/arduino/usb/cdc/usbd_cdc.c
	STM32generic/cores/arduino/usb/cdc/usbd_cdc.h
	STM32generic/cores/arduino/usb/cdc/usbd_cdc_if.c
	STM32generic/cores/arduino/usb/cdc/usbd_cdc_if.h


#	STM32generic/variants/BLUEPILL/systemclock_config.c
	STM32generic/variants/BLUEPILL/variant.h
)

ADD_LIBRARY(STM32generic STATIC ${STM32GENERIC_SRC})

TARGET_INCLUDE_DIRECTORIES(STM32generic PRIVATE
	STM32generic/system/STM32F1/stm32_chip
)

TARGET_COMPILE_DEFINITIONS(STM32generic PRIVATE 
	-DMENU_USB_SERIAL
)


###################
# CMSIS and HAL
# 
# Note: In order not to make yet another copy of CMSIS and HAL in the source tree I'll use
#       stm32generic's one for now. It seems it is pretty fresh and match the original
###################


SET(HAL_SRC
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
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_spi.c
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
)

ADD_LIBRARY(HAL STATIC ${HAL_SRC})

TARGET_INCLUDE_DIRECTORIES(HAL PRIVATE

)

TARGET_COMPILE_DEFINITIONS(HAL PRIVATE 
	-DVECT_TAB_OFFSET=${VECT_TAB_ADDR} 
)
