# GPSLogger
STM32-Arduino-based GPS logger with LCD screen. It is highly inspired by Holux M-241 but with multiple improvements

Key features:
* Logging to **SD card** as raw NMEA dump, as well as ready to use GPX format
* Small **128x32 Screen** to display various data: current time, horizontal and vertical speed, altitude, odometers, GPS stats and many more
* **POI** button
* **Multiple odometers** with statistics: total and movement time, maximum and average speed, elevation, min/max altitude
  * each odometer can be started/paused/resumed/reset independently
* **Buzzer** to inform user about important events
* Powered by **Lithium battery** with working time estimation

More features are TBD

# Hardware
Target platform for the project is some kind of STM32 microcontroller - it provides pretty good computation power and enough serial/I2C/SPI ports to connect all the components.
Current development goes with stm32f103c8t6 "Blue Pill" arduino-nano-like development board.

Other hardware used:
* 0.91" OLED 128x32 display
* Beitan BN-880 GPS module, based on UBLOX-M8N chip
* SD card module TBD
* Battery driver module TBD

Connection schematics is TBD. Brief list of connections:
* Display is connected to pins B6/B7 (I2C #1)
* GPS is connected to A9/A10 (UART #1)
* Buttons are connected to C14/C15. Other end is connected to VCC (not GND)

# Software used

The device firmware is based on STM32-arduino (stm32duino) framework to access peripheral. At the same time FreeRTOS is used to drive simultaneous and concurrent tasks. 

Libraries used:
* Adafruit GFX and Adafruit SSD1306 libraries to drive the display
* NeoGPS to parse NMEA GPS protocol

The project is using Arduino sketches concept. Atmel Studio + Visual Micro plugin is used to build all the stuff instead of ArduinoIDE. Due to limitations of arduino build process I will likely search for another build system option.

# Project Structure

* Docs - various useful docs for project components. Just to keep them at a single location
* Libs - Used libraries. Using originals at the moment, but they could be altered in future in terms of optimization
* Fonts - sources for fonts used in the project
* Src - logger sources

# UI structure

Key part of the system is display. All available information is structured to a screen page concept (or just Screen), each displays a specific set of data. 

From the C++ prospective each screen is a class derived from Screen interface. Each class knows how to display itself and implements a reaction on buttons.

Navigation is performed via 2 hardware buttons. Each screen shows a brief tooltip under the button. Typical buttons functionality:
* **Select** button switches to a next screen or value
* **Ok** button applies selected option or enters a submenu/subscreen

Main screens are:
* **Time Screen** displays current time according to selected time zone
  * Ok button enters time zone selection screen
* **Current speed** screen displays speed in selected units (km/h, mph), heading and altitude
  * Ok button leads to altutude and vertical speed screen
* **Current position** screen shows latitude/longtitude position as Degrees/Minutes/Seconds
  * Ok buttons stores current position as a way point (POI)
* **Odometer** screen displays main and additional odometer values
  * Long press of Ok button leads to pause/resume/reset selector. Applies to all odometers at once.
    * Reset has a warning message that all metrics will be lost and therefore has additional confirmation
  * Ok button leads to odometer selection. Each screen has detailed odometer information such as:
    * Distance
    * Overall duration, active (non-paused) duration
    * total average, active average and maximum speeds
    * elevation (difference between starting and resulting altitude), average active vertical speed
    * Sel button selects the odometers
    * Long Ok button leads to pause/resume/reset selector
      * Reset shows additional confirmation
* **Settings** is a set of options split by categories (selected with 'sel' button)
  * **GPS Settings** - various GPS options (instant GPS signal metrics, distance units selection, etc)
  * **SD Card** - options related to SD card handling (free space metrics, raw log enable/disable, gpx store frequency)
  * **System** - system wide settings (language, display off timings, etc)

# Links and misc stuff

* Build log with detailed description of goals, achievements, HW and SW solutions, etc. (in Russian)  - [Part1](https://geektimes.ru/post/286348/), other parts are coming