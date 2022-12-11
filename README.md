# Rǒta
Here are some different Rǒtas with different options
![IMG_1778 2](https://user-images.githubusercontent.com/20519442/201711569-40ef605c-c12b-4c56-9fea-919bc490b09b.jpeg)

![IMG_0975b copttty](https://user-images.githubusercontent.com/20519442/201712627-650df17a-27e6-4c06-91d1-4a762fd1d08d.png)

![IMG_1744 2](https://user-images.githubusercontent.com/20519442/201712909-99d46b61-f74d-4dfa-82de-9feb03b750d8.jpg)

![1_0_GIF_2 4](https://user-images.githubusercontent.com/20519442/197623333-b9eae115-00ea-4ea8-91c2-5675926cccdb.GIF)


That flashing means it wants your input. These decimal points do way more work than regular decimal points, they tell you a lot about what's going on. When you don't have segments to make crude letters and stuff, you need to get creative with dots.



https://user-images.githubusercontent.com/20519442/197625332-2014cbfb-5107-4166-b01f-a8d20bc4a214.mp4

![IMG_1754](https://user-images.githubusercontent.com/20519442/201710996-1dda6a10-2676-4cdb-be2f-f7fb13220ca6.jpeg)


Rǒta is a tool for counting things while being all nerdily extra about it. Are there more efficient ways to count things? Yes. But will those things increase your exposure to Nixie tubes and rotary dials? No? Well then if that's what passes for progress nowadays then I'm going to go live on a mountain somewhere and count things the cooler way, with nixie tubes.

![IMG_0975b](https://user-images.githubusercontent.com/20519442/197626143-0b8da9ae-dbe0-46c3-8e78-2c965b2f44d0.jpeg)


It was originally conceived as a gift for a duck hunting lodge, but now I'm making a few more and I know not everyone is into hunting so it's been generalized into a tool for just counting stuff in general where those $3 hand clickers or a golf pencil just lacks the glamour you need in your life. 

Instead of using sequences of numbers to use the special functions, now you just dial 0 and then the function you want which is printed on the number plate. 

here's the special function menu map
```
note - I'm calling moving the dial just a little to turn the display on a "click"
when inside a special function, dial anything to exit (preferably 0 because it's only reading intermittently when in menus, so it's most likely to register)

0 -- enter/exit menu or function   shows 111 when you're in the menu)
1 -- stopwatch                    click to start, click again to see last time.
2 -- show battery voltage         briefly turns off the display to get a more accurate reading (not under load)
3 -- new day                      this stores the total since the last time you started a new day. maximum 63 days
4 -- does nothing
5 -- enter a count by digits      enter 3 digits (hundreds - tens - ones) to add to your daily total
6 -- show history                 click through each day with first the day number (on the left) then the total for that day
7 -- toggle day/total display     either shows the count for that day or the running total of all days (default is total)
8 -- golf mode                    keep score for golf - first enter 1-4 players and the decimal points show which player's turn it is
9 -- clear everything             dial 9 again to confirm. dial anything else to go back
999999 -- emergency clear         dial 9 six times to emergency clear everything in case something gets messed up same as dialing 10 9 9
                                  you can also short the middle pulse pin high for super duper emergency clear

```




The voltage display was added because I was having issues with the K155ID1s browning out before the HV module (it still blows my mind that it works down to like 3V), so the display would just show all the digits on because the nixie drivers weren't blocking any current. 

So now I added a HV shutdown and a super simple voltage sense divider that drops the 9 down by half so it can be read by the AVR64DD32's ADC.


Now if the battery voltage is too low, it shuts down the display but still takes input. In case you don't have a spare battery but still want to count. And the power draw is so low for just the microcontroller that the battery life might as well be infinite.



https://user-images.githubusercontent.com/20519442/197626419-59003e0c-a97b-495f-a466-c8ea541df162.mp4


If you notice in the video, the display flashes when the battery voltage updates, that's because when it does the measurement, it briefly shuts the HV module down so we can measure the battery voltage unloaded. Because this thing draws ~300-400mA when it's on so without doing a ton of math and testing different 9v batteries to correct for it, the voltage it shows would be like ~2V lower than it really is unloaded. 

I probably should have put in a fresher battery before I filmed this. 

I also had to design a case for this because I'd always get anxious watching other people use it without one. People with smaller hands tend to put their fingers right on the high voltage section on the back of the Nixies. 
![IMG_0936](https://user-images.githubusercontent.com/20519442/197626502-5986a3f2-9786-407d-a474-73ecfe45962d.jpeg)


This project really helped me change my lukewarm feelings towards rapid prototyping, I've had a 3D printer for years but never really used it much. I found out the reason it never worked is the original spool of filament Sindoh included was garbage. So my prints would always turn into an absolute mess of spaghetti. I finally bought a fresh spool of filament and it works great now.

![IMG_0962](https://user-images.githubusercontent.com/20519442/197626542-a9db3e1a-2bc0-4cbc-95a3-32775121c8e2.jpeg)

I'll definitely have the case made by someone with a more expensive printer when I sell these or give them away, but to get the fit right and make sure there's room for everything inside, 3D printing really saved the day here.

![RotaNumberPlate](https://user-images.githubusercontent.com/20519442/197626724-35d7f85a-14b1-49e0-a744-9d2dded9bd13.png)


Putting in the number plate (which I just found out the name of) on these old dials really makes you appreciate how much thought they put into things in the 60's. Like on most phones you would only do this once, but they came up with a really cool system for it.


How you do it is by sticking a screwdriver under the ring at the 6 and moving a lever down to the 8 position to release the middle thing.

![IMG_0960](https://user-images.githubusercontent.com/20519442/197626811-78fd25a2-0aa9-4d9c-81ab-b55d80493cb2.jpeg)
![IMG_0959](https://user-images.githubusercontent.com/20519442/197626831-53a42e87-7145-46e3-b410-23b491b92b19.jpeg)


With it off you can see what's going on 

![IMG_0954](https://user-images.githubusercontent.com/20519442/197626920-7cd2f53e-ab70-4588-8e06-f6d29e00dc04.jpeg)


Then you just twist this pressure plate and it comes off. 
![IMG_0957](https://user-images.githubusercontent.com/20519442/197626963-8bfaba2a-ac35-4c27-b012-99e369f1feb4.jpeg)
![IMG_0955](https://user-images.githubusercontent.com/20519442/197626990-b4835aca-b19f-479d-8dc6-5159aebf505b.jpeg)


This isn't the final number plate from above because I'm a millennial and don't have a regular 2D printer. But I might try it on a plotter or a silkscreen to get a nicer finish.

I had to move the holes a bit to get it to line up with the dial, I must have carelessly moved the holes on the Edge Cuts layer in Kicad, but the new Kicad files on here are fixed (as well as a couple other problems) if you want to have these boards made yourself. 

![IMG_0926](https://user-images.githubusercontent.com/20519442/197627114-9e1a494a-ba9a-4e3c-abc2-25c89c4ac7bd.jpeg)

Yes I cleaned that flux off. For Nixies it's particularly important because flux lets enough current flow at that voltage to cause ghosting. I had a lot of problems wit ghosting on the first digit and I tried a bunch of things to get rid of it, eventually what fixed it for me was putting a 220KΩ resistor between high voltage and ground to both drop the voltage a bit (these tiny HV modules don't have an adjustment knob like the bigger ones) and bleed off the extra charge you get when you do low-side switching on nixies.

![IMG_0995](https://user-images.githubusercontent.com/20519442/197627218-c870993c-c2f6-48cf-9c1c-42e6c873cc29.jpeg)



PCBway was nice enough to sponsor this project. I had never used them before but the boards they sent were great. And their matte black finish is really velvety. 


It really seems like they spend a good deal of time with a human looking at your boards and making sure there aren't any problems. And I usually do a lot of weird stuff with my boards that falls outside of the usual best practices of PCB design. But they seemed to roll with it pretty well and make sure I did it on purpose (that came up a lot more with breadWare which they also sponsored.) Which is crazy to me because of how cheap the service is. You'd think it wouldn't be worth their time for my ~$20. 

Either way, I was ready to just pay them for my boards if they were shitty and say so here. But that wasn't necessary because they really did a great job on these. 


# RotaDuck (this is the previous revision so I'm leaving this here)
Sticking Nixie tubes to an old rotary telephone dial

Here's a video of it in action


https://user-images.githubusercontent.com/20519442/176002505-34b7ac66-cf38-471d-8984-42ffa85f04d7.mp4

And there's a few more on my [Youtube channel](https://www.youtube.com/user/arabidsquid/videos)


![IMG_0791](https://user-images.githubusercontent.com/20519442/176006940-7bcd5dae-7f92-44ec-84f8-035180d775ef.jpeg)

I'll do a more complete write-up at some point in the future, but for now I'll mention a few things.

It stores the running count (and a history of recent entries) in EEPROM because when the dial returns to it's resting position, the power is completely cut off. That makes it so I couldn't just use timing to figure out when the pulses stop (and the number that was ultimately dialed) so instead I put in a resistor ladder so the between the 3 contacts on the dial to give me 1/2 voltage on the last number, when the dial is between 0 and the end. that gives me a waveform like this:
```
   _   _   _   _
__| |_| |_| |_| |_,-,___
```
So it does all the saving during that shorter last pulse, which is pretty tight because EEPROM access is relatively slow. 


![IMG_0793](https://user-images.githubusercontent.com/20519442/176012606-1b5eee9d-8f97-465f-9617-c8cdb3c5b7ba.jpeg)

I didn't want to add any buttons to it so instead it uses multiple 9s and 0s for special codes, here's the list of what they do
```
0   - subtract the next number dialed from the total
000 - clear all counts to 0

9   - show total for all days
99  - list each day and the total for each
999 - start a new day
```
Here's what I'm talking about when I say days. So this was originally designed for duck hunting (hence the duck in the name) as a gift for a duck hunting club. If you want to count how many ducks you got, we'd usually bring one of those mechanical clicker counters and just keep a running total.But I figured if I'm making a thing to count how many ducks were shot during a trip, it might also be useful to have that information separated by days if you want to keep a separate daily and running total. 

At some point I'll print that menu in the middle of the dial.


![IMG_0803](https://user-images.githubusercontent.com/20519442/176012668-41f7fd30-4e87-41d0-9931-8e19932bbfa6.jpeg)


If you intend to make one of these for yourself, you should probably know this thing is like 90% bodges and perfboard circuits. I eyeballed the shape of the dial cutout nearly perfect, except that it's flipped on the board. So instead of just redoing the board like a sane person, I just mounted it with everything on the wrong side and bodged wired a ton of things to make it work. That's why there are those arrays in the code to convert the number shown to the number its actually wired to. So if you really want one, let me know and I can do a version 2 that will be 100x easier to put together. 

![IMG_0790](https://user-images.githubusercontent.com/20519442/176012737-33e297ab-1a3a-4ecb-9f2f-9e824154581a.jpeg)
![IMG_0802](https://user-images.githubusercontent.com/20519442/176012765-f1a702be-1683-44eb-a802-ccefb670e9be.jpeg)


I also made a rechargeable one by dead bug soldering a tiny 10DFN MCP73213T charge controller and putting in 2 1/2 AA sized lithium batteries, but after using it a while, the simplicity of just using a 9 volt battery is much nicer (and lasts a lot longer.)

![IMG_0795](https://user-images.githubusercontent.com/20519442/176012807-0eb6a247-c3b7-4dcf-8e81-acc7c615ea75.jpeg)
Eventually that red wire sticking out will be connected to a barrel jack port. Once I decide on an enclosure.
![IMG_0796](https://user-images.githubusercontent.com/20519442/176013627-a2ab8784-ac51-44e4-bc0c-d1b09885f908.jpeg)

![oshw_facts (1)](https://user-images.githubusercontent.com/20519442/206933172-6082a8b9-86c0-4ce3-bffa-d38b760f660f.png)

