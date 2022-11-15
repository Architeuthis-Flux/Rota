# Rǒta Assembly Instructions

Congrats on your new Rota! Now let's put this thing together.

## Here's what you should have in your kit
Assuming you got the full kit, you should have:

* 1 Rota PCB with assembled SMD parts
    * (If I gave you one with a warning about there being some issues that need to be fixed, [here's the guide to fix them](RotaErrata.jpg))
* 3 [IN-12B](http://www.tube-tester.com/sites/nixie/dat_arch/IN-12A_IN-12B_03.pdf) Nixie Tubes
* 3 [K155ID1](https://tubehobby.com/datasheets/k155id1.pdf) Nixie BCD drive ICs
* 1 [IN-3](https://tubehobby.com/datasheets/in3.pdf) Neon indicator bulb
* 1 [NCH82000HV](https://omnixie.com/products/nch8200hv-nixie-hv-power-module) High Voltage DC-DC converter module
* A Bag containing:
    * 2 printed number plate cards
        * If you ordered a plastic dial, I've also thrown in a slightly smaller number plate card that might look better with them, but you can use either one.
    * 2 plastic number plate covers
    * Some extra plastic spacers for the dial contactors
* Another bag containing:
    * 7 ~2" bits of wire
    * 6 Terminal lugs
    * 2 1x3 Male header pins
    * Some short bits of heat shrink tubing
    * 1 9V battery connector
* 1 Dial insulator
* 1 3D printed case


## Here's what you'll need that isn't included

* Soldering iron
* Solder
* Small-ish flathead screwdriver
* Flush Cut wire cutters
* Small needle-nose pliers
* Something to remove flux (isopropyl alcohol or flux remover or whatever)
* A multimeter (optional, sort of)
* An Xacto blade
* You might need some 1" screws that thread into your dial to attach the case
    * The threads these dials come with have an old thread pitch that's hard to find machine screws for, if you ordered the dial from me, I've recut the threads to fit 4-40 machine screws. They're close enough in pitch and the metal these dials are cast from is soft enough that if you really want to, you could probably just force a 4-40 screw in there and cut new threads that way. 


## Step 0 (checking my work)
The AVR64DD28 microcontrollers were soldered on by hand without the use of a solder paste stencil (the rest of the parts are already populated by JLCPCB so it wouldn't lay flat enough to be helpful), I check each one for shorts an unconnected pins but it's always possible that I missed something. So it might be helpful if you're the meticulous type to put your multimeter in continuity mode and make sure adjacent pins arent shorted. If you find one, I usually just use an Xacto knife blade side up and sort of floss between the pins to cut any tiny solder bridges. 

Or you can trust my work until the end and only do this if there's a problem. 

# Step 1 - Nixie Driver ICs

First, stick in the ICs so the notch aligns with the one silkscreened on the board. 
You'll probably need to bend the pins in a bit to get them to line up with the slightly smaller than usual through holes. 
![ics](RotaDuck/Assembly_Instructions/Pictures/IMG_1691.jpg) 

A thing to remember here is that this board will be laid flat against the dial, so it's important to keep everything on the back side super flush with the board.
I find that pushing them in really snug into the holes, cutting all the pins flush, then letting them come back out a fraction on a millimeter before soldering gives really flat solder joints. 
![ics](RotaDuck/Assembly_Instructions/Pictures/IMG_1696.jpg) 
![ics](RotaDuck/Assembly_Instructions/Pictures/IMG_1699.jpg) 
![ics](RotaDuck/Assembly_Instructions/Pictures/IMG_1701.jpg) 

# Step 2 - Dial Lugs / Battery Holder
Now we need to make the dial lugs that will connect the board to the dial. 

First, grab one of the lugs with your pliers on only one side of the split
![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1677.jpg) 
and squish it most of the way down (leave enough room for the wire.)
![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1679.jpg) 
Now strip off like 1/4" of one end of a wire and stick it in
![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1682.jpg) 
Squish down the other flap so it holds the wire snugly, you might want to press the sides in a bit so it's not so wide and flat.
![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1683.jpg) 
Add some solder
![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1685.jpg) 
and heat shrink it
![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1687.jpg) 

Now repeat this process 4 more times until you have 5 soldered up lugs/wires. (Most kits have the forked style seen at the far left here, these were just extras I had laying around)

![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1702.jpg) 

Cut and strip them so theyre about 1 1/4" long, and stick them in the 5 holes.

To get them to stay while you solder them, I bend them back and tape them down like this
![lugs](RotaDuck/Assembly_Instructions/Pictures/IMG_1704.jpg) 

We still want to keep the back of the board super flush so cut the parts sticking out and then solder them. 

With the battery holder, cut and strip the wires down to about 2 1/2" and solder them in (Red to + , black to -, obviously)

If your board has 6 holes (There's only 1-2 floating around in the wild,) just dont put anything in the 4th hole from the left.

# Step 3 - Nixie Tubes
This part I find is worth being a little picky about looks, you have 3 nixie tube that are matched for overall look as best I can, but once you start paying close attention to each one, you notice that there's a lot of variation from tube to tube. So it's a good idea to put shuffle the order of the tubes until you find an order that looks the most even to you. 

Most of them are a bit off-center in some direction, so part of the matching I do is to make sure if there's one that sort of goes to the left, there will be another that goes to the right. 

Once you have the order down, I set the whole thing face down with something supporting the bottom of the board so it all is flat

![nix](RotaDuck/Assembly_Instructions/Pictures/IMG_1718.jpg) 
And then just solder the top pin of each tube. 
![nix](RotaDuck/Assembly_Instructions/Pictures/IMG_1719.jpg) 

Now you can flip it over and melt the solder while adjusting the positions of the tubes so they're all evenly spaced and at the right angle and stuff. It's worth spending some time here getting it right because they're a huge pain to desolder once they're in.
![nix](RotaDuck/Assembly_Instructions/Pictures/IMG_1721.jpg) 
(These are some *very* unmatched tubes, interestingly, the two on the left have the same date code)

Pro tip: use the outlines on the front of the board as a reference

Once you get it perfect, solder and cut all the pins (don't use your good cutters, these pins are hard and will ruin them)
![nix](RotaDuck/Assembly_Instructions/Pictures/IMG_1722.jpg) 
Getting them super flat isn't important here.

Now make sure to clean the flux off *super duper extra* well here, like more than you'd usually deem necessary. At high voltages flux isn't a perfect insulator and the tiny current it passes is enough to cause other digits to partially come on that shouldn't be and it looks terrible. Like 90% of the troubleshooting I've done on these things ends up being flux related.

# Step 4 - HV Module
You'll notice that there's only 3 holes where the HV module connects to the board. That's because the two ground pins are connected and I needed to save space here. 

The board mounts with the parts facing toward the nixies to get some extra clearance for the 9V battery in the case. 

Find a 1x3 header and break it into individual pins (if you break the plastic part don't worry about it)

![hv](RotaDuck/Assembly_Instructions/Pictures/IMG_1712.jpg) 
![hv](RotaDuck/Assembly_Instructions/Pictures/IMG_1713.jpg) 

The through holes on the Rota board are tight enough that they friction fit pretty well. Put the pins into the Rota board and then put the module on. 

![hv](RotaDuck/Assembly_Instructions/Pictures/IMG_1726.jpg) 

You want to press the board down enough that the big square inductor on the board is as close as possible but not touching the Rota board. And that the evacuation nipple on the nixie tube isn't touching either. 

![hv](RotaDuck/Assembly_Instructions/Pictures/IMG_1727.jpg) 

Cut those pins super flush and then only solder the pins to the HV module. It stays in with friction so I usually wait until I know the board is working to solder it onto the Rota board.

# Step 5 - IN-3 Indicator
Use one of the small heat shrink tubes to measure out a height you want your IN-3 to sit at. 
![in](RotaDuck/Assembly_Instructions/Pictures/IMG_1730.jpg) 
I usually prefer it 1/4" below the face of the nixie tubes. But it's up to you.
Now cut it and the other to that length and put them around the leads of the bulb and shrink it. 

![in](RotaDuck/Assembly_Instructions/Pictures/IMG_1731.jpg) 

Note that there are 2 sides to these bulbs, this is the side you want facing out
![in](RotaDuck/Assembly_Instructions/Pictures/IMG_1728.jpg) 
This is the back side you want facing in towards the board.
![in](RotaDuck/Assembly_Instructions/Pictures/IMG_1729.jpg) 

(You can power these backwards and the glow comes off the wrong parts and looks kinda weird, feel free to try it if you want, it won't damage the bulb.)

Now cross the leads (because the footprint on the board is backwards) and cut the leads to a little over 1/8" 
![in](RotaDuck/Assembly_Instructions/Pictures/IMG_1733.jpg) 

Stick it in the board and solder it from the front. 

![in](RotaDuck/Assembly_Instructions/Pictures/IMG_1735.jpg) 
![in](RotaDuck/Assembly_Instructions/Pictures/IMG_1737.jpg) 

# Step 6 - The Dial

If you got a dial from me, this part should be done already. This is by far the trickiest part of this process. What we need to do is take a dial that originally comes like this 
![dial](RotaDuck/Assembly_Instructions/Pictures/IMG_1984.jpg) 
And move one of those screw teminals from the right to the left side so it looks like this
![dial](RotaDuck/Assembly_Instructions/Pictures/IMG_1663-2.jpg) 

## The Flattening
Depending on the dial you have, some will have a raised screw hole on the left side near where it says "Made In U.S.A." That needs to be sanded flat (shown in the picture above) or just use cutters and clip it off. It's cast zinc so it's surprisingly soft. 

Now the only thing sticking up should be the one on the left. 

## The Insulating

So the dial doesnt short any contacts when it's screwed tight againt the board, there's a piece of cut out gaffer's tape in the kit

![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-16.jpg) 

Use tweezers to peel each half off the backing and do your best to stick it on lined up with the holes (perfect alignment is not crucial)

![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-11.jpg) 
![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-10.jpg) 
![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-05.jpg) 
![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-04.jpg) 

## The Fiddly Bit
I can't step-by step through this part because it's a bit different for each dial depending on age and wear and stuff. but here's the final goal

![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-043.jpg) 

Here's what that looks like on an oscillocsope

The traces go from left to right, 

trace 1 is the pulse (left side, left contact, red above)

trace 2 is pulled to ground through a 10K resistor (left side, middle contact, green above), 

trace 3 is pulled high through a 100ohm resistor (left side, right contact, blue above)

trace 4 is continuity through the right side contacts

![dial](RotaDuck/Assembly_Instructions/Pictures/SDS00004.png) 

So unscrew the the screws holding the contactor arms

![dial](RotaDuck/Assembly_Instructions/Pictures/IMG_1986.jpeg) 

and lay them out in order 

![dial](RotaDuck/Assembly_Instructions/Pictures/IMG_1990.jpeg) 

swap the parts marked with purple arrows and flip the metal pice with the screw terminal around so it faces away from the one next to it when you put them back on.

Make sure each of the contactors on the left side are isolated from one another.

Its helpful to cut those plastic spacers around the screws like a millimeter shorter and fillet the edges so they dont catch on the edges as easily.

Now put all the parts back on the screw and screw it all back on. Watch carefully to see if the contactors are making good contact when they should. 

They probably won't, you're going to need to add and remove spacers (and replace them with the thinner plastic ones I've provided) by trial and error until good contact gets made at the right times. You can bend the arms slightly as needed. Make sure everything's screwed in tight when you test them. 

This is an absolute pain in the ass. But don't give up, once it's set you wont have to do this again. 

Here are some examples of working stackups for reference (one has 2 extra arms on the right side but they're unused)

![dial](RotaDuck/Assembly_Instructions/Pictures/IMG_1345.jpeg) 
![dial](RotaDuck/Assembly_Instructions/Pictures/IMG_1348.jpeg) 
![dial](RotaDuck/Assembly_Instructions/Pictures/IMG_1610.jpg)

At rest
![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-35.jpg) 

Moving the dial
![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-33.jpg) 

Mid pulse
![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-31.jpg) 

You may have to sand some oxidation off the rigid arm on the left side where it meets the middle arm, maybe add a bit of solder. It was wasn't meant to make electrical contact with that arm so sometimes it needs a bit of help.

Here's some Slow-mo video of what it should look like

![dial](RotaDuck/Assembly_Instructions/Pictures/Jumperless-22.mp4) 

If everything was done correctly you should be able to screw the lugs on the board into the terminals on the dial and it should work. This is also a good way to make sure your contactors are set up correctly, if they aren't the nixies will just show 0 and won't count when you dial. 

Now just put in a 9V battery, screw on the case and enjoy your rotary dialing adventures.


## So thats it! Your Rota should be working now!

If you have any issues with assembly, feel free to email me at [KevinC@ppucc.io]( KevinC@ppucc.io) and I'd be happy to help with any problems you may run into.
