RC-1101 Enclosure
The enclosure is designed with <a href='solidworks.com'>SolidWorks</a>, and the CAD folder is
the SolidWorks design. If you're not going to redesign the enclosure, you
don't need SolidWorks nor what's in the CAD folder. The STL folder contains exported shapes for the enclosures,
<i>almost</i> ready to print on a 3D printer. 
Read the notes below.
The enclosure has these 5 parts to print:
<ol>
	<li>the <a href='STL/enclosure.STL'>enclosure itself</a>. Do <b>not</b> print this STL file without first 
	understanding exactly what your 3D printer is
       	capable of printing. Do not overlook the cavities
	in the design that accept nuts for #4 and #2 machine screws. Those cavities are very difficult to clean out if your printing
	process fills them with support material. Hints for printing it on low end
	<a href='https://en.wikipedia.org/wiki/Fused_filament_fabrication#Fused_deposition_modeling'>Fused Deposition Modeling</a> (FDM) printers are below.
	<li><a href='STL/enclosure LCD back cover.STL'>enclosure LCD back cover.STL</a>
	<li><a href='STL/enclosure bottom cover.STL'>enclosure bottom cover.STL</a>
	<li><a href='STL/BigKnob.STL'>BigKnob.STL</a>
	<li><a href='STL/SmallKnob.STL'>SmallKnob.STL</a> (print quantity three)
</ol>

In the Trellis directory are the designs for the radio-specific
keypad overlays:
<ul>
	<li><a href='STL/Overlay-Generic.STL'>Overlay-Generic</a> keys used when WriteLog has no specific driver
	<li><a href='STL/Overlay-K2.STL'>Overlay-K2</a> for the Elecraft K2
	<li><a href='STL/Overlay-K3.STL'>Overlay-K3</a> for the Elecraft K3
	<li><a href='STL/Overlay-Icom.STL'>Overlay-Icom</a> For Icom radios, about IC-756 and newer.
</ul>
<h3>Enclosure printing hints</h3>
The <a href='STL/enclosure.STL'>enclosure.STL</a> is the resulting shape. Windows 10 has a viewer for stl. Just click the <a href='STL/enclosure.STL'>link</a> 
to have a look at it the model. 
On an inexpensive 3D printer, gravity won't let it print in that orientation, and I found I had to model some extra 
supports that break away
after printing. The modeled shape to print on an FDM printer, the one with the orientation that accounts for gravity and
has special supports, is <a href='STL/enclosure-all.stl'>enclosure-all.stl</a>. That STL file includes not only the enclosure, but also the
LCD back cover and the bottom cover all in an orientation that prints reliably. 

Printing this enclosure is not a beginner 3D printing project. It takes about 22 hours to print the three of them on my 
<a href='http://prusa3d.com'>prusa I3 MK2S</a> printer. 

<p>I used <a href='http://slic3r.org'>Slic3r</a> for the slicer, and the slicer parameters <b>are</b> important. In particular:
<ul>
	<li>Generate support material <b>must</b> be <b>on</b>
	<li>Overhang threshold of <b>45</b> degrees.
	<li>Support on build plate only <b>must</b> be <b>on</b>.
	<li>I used the support material options <code>Rectilinear grid</code> with pattern spacing of 2.9mm.
</ul>
I used PETG material for prints destined to actually be used for a device. I used PLA for test prints to 
confirm geometry. One not-in-the-manual trick I use is to program the first layer
to print cooler: about 60C. It does not adhere as well as it does for the 90C (or higher, 100C is not too high) that
is required for the entire rest of the print. A high build plate temperature minimizes the warping of the shape as it 
prints&mdash;and this model <i>does</i> warp. The lesser adhesion means you have to watch
the first layer and be preparted to stop it and retry if it fails to stick the first time. But that trouble becomes 
worthwhile when the
print finishes and you're trying to pry it off the heat bed. PETG on a clean build plate heated to 100C is rather
difficult to scrape off.
<h3>The overlays</h3>
<p>The Trellis keyboard overlays are designed to be printed lettering up with a filament color change anywhere
between Z coordinate 1.7mm and 1.8mm.</p> I print the overlays with either PLA or PETG filament depending on the color scheme
I want. PETG is the choice when I want the overlay to match the color of its mating enclosure, but PLA is just
easier to deal with, so its a choice for the case you want the overlay printed in a contrasting color.
<h3>Assembly</h3>
<p>
Fully assemble the two printed circuit boards. </p>
<p>Instructions for the Adafruit Trellis are
<a href='https://learn.adafruit.com/adafruit-trellis-diy-open-source-led-keypad/adding-leds'>here</a>.</p>
<p>The custom 3.5 inch by 2.8 inch printed circuit board is all through-hole components. We don't
offer step-by-step kit instructions, but here is an order of assembly that accommodates being able to
place the through-hole components and solder them without them falling out (much):</p>
<ul>
	<li>the two 1/8" mini jacks, J2 and J2.
	<li>Assemble the two 12 pin, .1" centered female connection headers for U2, and their corresponding
       12 pin male connection headers. Don't solder yet.
       <li>Add the two 2 pin headers, both male and female, for the A6/A7 and SDA/SCL pins for U1.
       <li>Solder <i><b>only</b></i> the four corner pins (D1, D9, D10 and RAW) on both the top of the Arduino board
and the bottom of the PCB. double check that all pins are aligned and pushed all the way in. 
Triple check. Its much more difficult to correct errors after this.
<li>Now solder all U2 pins on the top of the Arduino and the bottom of the PCB.
<li>Use a similar procedure for U1. It has 10 pin headers--a female and a male--and has to be soldered
on top of the Adafruit FT232H board and on the bottom of the PCB. Solder the four corners first.
Double check that all the pins are populated and pushed all the way in.
<li>triple check. you won't get another chance. Now solder all the pins for U1.
<li>Install male headers at J5 (5 pins) and J6 (2 pins)
<li>Install the J15 power jack.
<li>Install a right angle male header at enc-1. The pins extend out beyond the edge of the PCB.
<li>U14 has 3 leads that extend through the board and its TO-220 package <b>must</b> be bent over 
with the hold on its tab matching the hole in the PCB. It dissipates about 2W and will 
overheat if not bonded properly to the PCB.
<li>put a very small dot of heat sink grease on U14 to thermally bond it to the matching PCB
area with no solder mask.
<li>Fasten it with a 4-40x1/4" screw and #4 nut. 
<li>All the resistors R1 through R5 are the same value, 4.7K
<li>The power supply has the large diode D13, the polarized capacity C1 and the 5V output C2.
<li>C3, C4, C5, C6, C8 are all .001uF
<li>C7 and C10 are .1uF
<li>Install a 4 pin female connector on the leads from the panel mount optical encoder.
Observe the color designations on the PCB. The black wire goes into pin 1.
Don't shorten the encoder wires. Press their
stripped ends into the connector housing with a small allen key or similar tool.
<li>Solder 4 wires of about 2.5" length and 24 gauge in the J4 holes labeled Trellis.
I use three wires the same color and a distinct color for 5V.  
<li>Assemble the 4 wires from J4 into a 4 pin female header. Maintain the same order as on
the PCB. Pin 1 on the header (use a magnifying glass) is the 5V pin.
<li>Orient the Trellis board with its lettering upright in front of you, and with its bottom
up (with its IC up.)
<li>Solder a 4 pin male SMD header onto the trellis board on the top edge of the Trellis board
(the edge away from you) with the 4 pins spanning the SDA through the 5V pads and nothing
on the (unused) INT pad. The header's male pins are directed back at the board's IC (back
towards you.) Only solder the two end pads first (5V and SDA).
<li>Connect the header on the wires from J4 to the Trellis SMD header to confirm you have
clearance for the connectors to mate. Then finish soldering the SMD header to the trellis.
<li>Use two nylon cable ties to bind the 4 wires together on each of the 4-wire harnesses:
the one to the
Trellis and the one to the panel mount encoder.
<li>Last, install the 3 PCB mount encoders. solder all the pins. There will be significant
force trying to pull these back out of the board in the future if and when it becomes necessary to remove
the knobs pressed on them.
</ul>
