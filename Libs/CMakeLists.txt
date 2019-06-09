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


####################
## SD card library
####################

#SET(SDFAT_SRC
#	SdFat/BlockDriver.h
#	SdFat/FreeStack.h
##	SdFat/MinimumSerial.cpp
##	SdFat/MinimumSerial.h
#	SdFat/SdFat.h
#	SdFat/SdFatConfig.h
#	SdFat/SysCall.h

#	SdFat/FatLib/ArduinoFiles.h
#	SdFat/FatLib/ArduinoStream.h
#	SdFat/FatLib/BaseBlockDriver.h
#	SdFat/FatLib/bufstream.h
#	SdFat/FatLib/FatApiConstants.h
#	SdFat/FatLib/FatFile.cpp
#	SdFat/FatLib/FatFile.h
#	SdFat/FatLib/FatFileLFN.cpp
#	SdFat/FatLib/FatFilePrint.cpp
#	SdFat/FatLib/FatFileSFN.cpp
#	SdFat/FatLib/FatFileSystem.h
#	SdFat/FatLib/FatLib.h
#	SdFat/FatLib/FatLibConfig.h
#	SdFat/FatLib/FatStructs.h
#	SdFat/FatLib/FatVolume.cpp
#	SdFat/FatLib/FatVolume.h
#	SdFat/FatLib/FmtNumber.cpp
#	SdFat/FatLib/FmtNumber.h
##	SdFat/FatLib/fstream.cpp
##	SdFat/FatLib/fstream.h
##	SdFat/FatLib/ios.h
##	SdFat/FatLib/iostream.h
##	SdFat/FatLib/istream.cpp
##	SdFat/FatLib/istream.h
##	SdFat/FatLib/ostream.cpp
##	SdFat/FatLib/ostream.h
##	SdFat/FatLib/StdioStream.cpp
##	SdFat/FatLib/StdioStream.h

#	SdFat/SdCard/SdInfo.h
##	SdFat/SdCard/SdioCard.h
##	SdFat/SdCard/SdioCardEX.cpp
##	SdFat/SdCard/SdioTeensy.cpp
#	SdFat/SdCard/SdSpiCard.cpp
#	SdFat/SdCard/SdSpiCard.h
##	SdFat/SdCard/SdSpiCardEX.cpp

#	SdFat/SpiDriver/DigitalPin.h
#	SdFat/SpiDriver/SdSpiBaseDriver.h
#	SdFat/SpiDriver/SdSpiDriver.h
##	SdFat/SpiDriver/SdSpiESP8266.cpp
##	SdFat/SpiDriver/SdSpiSAM3X.cpp
#	SdFat/SpiDriver/SdSpiSTM32F1.cpp
##	SdFat/SpiDriver/SdSpiTeensy3.cpp
#	SdFat/SpiDriver/SoftSPI.h
#)
#ADD_LIBRARY(SDFAT STATIC ${SDFAT_SRC})

#TARGET_INCLUDE_DIRECTORIES(SDFAT PRIVATE
#	SdFat
#	STM32generic/libraries/SPI/src   # just for SPISettings
#)

#TARGET_COMPILE_DEFINITIONS(SDFAT PRIVATE
#	-DENABLE_ARDUINO_FEATURES=0
#)


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
	AdafruitGFX
)


###################
# Mini Arduino - HAL based Arduino API Minimal implementation (based on STM32GENERIC project)
###################

SET(MINI_ARDUINO_SRC
    MiniArduino/abi.cpp
    MiniArduino/Print.cpp
    MiniArduino/Print.h
    MiniArduino/Printable.h
    MiniArduino/Stream.cpp
    MiniArduino/Stream.h
)

ADD_LIBRARY(MiniArduino STATIC ${MINI_ARDUINO_SRC})


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

	USB/MSC_CDC/usbd_msc_cdc.c
	USB/MSC_CDC/usbd_msc_cdc.h
)
ADD_LIBRARY(USB STATIC ${USB_SRC})

TARGET_INCLUDE_DIRECTORIES(USB PRIVATE
	USB/Core
	USB/CDC
	USB/MSC
	USB/MSC_CDC
)


###################
# CMSIS and HAL
# 
# Note: In order not to make yet another copy of CMSIS and HAL in the source tree I'll use
#       stm32generic's one for now. It seems it is pretty fresh and match the original
###################


SET(HAL_SRC
	HAL/STM32F1/CMSIS_Inc/stm32f103xe.h
	HAL/STM32F1/CMSIS_Inc/stm32f1xx.h
	HAL/STM32F1/CMSIS_Inc/system_stm32f1xx.h
	HAL/STM32F1/CMSIS_Src/startup_stm32f103xe.s

	HAL/STM32F1/HAL_Inc/stm32f1xx_hal.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_adc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_adc_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_can.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_can_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_cec.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_conf.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_cortex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_crc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_dac.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_dac_ex.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_def.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_dma.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_dma_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_eth.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_flash.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_flash_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_gpio.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_gpio_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_hcd.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_i2c.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_i2s.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_irda.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_iwdg.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_mmc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_nand.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_nor.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_pccard.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_pcd.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_pcd_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_pwr.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_rcc.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_rcc_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_rtc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_rtc_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_sd.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_smartcard.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_hal_spi.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_sram.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_tim.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_tim_ex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_uart.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_usart.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_hal_wwdg.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_adc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_bus.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_cortex.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_crc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_dac.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_dma.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_exti.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_fsmc.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_ll_gpio.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_i2c.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_iwdg.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_pwr.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_rcc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_rtc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_sdmmc.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_spi.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_system.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_tim.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_ll_usart.h
	HAL/STM32F1/HAL_Inc/stm32f1xx_ll_usb.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_utils.h
	#HAL/STM32F1/HAL_Inc/stm32f1xx_ll_wwdg.h

	HAL/STM32F1/HAL_Src/stm32f1xx_hal.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_adc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_adc_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_can.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_cec.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_cortex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_crc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_dac.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_dac_ex.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_dma.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_eth.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_flash.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_flash_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_gpio.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_gpio_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_hcd.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_i2c.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_i2s.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_irda.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_iwdg.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_mmc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_nand.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_nor.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_pccard.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_pcd.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_pcd_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_pwr.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_rcc.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_rcc_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_rtc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_rtc_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_sd.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_smartcard.c
	HAL/STM32F1/HAL_Src/stm32f1xx_hal_spi.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_spi_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_sram.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_tim.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_tim_ex.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_uart.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_usart.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_hal_wwdg.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_adc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_crc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_dac.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_dma.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_exti.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_fsmc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_gpio.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_i2c.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_pwr.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_rcc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_rtc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_sdmmc.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_spi.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_tim.c
	HAL/STM32F1/HAL_Src/stm32f1xx_ll_usart.c
	HAL/STM32F1/HAL_Src/stm32f1xx_ll_usb.c
	#HAL/STM32F1/HAL_Src/stm32f1xx_ll_utils.c
	HAL/STM32F1/HAL_Src/system_stm32f1xx.c
)

ADD_LIBRARY(HAL STATIC ${HAL_SRC})

TARGET_INCLUDE_DIRECTORIES(HAL PRIVATE

)

TARGET_COMPILE_DEFINITIONS(HAL PRIVATE 
	-DVECT_TAB_OFFSET=${VECT_TAB_ADDR} 
)
