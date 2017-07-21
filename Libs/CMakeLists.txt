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
# SD card library
###################

SET(SDFAT_SRC
	SdFat/BlockDriver.h
	SdFat/FreeStack.h
#	SdFat/MinimumSerial.cpp
#	SdFat/MinimumSerial.h
	SdFat/SdFat.h
	SdFat/SdFatConfig.h
	SdFat/SysCall.h

	SdFat/FatLib/ArduinoFiles.h
	SdFat/FatLib/ArduinoStream.h
	SdFat/FatLib/BaseBlockDriver.h
	SdFat/FatLib/bufstream.h
	SdFat/FatLib/FatApiConstants.h
	SdFat/FatLib/FatFile.cpp
	SdFat/FatLib/FatFile.h
	SdFat/FatLib/FatFileLFN.cpp
	SdFat/FatLib/FatFilePrint.cpp
	SdFat/FatLib/FatFileSFN.cpp
	SdFat/FatLib/FatFileSystem.h
	SdFat/FatLib/FatLib.h
	SdFat/FatLib/FatLibConfig.h
	SdFat/FatLib/FatStructs.h
	SdFat/FatLib/FatVolume.cpp
	SdFat/FatLib/FatVolume.h
	SdFat/FatLib/FmtNumber.cpp
	SdFat/FatLib/FmtNumber.h
#	SdFat/FatLib/fstream.cpp
#	SdFat/FatLib/fstream.h
#	SdFat/FatLib/ios.h
#	SdFat/FatLib/iostream.h
#	SdFat/FatLib/istream.cpp
#	SdFat/FatLib/istream.h
#	SdFat/FatLib/ostream.cpp
#	SdFat/FatLib/ostream.h
#	SdFat/FatLib/StdioStream.cpp
#	SdFat/FatLib/StdioStream.h

	SdFat/SdCard/SdInfo.h
#	SdFat/SdCard/SdioCard.h
#	SdFat/SdCard/SdioCardEX.cpp
#	SdFat/SdCard/SdioTeensy.cpp
	SdFat/SdCard/SdSpiCard.cpp
	SdFat/SdCard/SdSpiCard.h
#	SdFat/SdCard/SdSpiCardEX.cpp

	SdFat/SpiDriver/DigitalPin.h
	SdFat/SpiDriver/SdSpiBaseDriver.h
	SdFat/SpiDriver/SdSpiDriver.h
#	SdFat/SpiDriver/SdSpiESP8266.cpp
#	SdFat/SpiDriver/SdSpiSAM3X.cpp
	SdFat/SpiDriver/SdSpiSTM32F1.cpp
#	SdFat/SpiDriver/SdSpiTeensy3.cpp
	SdFat/SpiDriver/SoftSPI.h
)
ADD_LIBRARY(SDFAT STATIC ${SDFAT_SRC})

TARGET_INCLUDE_DIRECTORIES(SDFAT PRIVATE
	SdFat
	STM32generic/libraries/SPI/src   # just for SPISettings
)

TARGET_COMPILE_DEFINITIONS(SDFAT PRIVATE
	-DENABLE_ARDUINO_FEATURES=0
)


###################
# FreeRTOS
###################

SET(FREERTOS_SRC
	FreeRTOS/Arduino_FreeRTOS.c
	FreeRTOS/Arduino_FreeRTOS.h
	FreeRTOS/FreeRTOS.h
	FreeRTOS/FreeRTOSConfig.h
	FreeRTOS/heap_1.c
	FreeRTOS/list.c
	FreeRTOS/list.h
	FreeRTOS/mpu_prototypes.h
	FreeRTOS/mpu_wrappers.h
	FreeRTOS/port.c
	FreeRTOS/portable.h
	FreeRTOS/portmacro.h
	FreeRTOS/projdefs.h
	FreeRTOS/queue.c
	FreeRTOS/queue.h
	FreeRTOS/semphr.h
	FreeRTOS/StackMacros.h
	FreeRTOS/task.h
	FreeRTOS/tasks.c
	FreeRTOS/timers.c
	FreeRTOS/timers.h
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
#	Adafruit_SSD1306/ssd1306_i2c_driver.cpp
#	Adafruit_SSD1306/ssd1306_i2c_driver.h
#	Adafruit_SSD1306/ssd1306_spi_driver.cpp
#	Adafruit_SSD1306/ssd1306_spi_driver.h
#	Adafruit_SSD1306/ssd1306_sw_spi_driver.cpp
#	Adafruit_SSD1306/ssd1306_sw_spi_driver.h
)

TARGET_INCLUDE_DIRECTORIES(AdafruitSSD1306 PRIVATE
#	STM32generic/libraries/Wire/src
#	STM32generic/libraries/SPI/src
	AdafruitGFX
)


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
#	STM32generic/cores/arduino/stm32/SerialUSB.cpp
#	STM32generic/cores/arduino/stm32/SerialUSB.h
##	STM32generic/cores/arduino/stm32/stm32_ADC.c
##	STM32generic/cores/arduino/stm32/stm32_build_defines.h
#	STM32generic/cores/arduino/stm32/stm32_clock.h
##	STM32generic/cores/arduino/stm32/stm32_def.h
#	STM32generic/cores/arduino/stm32/stm32_gpio.c
#	STM32generic/cores/arduino/stm32/stm32_gpio.h
#	STM32generic/cores/arduino/stm32/stm32_gpio_af.c
#	STM32generic/cores/arduino/stm32/stm32_gpio_af.h
##	STM32generic/cores/arduino/stm32/stm32_gpio_af_F0F2F3F4F7L0L1L4.c
#	STM32generic/cores/arduino/stm32/stm32_gpio_af_F1.c
#	STM32generic/cores/arduino/stm32/stm32_gpio_exti.c
	STM32generic/cores/arduino/stm32/stm32_init.c
#	STM32generic/cores/arduino/stm32/stm32_pin_list.c
#	STM32generic/cores/arduino/stm32/stm32_PWM.c
#	STM32generic/cores/arduino/stm32/USBDevice.cpp
#	STM32generic/cores/arduino/stm32/USBDevice.h
 
##	STM32generic/cores/arduino/usb/usb_device.c
##	STM32generic/cores/arduino/usb/usb_device.h
#	STM32generic/cores/arduino/usb/usbd_conf.h
#	STM32generic/cores/arduino/usb/usbd_conf_F1.c
##	STM32generic/cores/arduino/usb/usbd_conf_F4.c
##	STM32generic/cores/arduino/usb/usbd_conf_F7.c
##	STM32generic/cores/arduino/usb/usbd_conf_L0.c
##	STM32generic/cores/arduino/usb/usbd_conf_L4.c
#	STM32generic/cores/arduino/usb/usbd_core.c
#	STM32generic/cores/arduino/usb/usbd_core.h
#	STM32generic/cores/arduino/usb/usbd_ctlreq.c
#	STM32generic/cores/arduino/usb/usbd_ctlreq.h
#	STM32generic/cores/arduino/usb/usbd_def.h
#	STM32generic/cores/arduino/usb/usbd_desc.c
#	STM32generic/cores/arduino/usb/usbd_desc.h
#	STM32generic/cores/arduino/usb/usbd_ioreq.c
#	STM32generic/cores/arduino/usb/usbd_ioreq.h
 
#	STM32generic/cores/arduino/usb/cdc/usbd_cdc.c
#	STM32generic/cores/arduino/usb/cdc/usbd_cdc.h
#	STM32generic/cores/arduino/usb/cdc/usbd_cdc_if.c
#	STM32generic/cores/arduino/usb/cdc/usbd_cdc_if.h


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
# USB Core and classes
###################

SET(USB_SRC
	USB/Core/usbd_conf.c
	USB/Core/usbd_conf.h
	USB/Core/usbd_core.c
	USB/Core/usbd_core.h
	USB/Core/usbd_ctlreq.c
	USB/Core/usbd_ctlreq.h
	USB/Core/usbd_def.h
	USB/Core/usbd_desc.c
	USB/Core/usbd_desc.h
	USB/Core/usbd_ioreq.c
	USB/Core/usbd_ioreq.h

	USB/CDC/usbd_cdc.c
	USB/CDC/usbd_cdc.h
	USB/CDC/usbd_cdc_if.c
	USB/CDC/usbd_cdc_if.h

	USB/MSC/usbd_msc.c
	USB/MSC/usbd_msc.h
	USB/MSC/usbd_msc_bot.c
	USB/MSC/usbd_msc_bot.h
	USB/MSC/usbd_msc_data.c
	USB/MSC/usbd_msc_data.h
	USB/MSC/usbd_msc_scsi.c
	USB/MSC/usbd_msc_scsi.h
)
ADD_LIBRARY(USB STATIC ${USB_SRC})

TARGET_INCLUDE_DIRECTORIES(USB PRIVATE
	USB/Core
	USB/CDC
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

	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_adc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_adc_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_can.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_can_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_cec.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_conf.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_cortex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_crc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_dac.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_dac_ex.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_def.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_dma.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_dma_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_eth.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_flash.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_flash_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_gpio.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_gpio_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_hcd.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_i2c.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_i2s.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_irda.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_iwdg.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_mmc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_nand.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_nor.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_pccard.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_pcd.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_pcd_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_pwr.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_rcc.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_rcc_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_rtc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_rtc_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_sd.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_smartcard.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_spi.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_sram.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_tim.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_tim_ex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_uart.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_usart.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_hal_wwdg.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_adc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_bus.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_cortex.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_crc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_dac.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_dma.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_exti.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_fsmc.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_gpio.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_i2c.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_iwdg.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_pwr.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_rcc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_rtc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_sdmmc.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_spi.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_system.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_tim.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_usart.h
	STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_usb.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_utils.h
	#STM32generic/system/STM32F1/HAL_Inc/stm32f1xx_ll_wwdg.h

	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_adc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_adc_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_can.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_cec.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_cortex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_crc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_dac.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_dac_ex.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_dma.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_eth.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_flash.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_flash_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_gpio.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_gpio_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_hcd.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_i2c.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_i2s.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_irda.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_iwdg.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_mmc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_nand.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_nor.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pccard.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pcd.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pcd_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_pwr.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rcc.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rcc_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rtc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_rtc_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_sd.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_smartcard.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_spi.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_spi_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_sram.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_tim.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_tim_ex.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_uart.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_usart.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_hal_wwdg.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_adc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_crc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_dac.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_dma.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_exti.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_fsmc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_gpio.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_i2c.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_pwr.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_rcc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_rtc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_sdmmc.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_spi.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_tim.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_usart.c
	STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_usb.c
	#STM32generic/system/STM32F1/HAL_Src/stm32f1xx_ll_utils.c
	STM32generic/system/STM32F1/HAL_Src/system_stm32f1xx.c
)

ADD_LIBRARY(HAL STATIC ${HAL_SRC})

TARGET_INCLUDE_DIRECTORIES(HAL PRIVATE

)

TARGET_COMPILE_DEFINITIONS(HAL PRIVATE 
	-DVECT_TAB_OFFSET=${VECT_TAB_ADDR} 
)
