# IR_LightGun
<img src="/pictures/1csffls9rpl51.webp" width="500"/>

## Introduction

Two years ago on vacation I played a lightgun game in an arcade for the first time (Rambo arcade by SEGA) and I was amazed!
I always imagined that the virtual shots just hit somewhere in the general direction you are aiming, and that the gameplay was more or less spray and pray.
But to my surprise the gameplay was fast, snappy, and controlled. As soon as I got home, I wanted to get my hands on such a thing.
But after checking the prices for such a machine (and of course the practicality) I quickly decided to go with emulators on my PC.
Since there were no proper options on the market and I happen to study Electrical Engineering, I looked for DIY Options.

A widespread option (or at least as wide as the lightgun DIY community goes) were IR Camera Modules paired with sensor bars from the Wii.
With a video series on YouTube from a channel called "SAMCO" I got my first version up and running.
With this setup you needed to calibrate the lightgun, so it knows where the screen is, relative to the Wii sensor bar, which the camera can track.
The problem for me was that this calibration was tedious, inconsistent and you had to do it as soon as you moved a little.
You always had to adjust your aim by a few centimeters, because the shots did not land precisely where you aimed.
Also, the input delay was too high, so the shots felt like they traced the direction you were pointing.

A year later, I sat down and developed my own code with the following features:
## Features:
- No calibration
- No software on the Computer needed, the lightgun just works as a plug and play USB device
- pretty much perfect accuracy, so you can even turn off in game crosshairs in slower paced games
- Extreme viewing angles
- Decreased input delay

## Parts List:
- **Arduino Pro Micro 5V/16Mhz (ca. 5€)**  
	Arduino Leonardo would also work, but it's too large and more expensive
- **IR Positioning Camera**  (20 - 25€)
	I got [this one](https://www.dfrobot.com/product-1088.html) from DFRobot, but I think I saw the same one on multiple Websites. It does all the heavy lifting (image processing) internally and directly provides the coordinates of up to 4 tracked IR sources via I2C. Also in my case it perfectly fits into the end of the broken nerf gun I used.
- **A long USB cable**
	The Camera needs to see the entire screen, so you have to keep a minimum distance to the screen. In the case of a 22" screen, it's around 2 meters. The distance scales linearly with monitor size, so take a cable length that fits to your screen. Also, better plan in some tolerance and make sure the cable is not meant for charging only.
- **Some push buttons**  
	Any form of a simple push button (button connects the terminals when pushed / if not pushed → no connection) will work here. They can later be mapped to mouse buttons and keyboard keys. In my nerf gun, I could use the already existing switches. I can use the trigger, the small second trigger, inserting a magazine and sliding back the lid on top.
- **An LED + matching Resistor (optional)**  
	In one of the lightguns I included a bright white/blue LED under the panels in the front which flashes when the trigger is pulled. You don't notice it while playing, but it looks cool for everyone watching.
	You also need a resistor if you want to make guarantee that the LED stays alive for longer than a second. For calculating its resistance value, you need the forward voltage and the recommended/maximum forward current of your LED. It is either supplied by the manufacturer or you can find estimates by color.
	Also, the Pro Micro comes in two versions with different voltages and operating speeds(3.3V/8MHz and 5V/16Mhz). I would recommend getting the 5V version since it faster and should result in less delay, but both should work. You calculate the resistance with this formula:  
	```math
	R = \frac{Operating Voltage - LED Forward Voltage}{ForwardCurrent}
	```
	If you don't want to calculate anything, just try some values between 100 Ohm and 1 kOhm and choose what brightness looks right for you.
	I also added an LED that lights up when the lightgun does not see all 4 IR Sources, but I never looked at it. I only remembered that it's even there because I now read through my code again.
- **4 small IR Sources**  
	These IR Sources need to be placed at the corners of your screen. I use the IR LEDs I salvaged from the Wii Sensor bars I used before, but anything would work. A nice brightness and a large viewing angle are desirable. Multiple LEDs close to each other will be picked up as one source by the sensor, which can be very helpful. In theory, even 4 candles with small flames placed at the screen's corners would work, but I don't think this is the most practical solution.
	By the way, you can check if your IR LEDs are on and their brightness by looking at them with your phones' camera. You can try it by watching a remote control's LED flicker when you press a button.  
	
## Wiring
The Wiring is very simple. The IR Camera needs to be powered with 3.3V or 5V and its two communication lines need to be connected to the Microcontrollers I2C Pins. The yellow wire of the camera is the SDA line and connects to pin 2; the green wire  is the SCL line and connects to pin 3.
All the buttons can be connected between any free digital pin and ground. If you chose to include and LED you can also connect it to a free pin. Depending on the bootloader you use (which board you choose in the Arduino IDE) to flash your software onto the Arduino memory, the pin numbers in the software may not correspond to the numbers printed on the board. If that's the case, just try which number addresses which pin until you found enough for all your buttons. This is not true for SDA and SCL! They are most defiantly where 2 and 3 are printed on your board.

<img src="/pictures/wiring.svg"  width="500"/>
This image shows how the camera needs to be connected (4 wires to the left) and an example of a pushbutton and a LED.

## Code
In the Code, there are two parts where you need to edit something. First of all in line 8 to 13 you need to change the numbers to the ones where you connected a button or an LED.
```
#define triggerA 2
#define triggerB 3
#define mag 9
#define lid 8
#define fireLED 4
#define warningLED 15
```
Notice that I used the pins called 2 and 3 for the trigger buttons, but they don't correspond to 2 and 3 on the Board because these pins are already used by the I2C interface.

The second part which needs your attention is the function 'handleButtons()' which starts in line 67.
Here, you need to tell the code which mouse button or key should get pressed when you press any buttons on your lightgun.
Just change the two lines where it says 
```
AbsMouse.press(MOUSE_MIDDLE);
```
or
```
Keyboard.press(KEY_RETURN);
```


After that, your code should be ready. In the Arduino IDE, you need to choose the COM-Port where your board is connected. For the board selection you can either try to find a 3rd party board library for the Pro Micro itself, or you just use the one provided for the Arduino Leonardo. They use the same chip.

The lightgun should now work, but because there is some distance between the IR camera and the sights you are using, the mouse/crosshair is probably not perfectly centered.
To 'zero' it properly, you can adjust the values in line 39 and 40 by some steps until your mouse is spot on.
```
int gunCenterX = 595;
int gunCenterY = 325;
```

## Using the lightgun
There is not much left to do. Just turn on your IR Sources, plug in the USB Cable of your lightgun, and you should be good to go. Especially games which can accept the raw mouse input and run on Windows natively are excellent! And for all the emulators you just use the usual tools/plugins like nuvee or demulshooter, that you would also need if you were playing with a mouse. Since these plugins interrupt the signal of the device, you can use multiple lightguns of this type without any problems.

I would say the weakest link in the chain are your IR Sources. Most IR LEDs are quite directional and are not visible after around 45°. That's also why the Wii Sensor bar puts three differently angled LEDs next to each other. My cheap solution to increase the performance is that I shorted one of the three LEDs I got from the sensor bar and the other ones are now running with too much current. That means they are super bright, but they get quite warm and probably won't live for more than a few years.
I also heard that sanding the tips of the LEDs flat can work wonders if you find yourself having tracking issues. That acts as a diffuser and the light leaves in multiple directions. If you find any already diffused IR Bulbs would probably be the best, but I never had the need to upgrade from my cheap solution which works perfect. As said before, you can always check if your LEDs are visible by using your phones' camera.

## Future upgrades ##
There are two things which I would still like to add sometimes. The first one is a front grip with a DPad or analog Stick for the few games where you can use directional input. I want to do this either with a detachable grip which attaches to the rail on the bottom, or I just mount it directly to the nerf blaster. The compartment in the front where the large batteries used to be is completely empty, and that's also the exact spot where my hands usually rest.

The second addition would be some changes in the code and maybe in the IR Setup so the lightgun knows where it's pointing even when It's so far off the screen, that it does not see all four IR Markers anymore. The result would be that you could come way closer to the screen if you want to. But I'm not sure yet if there is a robust way to achieve that without compromising on accuracy or needing a more complex sensor.


I hope you found this useful and were able to get everything working for you. If you have any questions to how it works or need help with the setup, feel free to ask.