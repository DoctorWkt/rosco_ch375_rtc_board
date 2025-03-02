## ds1302test

This code tests the interface to the DS1302 real-time clock.
The file `asmcode.asm` holds the I/O addresses for the device
as well as the two functions `rtcread()` and `rtcwrite()` to
send commands and send/get data from the device.

The file `kmain.c` has functions to read/write the year, month, day,
hours, minutes and seconds.

The main function in `kmain.c` prints out the current date/time according
to the DS1302, then asks you if you want to set the time. If you say
yes, you will enter the new date and time. The program updates the
DS1302 and then prints out the new date/time to ensure that it is correct.
