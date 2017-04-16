# All compiler related stuff is defined in the toolchain file
add_definitions(-DSTM32_CHIP=STM32F103CB)
SET(CMAKE_TOOLCHAIN_FILE cmake/gcc_stm32.cmake)

# Project definition
PROJECT(GPSLogger)

# Misc stuff
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ENABLE_LANGUAGE(ASM)

# Vars passed to submodules
SET(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libs)

# Project wide definitions
ADD_DEFINITIONS(
	-DSTM32F103CB
	-DMCU_STM32F103CB
	-DF_CPU=72000000
	-DARDUINO=10801
	)

# Flash offset due to bootloader
SET(VECT_TAB_ADDR "0x08002000")

# Project wide include directories
INCLUDE_DIRECTORIES(
	# Arduino core
	${LIBS_DIR}/STM32duino/cores/maple

	# Libmaple/HAL	
	${LIBS_DIR}/STM32duino/system/libmaple
	${LIBS_DIR}/STM32duino/system/libmaple/include
	${LIBS_DIR}/STM32duino/variants/generic_stm32f103c
)

ADD_SUBDIRECTORY(Libs)
ADD_SUBDIRECTORY(Src)

