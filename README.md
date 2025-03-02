# Rosco r2 Expansion Board

This repository contains the design and sample code for an expansion
board that suits the [rosco-m68k r2 SBC](https://rosco-m68k.com/).
It support three options:

 - a CH375 interface to a USB block device (e.g. USB key, USB drive),
 - an I2C interface to a DS1302 real-time clock (RTC), and
 - 1M of expansion RAM.

You will find the KiCaD 8 PCB design in the [kicad](kicad) folder, the
GALasm code for the two ATF22V10 GALs in the [pld](pld) folder, and some
example software to drive the CH375 and DS1302 in the [software](software)
folder.

If you want the CH375 interface, you will need to burn and install GAL 1.
If you want the expansion RAM, you will beed to burn and install GAL 2.
If you want the DS1302 RTC, you will need to burn and install both GALs.

## Gerbers and Board Images

In the [kicad](kicad) folder there is the KiCad design for the PCB in a Zip
file, the gerbers in a Zip file, a photo of the finished PCB and photos
of the CH375 and DS1302 modules.

## Bill of Materials

Here is a BoM for a fully populated PCB:

 - two ATF22V10C GALs
 - two AS6C4008-55 RAM chips
 - four 100nF size 0805 SMD caps
 - two Harwin M20-7823242 32-pin sockets
 - one Harwin M20-7830846 2x8-pin socket
 - one CH375 module
 - one DS1302 module
 - one CR2032 button battery

I used DIP sockets for the GALs and RAM chips.
I also decided to solder the DS1302 module directly to the PCB.

Here are links to the type of CH375 and DS1302 modules that I used:

 - [CH375 from Electropeak](https://electropeak.com/ch375b-read-and-write-usb-host-interface-communication-module)
 - [DS1302 from Altronics](https://www.altronics.com.au/p/z6207-ds1302-real-time-clock-module/)

The links are solely for you to see the layout of the modules; I do not
endorse the specific vendors of these modules.

## GAL Code

I've left not only the original GAL code for the two GALs in 
the [pld](pld) folder, but also the resulting JED files.

The `Makefile` in this folder shows you how to burn the JED files to
the GALs from Linux using the
[minipro](https://gitlab.com/DavidGriffith/minipro)
tool and a TL866II+ or T48 device.

If you want to build your own JED files, you will need the
[GALasm](https://github.com/daveho/GALasm) tool installed.

## Software

In the [software](software) folder you will find two projects:

 - [software/ch375test](software/ch375test)
 - [software/ds1302test](software/ds1302test)

You can copy both folders into the `code/software` folder of your copy
of the [rosco_m68k](https://github.com/rosco-m68k/rosco_m68k) GitHub
repository.

The `ch375test` program initialises the CH375 device, reads the first
block from the USB block device, then overwrites this block.
The code for the CH375 is mostly in C. I've also written the block
read/write code in assembly for speed. You may want to look at
[that version](https://github.com/EtchedPixels/FUZIX/blob/master/Kernel/platform/platform-rosco-r2/p68000.S) for ideas.

The `ds1302test` program reads the time and date from the DS1302 module,
prints it out and asks if you want to set the time. If you say yes, then
you can type in a new date and time, and the program will update the
DS1302 module.

## Datasheets

In the [datasheets](folder) I've put copies of the datasheets
for the CH375 and DS1302 devices. I haven't tried, but I think
the CH376 is upwardly compatible with the CH375; if you can't
get a CH375 module you might be able to use a CH376 module instead.
