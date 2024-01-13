Programming the Gen4-uLCD-32PT for the RC-1101

The LCD device has both a built-in EEPROM program memory and
a micro-SD plugin memory card. The RC-1101 requires <strong>both</strong> be programmed!

Programming the Gen4-uLCD-32PT requires a 4D systems programming cable.
They offer more than one option, and they have changed over time. The
older breakout can program both the old and new LCDs, but the new breakouts have
a connector unique to the Gen4 display.

Use the free IDE from 4D systems to compile the two projects.
https://www.4dsystems.com.au/product/4D_Workshop_4_IDE/

<p>There are (currently) four separate projects to build under the 4D IDE.
The one at Loader32PTU/Loader32PTU.4dg is a loader that you must program into the LCD device
FLASH. It accepts a command over its serial port from the arduino to load a selected
4xe file. This FLASH program in the LCD must be compatible with the ino file in 
the sketch in this same RC1101 source code repository. The other three projects
are rig-specific screen layouts for the RC-1101 LCD.</p>

<p>The workshop files in this directory are identical for those few
RC-1101's built with the older uLCD-32PTU
 device. The 4D Workshop build for the Gen4-uLCD-32PT-AR target as specified in
the .4DGenie projects works fine on those older LCD uSD cards.</p>

The IcomAndFlex.4DGenie project has the screen displays for:
<ul>
<li>A generic form for the case that there is no rig-specific WriteLog driver for the RC-1101.
<li>Icom rigs of the vintage IC-756PRO and newer.
 <li>Flex-6000
</ul>
<p>The 4D workshop build of IcomAndFlex.4DGenie produces a series of files recognizable by their
8.3 DOS file names plus the file IcomAndFlex.4XE.</p>

The TS590andK3.4DGenie has displays for:
<ul>
<li>The same generic form as the above .4D project.
<li>Elecraft K2
<li>Elecraft K3
<li>TS-590
</ul>
<p>The 4D workshop build of TS590andK3.4DGenie produces a series of files recognizable by their
8.3 DOS file names (like the other project above.) And it also creates the file TS590andK3.4xe.
This last file must be renamed TS590.4xe on the uSD card for current WriteLog rig drivers to
specify it. The makeimage.cmd file here does the appropriate file renaming automatically.</p>

The Yaesu.4DGenie has displays for:
<ul>
<li>The same generic form as the above .4D projects.
<li>Recent Yaesu rigs, including the FtDx-10, FtDx-100, FtDX-5000 and others.
</ul>
<p>The 4D workshop build of Yaesu.4DGenie produces a series of files recognizable by their
8.3 DOS file names (like the other project above.) And it also creates the file Yaesu.4xe.</p>

<p>The <code>makeimage.cmd</code> batch file creates a zip file image of what must be copied
to the LCD's uSD card.</p>
<p> The batch file also creates a <code>RunFlash.4xe</code> for 
compatibility with older LCD/Arduino firmware (prior to the WL12.49-Firmware tag in this repository.) 
That older firmware lacks the ability to load a rig-specific 4xe file, and always and only
uses <code>RunFlash.4xe</code>. When you update 
the uSD card on an RC-1101 with the old firmware,  
you must choose which of the rigs it will support by copying the <code>RunFlash.4xe</code>
from one of the 4xe files created above. If you don't know the firmware status,
its safe to copy the appropriate 4xe file to the name <code>RunFlash.4xe</code> on the uSD
card.</p>

