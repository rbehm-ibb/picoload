# picoload
A (down) loader for the RPi pico

The standard downloader from the RP2040 SDK does not work for me.

So here is a simple replacement:
After entering boot mode for the pico (press BOOT+RESET or double reset if "pico_bootsel_via_double_reset"  is included)
the pico appears as a mounted  dir (..../RPI-RP2/). This and the existance of a .uf2 file in the build dir triggers a  copy(actually a mv)
of the uf2 file into the pico. This will flash and reset it.

This  programm is just started and the  build dir set. Then it watches  for this occurance and flashes the firmware.

It has the avantage that the (USB-)serial port is not touched. So I can have minicom still running. minicom just automatically reconnects
after the download.

