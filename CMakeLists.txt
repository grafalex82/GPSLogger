# Always build in release as it uses -Os
SET(CMAKE_BUILD_TYPE Release)

# Handy path variables used in this script and passed to submodules
SET(GPSLOGGER_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libs)

# All compiler related stuff is defined in the toolchain file
SET(STM32_CHIP "STM32F103RC")

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
	-DSTM32F103RC
	-DSTM32F103xE # That is correct, all chips (F103xC, F103xD, and F103xE) have code under this define
#	-DUSE_USB_COMPOSITE
#	-DUSE_USB_MSC
	-DF_CPU=72000000L
	-DARDUINO=10801
)

# Project wide include directories
INCLUDE_DIRECTORIES(
	# Arduino layer (MiniArduino)
	${GPSLOGGER_LIBS_DIR}/MiniArduino

	# HAL and CMSIS
	${GPSLOGGER_LIBS_DIR}/STM32generic/system/CMSIS
	${GPSLOGGER_LIBS_DIR}/STM32generic/system/STM32F1/CMSIS_Inc
	${GPSLOGGER_LIBS_DIR}/STM32generic/system/STM32F1/HAL_Inc
)

ADD_SUBDIRECTORY(Libs)
ADD_SUBDIRECTORY(Src)

