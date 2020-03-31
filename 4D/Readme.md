Programming the uLCD-32PTU for the RC-1101

The LCD device has both a built-in EEPROM program memory and
a micro-SD plugin memory card. The RC-1101 requires BOTH be programmed!

Programming the uLCD-32PTU requires a 4D systems programming cable.
They offer more than one option. I have succesfully used the predecessor to this one:
https://4dsystems.com.au/uusb-pa5-ii
(The one I have is no longer on their web site.)

Use the free IDE from 4D systems to compile the two projects.
https://www.4dsystems.com.au/product/4D_Workshop_4_IDE/

There are two separate projects to build.
The one at RunFlash/RUNRC1101.4dg is a small loader that you load into the LCD device
EEPROM. It does nothing more than find the file RUNFLASH.4XE produced by building
the WLRemoteGeneric.4DGenie project in this directory.

The WLRemoteGeneric.4DGenie project has four "Forms".

A generic form for the case that there is no rig-specific WriteLog driver for the RC-1101.
Icom rigs of the vintage IC-756PRO and newer.
Elecraft K2
Elecraft K3

Should the WLRemoteGeneric.4DGenie project be upgraded in the future,
expect to have to recompile it and reload the micro-SD card. But the
EEPROM programming in RUNRC1101.4dg is expected to be able to handle 
the new RUNFLASH.4XE.
