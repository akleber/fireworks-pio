# LED Fireworks

![LED Fireworks teaser](/fireworks-teaser.gif)

This is the documentation and code of my LED firework build at the end of 2022. The idea came from [this video](https://youtu.be/9oAXtgnxZNs).
The code uses [FastLED](http://fastled.io), started from [Carl Rosendahl](https://www.anirama.com/1000leds/1d-fireworks/) and was then heavily modified and extended by me.
I am using [PlatformIO](https://platformio.org) with Visual Studio Code for development.
For the initial implementation I used [wokwi](https://wokwi.com/projects/349772276028146259) and switched to VS Code for the finetuning with the real hardware.

The LED strings came from AliExpress "50pcs 5V WS2811 Full Color LED IP68 Waterproof" (~13€ each).
The flare (upwards segment) is one string with 50 LEDs. The streaks are 16 LED strings fastened with zip ties to 1m long Aluminium pipes with 6mm diameter (2,15€ each).
There are 18 streaks, so there are 338 LEDs in total. The streaks are all wired in parallel so I have only 66 adressable LEDs: 0-49 for the flare and 50-65 for the flares.

The connections are mostly 3 Pin JST SM connectors. The code runs on a Wemos/Lolin D1 mini.
Power is provided by an unbranded DC 5V 20A universal power supply (~20€).

The mast is made from two 2m wooden sticks and the central streaks "hub" from some leftover wooden block.
The total cost was about 170€.

More info and pictures can be found in the [wiki](https://github.com/akleber/fireworks-pio/wiki)

## Possible improvements

* The 6mm Aluminium pipes could be a bit thinner, maybe 5mm or 4mm.
* The connectors could be waterproof.
