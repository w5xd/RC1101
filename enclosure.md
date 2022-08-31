RC-1101 Enclosure
The enclosure is designed with <a href='https://solidworks.com'>SolidWorks</a>, and the CAD folder is
the SolidWorks design. If you're not going to redesign the enclosure, you
don't need SolidWorks nor what's in the CAD folder. The STL folder contains exported shapes for the enclosures,
<i>almost</i> ready to print on a 3D printer. 
Read the notes below.
The enclosure has these 5 parts to print:
<ol>
	<li>the <a href='STL/enclosure2.STL'>enclosure itself</a>. Do <b>not</b> print this STL file without first 
	understanding exactly what your 3D printer is
       	capable of printing. Do not overlook the cavities
	in the design that accept nuts for #4 nuts. Those cavities are very difficult to clean out if your printing
	process fills them with support material. Hints for printing it on low end
	<a href='https://en.wikipedia.org/wiki/Fused_filament_fabrication#Fused_deposition_modeling'>Fused Deposition Modeling</a> (FDM) printers are below.
	<li><a href='STL/enclosure2 LCD back cover.STL'>enclosure2 LCD back cover.STL</a>
	<li><a href='STL/enclosure2 bottom cover.STL'>enclosure2 bottom cover.STL</a>
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
<p>
The <a href='STL/enclosure2.STL'>enclosure2.STL</a> is the shape you end up with. 
Windows 10 has a viewer for stl. Just click the <a href='STL/enclosure2.STL'>link</a> 
to have a look at it the model. 
On an inexpensive 3D printer, gravity won't let it print in that orientation, and I found I had to model some extra 
supports that break away
after printing. The modeled shape to print on an FDM printer, the one with the orientation that accounts for gravity and
has special supports, is <a href='STL/enclosure42.STL'>enclosure42.STL</a>. 
</p>

<p>Printing this enclosure is not a beginner 3D printing project. It takes a total above 20 hours to print the three of them on my 
<a href='http://prusa3d.com'>prusa I3 MK3S</a> printer. </p>

<p>I used the Prusa recommended slicer, <a href='http://slic3r.org'>Slic3r</a>, to slice the model for printing. The slicer parameters <b>are</b> important for 
the biggest part, <a href='STL/enclosure42.STL'>enclosure42.STL</a>. In particular:</p>
<ol>
	<li>Generate support material is <b>on</b>.</li>
	<li>Support on build plate only is <b>on</b>.</li>
	<li>Various versions of the slicer have given different results with respect to support material. In the current version (PrusaSlicer-2.2.0) I have found that a support material overhang threshold at 20 makes no extra supporting material, but omits the left and right edges of the enclosure. Extra supporting material is a problem because it is difficult to remove. But the left and right edges of the enclosure need supports else they wobble during printing and, at best, have uneven surfaces, but at worse collapse and ruin the print. I used the "support enforcer" option in Slic3r to ensure supporting material on those edges.</li>
	<li>I recommend layer height 0.2mm and the standard 0.4mm nozzle. However, I have successfully printed one in half the time using 0.3mm layer height with a 0.6mm nozzle. But the nut mounting holes are not quite printed square enough, plus it took a lot of my time to fit the LCD into its mounting place because the low resolution print doesn't quite match the shape of the LCD. So use a standard 0.4mm nozzle unless you are practiced with 3D printing and are willing to be extra careful mounting the LCD, and after all that want to save a couple of hours on the printer.</li>
</ol>
The <a href='STL/enclosure2 bottom cover.STL'>enclosure2 bottom cover.STL</a> needs to be printed with:
<ol>
	<li>Generate support material <b>on</b></li>
	<li>Support on build plate only is <b>on</b></li>
	<li>The other cover, <a href='STL/enclosure2 LCD back cover.STL'>enclosure2 LCD back cover.STL</a>, may be printed concurrently. It doesn't need support material but the slicer parameters above won't generate any for the back cover.</li>
</ol>
<p>The three items on this page are the main enclosure. The overlays, and the back and bottom covers can
be printed with default Slic3r settings.  
</p>
I used PETG material for prints destined to actually be used for a device. I used PLA for test prints to 
confirm geometry. A video showing how to remove the support material once printed is posted here:
<p align='center'>
<a href="http://www.youtube.com/watch?feature=player_embedded&v=Ue96Qx1N7DY" target="_blank"><img src="http://img.youtube.com/vi/Ue96Qx1N7DY/0.jpg" 
alt="youtube Ue96Qx1N7DY" width="240" height="180" border="10" /></a></p>
<h3>The overlays</h3>
<p>The Trellis keyboard overlays are designed to be printed lettering up with a filament color change anywhere
between Z coordinate above 1.8mm.</p> The text on the overlays comes out far better using a 0.25mm nozzle compared to the standard 0.4mm. I print the overlays with either PLA or PETG filament depending on the color scheme
I want. PETG is the choice when I want the overlay to match the color of its mating enclosure, but PLA is 
easier to deal with, so its a choice for the case you want the overlay printed in a contrasting color.
<h3>Knobs</h3>
All four knobs are for press fit onto a shaft size of 6mm. The 3D printer generally leaves a little debris that can be cleaned out using an undersized drill bit in a bare hand. Rotate the drill bit in the knob's hole while scraping it against all the sides. I used size 15/64 inch.

<h3>Enclosure Cleanup</h3>
<p align='center'><img width="50%" alt='EnclosureBottomCleanup.jpg' src='EnclosureBottomCleanup.jpg'/></p>
The 3D printing process can leave debris that makes it difficult or impossible to assemble. This
is especially true of the mounting of the LCD. Be sure to use a flat blade screwdriver to
scrape any debris off the surfaces indicated in the photo. Also push a #4 machine screw 
through the 4 mouting holes to clear them as well. Wiggle a small hex key in the nut
slots to loosen the debris.

<h3>Final assembly</h3>
The elastomer pad for the <a href='https://learn.adafruit.com/adafruit-trellis-diy-open-source-led-keypad'>trellis</a> is square, but only two of its possible four orientations lines up its feet into the corresponding holes on its PCB. See this illustration. 
<p align='center'><img width="50%" alt="ElastomerOrientation.jpg" src='ElastomerOrientation.jpg'/></p>
