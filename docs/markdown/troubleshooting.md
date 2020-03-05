# General troubleshooting
------

## Issue: DM_FX volume not showing up when plugging pedal into USB port
------

There are a few common reasons why a pedal doesn't show up when plugging it into USB
 * The pedal also needs to be plugged into the wall as it is not USB powered.  Go plug it in.
 * The cable is a USB charging cable and doesn't have any data wires.  Find a different cable.
 * The pedal needs to be placed in bootloader mode.  Place the pedal in bootloader mode per the description below.
 * If you're running Linux or Windows 7, this may be an Arduino driver issue.  Make sure you're running the latest version of the Arduino IDE.


## Issue: SAM-BA operation failed error while downloading an Arduino sketch
------
When clicking the download button, sometimes the download process will stop.  

When this happens, wait for the Arduino IDE to display the message `"SAM-BA operation failed"` which usually happens a few seconds after the download process stops.  Then, put the pedal into bootloader mode as shown below.  You should see the DM_FX drive/volume appear on your computer after you do this.  You should now be able to download without issue.



## Issue: After downloading my sketch, one footswitch LED is on and the other is periodically strobing
------

If the pedal encounters an error while downloading your sketch, it will turn on the left LED and periodically strobe the right LED.  The number of times the right LED strobes in quick succession indicates the issue the pedal encountered.

 * 2 flashes - the pedal encountered an illegal routing combination (e.g. two output nodes connected to an input node).  Try using debug mode as described in the Debugging Sketches article.
 * 3 flashes - the firmware running on the DSP does not match the Arduino package version.  Make sure your firmware is up to date.
 * 5 flashes - the DSP is not running or responding.  Run for the hills.

## Issue: I am getting a "bad CPU type in executable" error when compiling my sketch
------
Upgrade your Arduino tools to version 1.8.10 or later.  This is a known issue running Arduino on OS X Catalina.

## Issue: When building my sketch, error "dreammakerfx.h: No such file or directory"
------

If you encounter the following error while compiling your sketch, it likely means you don't have the right board selected in the Arduino tools.

```
exit status 1
Dreammakerfx.h: No such file or directory
```

First, make sure you have the DreamMakerFX package installed as described in the Installing section.  Then, make sure you have the DreamMakerFX hardware selected.  Go to Tools->Boards and you'll see a number of different Arduino boards listed in the menu.  At the end of the list in the menu, you should see DreamMaker FX (SAMD51/ARM Cortex M4 Core).  Select this and try compiling again.


## Placing the pedal in bootloader mode
------
To place the pedal into bootloader mode, follow this process:

![Placing DreamMaker FX into bootloader mode](https://runjumplabs.github.io/dreammaker_fx/assets/images/bootloader-1.gif)

In rare circumstances, the pedal may not respond to the bootloader sequence.  In this case, remove the back cover of the pedal and press the reset button on the circuit board twice in quick succession while the pedal is powered up.  A small LED near the USB connector should flash a few times and then slowly strobe in and out (like it is breathing).   This will also put the pedal into bootloader mode.  You should see the DM_FX drive/volume appear on your computer after you do this.  You should now be able to download without issue.

