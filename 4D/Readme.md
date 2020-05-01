Programming the Gen4-uLCD-32PT for the RC-1101

The LCD device has both a built-in EEPROM program memory and
a micro-SD plugin memory card. The RC-1101 requires BOTH be programmed!

Programming the Gen4-uLCD-32PT requires a 4D systems programming cable.
They offer more than one option. I have succesfully used the predecessor to this one:
https://4dsystems.com.au/uusb-pa5-ii
The programming breakout I have is no longer on their web site and was for the 
older uLCD-32PTU display as specified in earlier publications of the RC-1101. The
older breakout can program both the old and new LCDs, but the new breakout has
a connector unique too the Gen4 display.

Use the free IDE from 4D systems to compile the two projects.
https://www.4dsystems.com.au/product/4D_Workshop_4_IDE/

<p>There are (currently) three separate projects to build.
The one at Loader32PTU/Loader32PTU.4dg is a small loader that you load into the LCD device
EEPROM. It accepts a command over its serial port from the arduino to load a selected
4xe file. For compatibility with older versions of WriteLog, it by default loads
and runs RUNFLASH.4XE produced by building the IcomAndFlex.4DGenie project in this directory.</p>

<p>The workshop files in this directory have (slightly) different contents for the older uLCD-32PTU
 device. Use that for-uLCD-32 branch to program that older LCD (or use the master branch
  and, in the 4D IDE switch the target device to the older one)
</p>

The IcomAndFlex.4DGenie project has the screen displays for:
<ul>
<li>A generic form for the case that there is no rig-specific WriteLog driver for the RC-1101.
<li>Elecraft K2
<li>Elecraft K3
<li>Icom rigs of the vintage IC-756PRO and newer.
 <li>Flex-6000
</ul>
<p>The 4D workshop build of IcomAndFlex.4DGenie produces a series of files recognizable by their
8.3 DOS file names plus the file IcomAndFlex.4XE. That last one, IcomAndFlex.4XE must be
renamed RunFlash.4xe on the copy on the uSD card installed in the LCD. That name is called out
by the latest WriteLog rig drivers, and for older versions of WriteLog that do not specify
a name, the RC-1101 firmware loads RunFlash.4xe by default.</p>

The TS590andK3.4DGenie has displays for:
<ul>
<li>A generic form for the case that there is no rig-specific WriteLog driver for the RC-1101.
<li>Elecraft K2
<li>Elecraft K3
<li>TS-590
</ul>
<p>The 4D workshop build of TS590andK3.4DGenie produces a series of files recognizable by their
8.3 DOS file names (like the other project above.) And it also creates the file TS590andK3.4xe.
This last file must be renamed TS590.4xe on the uSD card for current WriteLog rig drivers to
specify it.</p>

