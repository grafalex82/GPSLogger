# Always build in release as it uses -Os
SET(CMAKE_BUILD_TYPE Release)

# Handy path variables used in this script and passed to submodules
SET(GPSLOGGER_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libs)

# All compiler related stuff is defined in the toolchain file
SET(STM32_CHIP "STM32F103CB")

# Using Maple's linker script that corresponds maple/stm32duino code
SET(STM32_LINKER_SCRIPT ${GPSLOGGER_LIBS_DIR}/STM32duino/variants/generic_stm32f103c/ld/bootloader_20.ld)
LINK_DIRECTORIES(${GPSLOGGER_LIBS_DIR}/STM32duino/variants/generic_stm32f103c/ld)

# Flash offset due to bootloader
SET(VECT_TAB_ADDR "0x08002000")
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
	-DSTM32F103CB
	-DMCU_STM32F103CB
	-DF_CPU=72000000L
	-DARDUINO=10801
	-DDEBUG_LEVEL=DEBUG_NONE
	-DSERIAL_USB
)

# Project wide include directories
INCLUDE_DIRECTORIES(
	# Arduino core
	${GPSLOGGER_LIBS_DIR}/STM32duino/cores/maple

	# Libmaple/HAL	
	${GPSLOGGER_LIBS_DIR}/STM32duino/system/libmaple
	${GPSLOGGER_LIBS_DIR}/STM32duino/system/libmaple/include
	${GPSLOGGER_LIBS_DIR}/STM32duino/variants/generic_stm32f103c
)

ADD_SUBDIRECTORY(Libs)
ADD_SUBDIRECTORY(Src)

