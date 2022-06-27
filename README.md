# RotaDuck
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

