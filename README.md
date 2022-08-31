# RC1101
Remote control box to add knobs and switches to a PC via USB

3D printed in black and controlling an Elecraft K3 remote.
<img width='100%' src='PowerOn.jpg' alt='PowerOn.jpg'/>

This is a build-it-yourself device that WriteLog can use at its control site 
to remote control rigs at your WriteLog-equipped home station. The device can also be used
with WriteLog locally, <i>i.e.</i> at the rig position. (WriteLog restricts your use
of the rig's front panel for the case of RC-1101 control. See the WriteLog documentation.)

Documentation is published here to enable you to:
<ul>
  <li>Fabricate a printed circuit board yourself, or have a service fabricate one for you.</li>
  <li>3D print an enclosure (in three parts.) </li>
  <li>3D print rig-specific keypad overlays</li>
  <li>Parts lists for the remaining, commercially available parts.
  <li>Programming information for its Arduino microprocessor.</li>
  <li>Programming information for its LCD display.</li>
</ul>

<img width='50%' src='BottomView.jpg' alt='BottomView.jpg'/>
The enclosure orientation shown&mdash;left hand side down&mdash; is the best one to use for assembly and 
disassembly because the four #4 machine nuts will be retained in their
slots by gravity. Any other orientation risks them falling out.
3D printed using "transparent" PETG. The Adafruit Trellis connector is disconnected for clarity. 
See <a href='construction.md'>construction.md</a> for details.

<h3>Connections</h3>
The back panel has:
<ul>
  <li>DC power. Positive polarity on the center conductor. 7VDC to 9VDC recommended, which dissipates about 1W
  in the TO-220 cased model 7805 5V regulator IC. That part, properly installed with thermal
    grease to the PCB, will stay below 100C up to about 12VDC. The PCB has a 6A rated crowbar
  diode that attempts to protect this circuit against a reverse polarity power supply input. If
    you connect a 35A DC supply to this input backwards...you probably won't like the result.
  </li>
  <li>USB-C. This goes to your PC. WriteLog has support for the RC-1101 over this USB. 
Other software solutions are possible using the open source firmware and PCB on this website. (see <a href='construction.md'>note</a>
about the Adafruit model 2264 old and new. Still-available older parts have micro-USB instead of USB-C,
but both work in the RC1101.)</li>
  <li>Footswitch label on 1/8" phone jack. There is currently no support for this input.</li>
  <li>Paddles label on 1/8" phone jack. There is currently no support for this input.</li>
</ul>
<h3>Printed circuit board</h3>
The layout for the four-layer PCB was done using <a href='http://expresspcb.com'>expresspcb.com</a>.
The file is <a href='WlRemote.rrb'>here</a>.
There are also gerber files <a href='gerber/'>here</a>. 

<h3>Enclosure</h3>
The enclosure was designed using <a href='http://solidworks.com'>Solidworks</a>. 
The Solidworks files are <a href='CAD/'>here</a>.
You don't need Solidworks to print it. The STL files are enough and are
published <a href='STL/'>here</a>. Details about how to print
an enclosure are <a href='enclosure.md'>here</a>.

<h3>Construction hints</h3>
See <a href='construction.md'>this page</a> for construction details.

<br/>Machine screws. 
<ul>
<li>Quantity 8 of #4 by 1/4 inch pan head screws
<li>Quantity 8 of #4 nuts. (square nuts hold in the channel better, but hex nuts fit.)
</ul>

<h3>The keypad, knobs, and LCD</h3>
The Arduino firmware and LCD programming published here work for all supported rigs in a way 
that is customized for each rig. There are conventions in the firmware that at least encourage,
or in some of these cases require, that all rigs behave the same way for these uses:
<ul>
<li>Each button on the keypad represents some function on the rig. Its LED is on to indicate
the function is on, or off otherwise.</li>
<li>The rig driver <i>may</i> program a button to be momentary such that when you press it,
it only blinks on momentarily, and immediately goes back off. This indicates a toggle of 
some function on the remote rig. This is done, for example, for the SPOT function on the 
K3 and K2. Pressing it once turns on the sidetone and pressing it again turns it off.</li>
<li>The biggest knob, to the right, is generally assigned to VFOA, but a given WriteLog
rig driver can change that.</li>
<li>The three smaller knobs not only turn, but also have a momentary contact switch 
that you access by pressing the knob down. The RC-1101 firmware interprets that momentary
press as a command to switch the knob's mode to a second mode. Pressing the switch does not change the
setting of either of the two underlying features on the remote rig.</li>
<li>By convention, the LCD display is programmed to label the knob function at 
the very bottom of its display. And there are two labels for each knob. The
top label is the knob's primary function (the one it has with remote control
is initiated) and the lower label is the secondary function.</li>
<li>The WriteLog rig driver has the capacity to command the RC-1101 as if
you had turned any of its knobs or pressed any of its switches. When the driver
does so, the RC-1101 changes its display state to match. That is, if the driver
commands a button to be pressed, then the LED lights up, in general.
</li>
<li>The LCD display technology is touch sensitive, however the RC-1101
firmware ignores any touches. You may only change the rig by
pressing its switches (the 16 on the keypad, or the 3 under the 
left-hand knobs) or turning its knobs. </li>
</ul>

<h3>Arduino programming</h3>
The device requires the <a href='https://www.sparkfun.com/products/11113'>SparkFun Pro Mini at 5V</a>.
The Arduino sketch is <a href='sketch/wlRemoteGeneric/'>here</a>.

<h3>Gen4-uLCD-32PT programming</h3>
The <a href='https://www.4dsystems.com.au/product/gen4_uLCD_32PT/'>3.2" LCD screen</a> is programmable. The programming 
information is <a href='4D/'>here</a>. The PCB has a 5 pin header with 0.1" spacing to connect to the LCD. It requires the
use of the adapter shipped with the uLCD that converts the 0.5mm FPC cable to 0.1".

<h3>Other</h3>
<a href='WlRemoteCircuit.pdf'>Here</a> is the circuit diagram.

Should you want to use this device without WriteLog, you're going to need
to have some programming on the PC-side of the USB connection to the
RC-1101. <a href='PC/'>Here</a> is an example of a .NET program 
for Windows that can access the RC-1101. It uses the libraries
available from <a href='http://ftdichip.com'>ftdichip.com</a>.

