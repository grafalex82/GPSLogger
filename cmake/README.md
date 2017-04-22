# Files origin

Files in this folder originally taken from https://github.com/ObKo/stm32-cmake with minor modifications.
Only 2 files are used from the original project

# Changes details

These files have compiler flags fine tuned for GPS Logger project in order to reduce resulting firmware size.


# TODOs

I had to remove -flto flag from compiler and linker switches as Arduino/stm32duino sources do not compile with this flag.
Obviously this is a helpful option that needs attention later