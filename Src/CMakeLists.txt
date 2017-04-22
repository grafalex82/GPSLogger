# Build rules for GPS logger target.
# App specific compiler/linker settings are also defined here

SET(SOURCE_FILES
	Screens/AltitudeScreen.cpp
	Screens/CurrentPositionScreen.cpp
	Screens/CurrentTimeScreen.cpp
	Screens/DebugScreen.cpp
	Screens/OdometerActionScreen.cpp
	Screens/OdometerScreen.cpp
	Screens/ParentScreen.cpp
	Screens/SatellitesScreen.cpp
	Screens/Screen.cpp
	Screens/ScreenManager.cpp
	Screens/SelectorScreen.cpp
	Screens/SettingsGroupScreen.cpp
	Screens/SpeedScreen.cpp
	Screens/TimeZoneScreen.cpp

	8x12Font.cpp
	Buttons.cpp
	FontTest.cpp
	GPSDataModel.cpp
	GPSLogger.cpp
	GPSOdometer.cpp
	GPSSatellitesData.cpp
	GPSThread.cpp
	IdleThread.cpp
	TimeFont.cpp
	Utils.cpp
)

INCLUDE_DIRECTORIES(
	.
	${GPSLOGGER_LIBS_DIR}/AdafruitGFX
	${GPSLOGGER_LIBS_DIR}/NeoGPS
	${GPSLOGGER_LIBS_DIR}/STM32duino/libraries/Adafruit_SSD1306
	${GPSLOGGER_LIBS_DIR}/STM32duino/libraries/SPI/src
	${GPSLOGGER_LIBS_DIR}/STM32duino/libraries/FreeRTOS821
	)


# Do not link to libc or newlib-nano - we are not using anything from that
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nosys.specs")

ADD_EXECUTABLE(GPSLogger ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(GPSLogger
	NeoGPS
	FreeRTOS
	AdafruitGFX
	AdafruitSSD1306
	ArduinoLibs
	STM32duino
)

STM32_SET_TARGET_PROPERTIES(GPSLogger)
STM32_PRINT_SIZE_OF_TARGETS(GPSLogger)

# Additional handy targets
STM32_ADD_HEX_BIN_TARGETS(GPSLogger)
STM32_ADD_DUMP_TARGET(GPSLogger)
