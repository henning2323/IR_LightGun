# IR_LightGun
<img src="/pictures/1csffls9rpl51.webp" width="500"/>

## Introduction

Two years ago on vacation I played a light gun game in an arcade for the first time (Rambo arcade by SEGA) and I was amazed!
I always imagined that the virtual shots just hit somewhere in the general direction you are aiming and that the gameplay was more ore less spray and pray.
But to my surprise the gameplay was fast, snappy, and controlled. As soon as I got home, I wanted to get my hands on such a thing.
But after checking the prices for such a machine (and of course the practicallity) I quickly decided to go with emulators on my PC.
Since there were no proper options on the market and I happen to study Electrical Engineering I looked for DIY Options.

A wide spread option (or at least as wide as the lightgun DIY community goes) were IR Camera Modules paired with sensor bars from the Wii.
With a video series on YouTube from a channel called "SAMCO" I got my first version up and running.
With this setup you needed to calibrate the lightgun so it knows where the screen is, relative to the Wii sensor bar, which the camera can track.
The problem for me was that this calibration was tedious, inconsistant and you had to do it as soon as you moved a little.
You always had to adjust your aim by a few centimeters, because the shots did not land precisely wher you aimed.
Also the input delay was too high so the shots felt like they traced the direction you were pointing.

A year later I sat down and developed my own code with the following features:
## Features:
- No calibration
- No software on the Computer needed, the lightgun just works as a plug and play usb device
- pretty much perfect acuracy so you can even turn off ingame crosshairs in slower paced games
- Extreme viewing angles
- decreasd input delay

## Parts List:
- **Arduino Pro Micro 5V/16Mhz (ca. 5€)**  
	Arduino Leonardo would also work, but its too large and more expensive
- **IR Positioning Camera**  (20 - 25€)
	I got [this one](https://www.dfrobot.com/product-1088.html) from frome DFRobot, but I think I saw the same one on mutltiple Websites. It does all the heavy lifting (image processing) internally and directly provides the coordinates of up to 4 tracked IR sources via I2C. Also in my case it perfectly fitted into the end of the broken nerf gun I used.
- **A long USB cable**
	The Camera needs to see the entire screen, so you have to keep a minimum distance to the screen. In the case of a 22" screen, its around 2 meters. The distance scales linearly with monitor size so take a cablelength that fits to your screen. Also better plan in some tolerance.
- **Some push buttons**  
	Any form of a simple push button (button connects the terminals when pushed / if not pushed -> no connection) will work here. They can later be mapped to mouse buttons od keyboard keys. In my nerf gun I could use the already existing switches. I can use the trigger, the small second trigger, inserting a magazine and sliding back the lid on top.
- **An LED + matching Resistor (optional)**  
	In one of the lightguns I included a bright white/blue LED under the panels in the front which flashes when the trigger is pulled. You don't notice it while playing but it looks cool for everyone watching.
	You also need a resistor if you want to make guarantee that the LED stays alive for longer than a second. For calculating its resistance value you need the forward voltage and the recomended/maximum forward current of your LED. It is either supplied by the manufacturer or you can find esitmates by color.
	Also the Pro Micro comes in two versions with different voltages and operating speeds(3.3V/8MHz and 5V/16Mhz). I would recommend getting the 5V version since it faster and should result in less delay, but both should work. You calculate the resistance with this formula:  
	```math
	R = \frac{Operating Voltage - LED Forward Voltage}{ForwardCurrent}
	```
	If you dont want to calculate anything, just try some values between 100 Ohm and 1 kOhm and choose what brightness looks right for you.
- **4 small IR Sources**  
	These IR Sources need to be placed at the corners of your screen. I use the IR LEDs I salvaged from the Wii Sensor bars I used before, but anything would work. A nice brightness and a large viewing angle are desireable. Multiple LEDs close to each other will be picked up as one source by the sensor which can be very helpful. In theory even 4 candles with small flames placed at the screens corners would work, but I don't think this is the most practical solution.
	By the way you can check if your IR LEDs are on and their brightness by looking at them with your phones camera. You can try it by watching a remote controls LED flicker when you press a button.  
	
## Wiring
The Wiring is very simple. The IR Camera needs to be powered with 3.3V or 5V and its two communication lines need to be connected to the Microcontrollers I2C Pins. The yellow wire of the camera is the SDA line and connects to pin 2; the green wire  is the SCL line and connects to pin 3.
All the buttons can be connected between any free digital pin and ground. If you chose to include an LED you can also connect it to a free pin. Depending on the bootloader you use (which board you choose in the Arduino IDE) to flash your software onto the arduinos memory the pin numbers in the software may not correspond to the numbers printed on the board. If thats the case, just try which number adresses wich pin until you found enough for all your buttons. This is not true for SDA and SCL! They are most defiently where 2 and 3 is printed on your board.

<img src="/pictures/wiring.svg" width="500"/>