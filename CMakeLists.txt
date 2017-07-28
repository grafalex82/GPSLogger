# Always build in release as it uses -Os
SET(CMAKE_BUILD_TYPE Release)

# Handy path variables used in this script and passed to submodules
SET(GPSLOGGER_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libs)

# All compiler related stuff is defined in the toolchain file
SET(STM32_CHIP "STM32F103CB")

# Using Maple's linker script that corresponds maple/stm32duino code
#SET(STM32_LINKER_SCRIPT ${GPSLOGGER_LIBS_DIR}/STM32generic/variants/BLUEPILL/ldscript_bootloader_2000.ld)
#LINK_DIRECTORIES(${GPSLOGGER_LIBS_DIR}/STM32generic/variants/BLUEPILL/)

# Flash offset due to bootloader
SET(VECT_TAB_ADDR "0x2000")
SET(STM32_FLASH_ORIGIN "0x08002000")

# Load the toolchain file that uses vars above
SET(CMAKE_TOOLCHAIN_FILE cmake/gcc_stm32.cmake)



# Project definition
PROJECT(GPSLogger)

# Misc stuff
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ENABLE_LANGUAGE(ASM)

# Project wide definitions
ADD_DEFINITIONS(
	-DSTM32F1
	-DSTM32F103CB
	-DSTM32F103xB
#	-DUSE_USB_MSC
	-DMCU_STM32F103CB
	-DF_CPU=72000000L
	-DARDUINO=10801
)

# Project wide include directories
INCLUDE_DIRECTORIES(
	# Arduino layer (stm32generic)
	${GPSLOGGER_LIBS_DIR}/STM32generic/cores/arduino
	${GPSLOGGER_LIBS_DIR}/STM32generic/cores/arduino/stm32
#	${GPSLOGGER_LIBS_DIR}/STM32generic/cores/arduino/usb
#	${GPSLOGGER_LIBS_DIR}/STM32generic/cores/arduino/usb/cdc # TODO: code should not call these functions directly and work via arduino layer
	${GPSLOGGER_LIBS_DIR}/STM32generic/variants/BLUEPILL

	# HAL and CMSIS
	${GPSLOGGER_LIBS_DIR}/STM32generic/system/CMSIS
	${GPSLOGGER_LIBS_DIR}/STM32generic/system/STM32F1/CMSIS_Inc
	${GPSLOGGER_LIBS_DIR}/STM32generic/system/STM32F1/HAL_Inc
)

ADD_SUBDIRECTORY(Libs)
ADD_SUBDIRECTORY(Src)

