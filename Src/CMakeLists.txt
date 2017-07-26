# Build rules for GPS logger target.
# App specific compiler/linker settings are also defined here

SET(SOURCE_FILES
	# Screens and screen management stuff
	Screens/AltitudeScreen.cpp
	Screens/AltitudeScreen.h
	Screens/CurrentPositionScreen.cpp
	Screens/CurrentPositionScreen.h
	Screens/CurrentTimeScreen.cpp
	Screens/CurrentTimeScreen.h
	Screens/DebugScreen.cpp
	Screens/DebugScreen.h
	Screens/DisplayDriver.cpp
	Screens/DisplayDriver.h
	Screens/OdometerActionScreen.cpp
	Screens/OdometerActionScreen.h
	Screens/OdometerScreen.cpp
	Screens/OdometerScreen.h
	Screens/ParentScreen.cpp
	Screens/ParentScreen.h
	Screens/SatellitesScreen.cpp
	Screens/SatellitesScreen.h
	Screens/Screen.cpp
	Screens/Screen.h
	Screens/ScreenManager.cpp
	Screens/ScreenManager.h
	Screens/SelectorScreen.cpp
	Screens/SelectorScreen.h
	Screens/SettingsGroupScreen.cpp
	Screens/SettingsGroupScreen.h
	Screens/SpeedScreen.cpp
	Screens/SpeedScreen.h
	Screens/TimeZoneScreen.cpp
	Screens/TimeZoneScreen.h

	GPS/GPSDataModel.cpp
	GPS/GPSDataModel.h
	GPS/GPSOdometer.cpp
	GPS/GPSOdometer.h
	GPS/GPSSatellitesData.cpp
	GPS/GPSSatellitesData.h
	GPS/GPSThread.cpp
	GPS/GPSThread.h

	8x12Font.cpp
	8x12Font.h
	BoardInit.cpp
	BoardInit.h
	ButtonsThread.cpp
	ButtonsThread.h
#	FontTest.cpp
#	FontTest.h
	FreeRTOSHelpers.h
	GPSLogger.cpp
	IdleThread.cpp
	IdleThread.h
	LEDThread.cpp
	LEDThread.h
	PrintUtils.cpp
	PrintUtils.h
	SdFatSPIDriver.cpp
	SdFatSPIDriver.h
	SDThread.cpp
	SDThread.h
	TimeFont.cpp
	TimeFont.h
	USBDebugLogger.cpp
	USBDebugLogger.h
)

INCLUDE_DIRECTORIES(
	.
	${GPSLOGGER_LIBS_DIR}/AdafruitGFX
	${GPSLOGGER_LIBS_DIR}/Adafruit_SSD1306
	${GPSLOGGER_LIBS_DIR}/NeoGPS
	${GPSLOGGER_LIBS_DIR}/SdFat
	${GPSLOGGER_LIBS_DIR}/STM32generic/libraries/SPI/src
	${GPSLOGGER_LIBS_DIR}/STM32generic/libraries/FreeRTOS/src
	${GPSLOGGER_LIBS_DIR}/USB/Core
	${GPSLOGGER_LIBS_DIR}/USB/CDC
)


# Do not link to libc or newlib-nano - we are not using anything from that
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nosys.specs")

ADD_EXECUTABLE(GPSLogger ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(GPSLogger
	NeoGPS
	FreeRTOS
	SDFAT
	AdafruitGFX
	AdafruitSSD1306
	USB
	STM32generic
	HAL
	m
)

TARGET_COMPILE_DEFINITIONS(GPSLogger PRIVATE
	-DENABLE_ARDUINO_FEATURES=0	#Disable SdFat stuff
)

STM32_GENERATE_MAP_FILE(GPSLogger)
STM32_SET_TARGET_PROPERTIES(GPSLogger)
STM32_PRINT_SIZE_OF_TARGETS(GPSLogger)

# Additional handy targets
STM32_ADD_HEX_BIN_TARGETS(GPSLogger)
STM32_ADD_DUMP_TARGET(GPSLogger)
