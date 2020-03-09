<h3>Parts List</h3>
You are reading the documentation for design that uses the gen4 display device, the gen4-uLCD-32PT. Switch git branches if you have the older uLCD-32PTU device because
it has different mounting dimensions. The parts to build this device, except for the PCB and the enclosure and the machine screws and nuts, are available at digikey. See a PDF of the parts <a href='partslist.pdf'>here</a>. Or link to the digikey shared shopping cart from <a href='https://www.digikey.com/short/z84fb8'>here</a>.

<p>Note that the specified female headers have a 0.29 inch insulation height. This, combined with the 0.10 inch insulation height
on the matching male header, is a critical dimension
for the Adafruit FT232H breakout board. The USB port on that board will not line up with its matching hole in the enclosure otherwise.
Also note that Adafruit has put their same part number, 2264, on both an older version of the board with a micro-USB connector,
and a newer version with a USB-C connector. Both the old an new Adafruit boards will fit this PCB and enclosure, but of course you'll need a different cable.</p>

<h3>Construction hints</h3>
<ul>
<li>Populate the trellis LED array using the instructions on the 
<a href='https://www.adafruit.com/product/1616'>Adafruit website</a>.</li>
<li>Add the right angle 4 pin SMD header. There is only one position it
will work.
<p align='center'><img width='50%' alt='trellis-1.jpg' src='trellis-1.jpg'/></p>
Use the 4-pin connector as a spacer to ensure you have room to install it later.
<p align='center'><img width='50%' alt='trellis-2.jpg' src='trellis-2.jpg'/></p>
 </li>
<li>Smallest parts go on the PCB first.
<p align='center'><img width='50%' alt='smallest-first.jpg' src='smallest-first.jpg'/></p></li>
<li>Next in size are the 6mm back panel connectors.
<p align='center'><img width='50%' alt='second-in-size.jpg' src='second-in-size.jpg'/></p></li>
<li>Cut the headers for the RS232H part to the approppriate number of pins, both the socket side and the pin side. Confirm you have the 
right size. The total offset of the RS232H from the PCB must be 0.390 inches (25/64 inches, or 9.9mm)
<p align='center'><img width='50%' alt='check-header-offset.jpg' src='check-header-offset.jpg'/></p></li>
<li>Solder the RS232H break out board top and bottom while inserted. This picture was taken after soldering and pulling it out of the sockets. 
<p align='center'><img width='50%' alt='solder-in-rs232h.jpg' src='solder-in-rs232h.jpg'/></p></li>
<li>Rev 3 of the PCB has the flat side of Q1 silkscreened incorrectly.
<p align='center'><img width='50%' alt='Q1-polarity.jpg' src='Q1-polarity.jpg'/></p></li>
</li>
<li>PCB populated. Note that a 5 pin female header must be installed at the LCD header position.
<p align='center'><img width='50%' alt='PCB-populated.jpg' src='PCB-populated.jpg'/></p></li>
</li>
<li>The connectors on the 4D systems parts are very small. Look at the photo carefully.
The locking tab slides towards the "gen4 Display" logo to unlock.
<p align='center'><img width='50%' alt='LockedVsUnlocked.jpg' src='LockedVsUnlocked.jpg'/></p></li>
<p align='center'><img width='50%' alt='4Dsystems.jpg' src='4Dsystems.jpg'/></p></li>
</li>
<li>When you get the connectors installed correctly and connect it to the programmer and a PC, the
3.2" screen lights up.
<p align='center'><img width='50%' alt='4Dinitialpowerup.jpg' src='4Dinitialpowerup.jpg'/></p></li>
</li>
<li>A FAT formatted uSD card is installed. The primary partition must be no more than 4GB.
Copy the contents of the file WL1244-gen4.zip onto the card.
<p align='center'><img width='50%' alt='microSDinstall.jpg' src='microSDinstall.jpg'/></p></li>
</li>
</ul>

Once the PCB is assembled, here is a photograph of the parts (REV 2 device):
<img src='RC1101-parts-on-desktop.jpg' alt='RC1101-parts-on-desktop.jpg' />


