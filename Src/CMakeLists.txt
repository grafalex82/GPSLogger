SET(SOURCE_FILES
	8x12Font.cpp
	AltitudeScreen.cpp
	Buttons.cpp
	CurrentPositionScreen.cpp
	CurrentTimeScreen.cpp
	DebugScreen.cpp
	FontTest.cpp
	GPSDataModel.cpp
	GPSLogger.cpp
	GPSOdometer.cpp
	GPSSatellitesData.cpp
	GPSThread.cpp
	IdleThread.cpp
	OdometerActionScreen.cpp
	OdometerScreen.cpp
	ParentScreen.cpp
	SatellitesScreen.cpp
	Screen.cpp
	ScreenManager.cpp
	SelectorScreen.cpp
	SettingsGroupScreen.cpp
	SpeedScreen.cpp
	TimeFont.cpp
	TimeZoneScreen.cpp
	Utils.cpp

	../Libs/STM32duino/variants/generic_stm32f103c/wirish/syscalls.c
)

INCLUDE_DIRECTORIES(
	.
	${LIBS_DIR}/AdafruitGFX
	${LIBS_DIR}/NeoGPS
	${LIBS_DIR}/STM32duino/libraries/Adafruit_SSD1306
	${LIBS_DIR}/STM32duino/libraries/SPI/src
	${LIBS_DIR}/STM32duino/libraries/FreeRTOS821
	)


ADD_EXECUTABLE(GPSLogger ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(GPSLogger
	NeoGPS
	FreeRTOS
	AdafruitGFX
	AdafruitSSD1306
	ArduinoLibs
	STM32duino
)
