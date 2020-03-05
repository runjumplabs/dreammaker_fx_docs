# Buttons, Knobs and Lights
------

The DreamMakerFX platform has a few buttons and knobs on it.  Each of these is completely programmable.

## Configuring the Buttons (aka Footswitches)
------

The buttons can be set up to do the following:
 1. Bypass the entire effect like a typical bypass switch on a pedal
 1. Tap in a tempo or delay lenght
 1. Behave as a momentary effect (i.e. while button is held down, do one thing and when it is release, do something else)
 1. Behave as a toggle for an effect (i.e. tap it once to turn something on, tap it again to turn it off).

**1. Configuring the button as a pedal bypass switch**
------

To configure either the left or right footswtich to become the bypass button for the effect, we use the `pedal.add_bypass_button()` function while defining our pedal routes in `setup()`.  When we _call_ the `pedal.add_bypass_button()` function, we tell it which footswtich to use as the bypass button by using either `FOOTSWITCH_LEFT` or `FOOTSWITCH_RIGHT` as the _argument_.  

Let's revisit our delay function and set it up to use the left footswitch as the bypass button.  If you don't add a bypass switch, the effect will just start running out of the gate.

``` C

void setup() {

  pedal.init();   // Initialize the system 

  // Connect our effect(s) to input and output jacks
  pedal.route_audio(pedal.instr_in, my_echo_1.input);		// Instr in -> echo in
  pedal.route_audio(my_echo_1.output, pedal.amp_out);		// Echo out -> Amp out

  // Left foot switch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  pedal.run();    // Run the effect

}
```


**2. Configuring the button to be a tap delay/tempo button**
------

A tap function allows you to tap the switch at a certain tempo/rate.  The Arduino will lock onto this tap rate and it can be used to update things like the rate of a tremelo and the length/time of an echo effect.

To configure either of the footswitches to be a tap delay/tempo button, we use the `pedal.add_tap_interval_button()` function.  Just like the pedal bypass function, we can use either footswitch as our tap delay/tempo input.  We just can't use the same footswitch that we're using for bypass.  The `pedal.add_tap_interval_button()` takes two arguments.  The first is the foot switch we want to use (either `FOOTSWITCH_LEFT` or `FOOTSWITCH_RIGHT`).  The second argument determines if we want the LED next to the footswitch to flash at the same rate.  Setting the second argument to `true` will enable the LED strobe and setting it `false` will disable the LED strobe.

Let's again revisit the delay function and add the ability to "tap in" a new delay length using the right footswitch.

``` C

void setup() {

  pedal.init();   // Initialize the system 

  // Connect our effect(s) to input and output jacks
  pedal.route_audio(pedal.instr_in, my_echo_1.input);		// Instr in -> echo in
  pedal.route_audio(my_echo_1.output, pedal.amp_out);		// Echo out -> Amp out

  // Left foot switch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is our tap delay lenght input and turn on LED strobe
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  pedal.run();    // Run the effect

}
```

In our `loop()` function, we call `pedal.new_tap_interval()` to determine if the user has tapped in a new tempo and if so, what we want to do about it.  If the user has just tapped in a new tempo/delay lenght, this function will __return__ `true` and if this user has not, it will return `false`.  

We then have two options in terms of how we read the tap interval.  Some effects take a period of time (tyically in milliseconds) as an argument like fx_delay.  Other effects that use an oscillator take a rate (typically cycles per second).  

If we are reading the newly tapped interval for a function that uses time in milliseconds, we use `pedal.get_tap_interval_ms()`.  And if we are reading the newly tapped interval for a function that uses rate in Hertz, we use `pedal.get_tap_freq_hz()`.  

In this case, we are using a delay function so when we detect that we have a new tap interval from the user, we're going to update the length of the delay effect using the tap interval in milliseconds (e.g. using `pedal.get_tap_interval_ms()`).

``` C

loop() {
  // If new delay time has been tapped in, use that
  if (pedal.new_tap_interval()) { 

  	// Update delay length with new tap interval
    my_delay.set_length_ms(pedal.get_tap_interval_ms());
  } 

  // Service pedal
  pedal.service();

}

```

If we also have the option to change the tempo / time of an effect with other means (like a pot), we may want to update the rate the light is flashing.  In this case, we can use
`pedal.set_tap_blink_rate_ms()` and `set_tap_blink_rate_ms()` to set a new blink rate.  First argument is which LED and the second is whether or not to flash the LED.

So with just a few lines of code, we've added the ability to tap in a tempo into the pedal to control the rate of LFOs (flangers, phasers, tremelos, vibratos, etc.) or the length of our delays.
 
**3. Configuring the button to be a momentary switch**
------

Sometimes it's nice to be able to hold a footswitch down to momentarily change the sound of the effect.

Here, we can use the `pedal.button_pressed()` and `pedal.button_released()` functions.  The first argument is the footswitch to watch (either `FOOTSWITCH_LEFT` or `FOOTSWITCH_RIGHT`).  And the second argument is whether to set the LED next to that footswitch while it is being held down.  Similar to the others, this should be either `true` or `false`.

``` C
loop() {

	  if (pedal.button_pressed(FOOTSWITCH_RIGHT, true)) {
	    // Adjust one or more effect parameters when button is held down
	  }
	  if (pedal.button_released(FOOTSWITCH_RIGHT, true)) {
	    // Adjust back one or more effect parameters when button is released
	  }

  // Service pedal
  pedal.service();

}
 ```

 **4. Configuring the button to be a toggle switch**
 ------
 This will be implemented soon! 

 In the mean time, you can use the `pedal.button_pressed()` function like so to behave like a toggle:

``` C
loop() {

	static bool toggle = false;
	static bool first_press = false;

	if (pedal.button_pressed(FOOTSWITCH_RIGHT, false)) {
	  // Is this the first time though where button is pressed?
	  if (first_press) {
	     toggle = !toggle;
	  }
	  first_press = false;
	} else {
	  // Reset so next time button goes low, we can respond on first press
	  first_press = true;
	}

	// Do effect based on toggle state
	if (toggle) {
	  digitalWrite(PIN_FOOTSW_LED_RIGHT, HIGH);
	  // change effect parameter here when enabled
	} else {
	  digitalWrite(PIN_FOOTSW_LED_RIGHT, LOW);
	  // change effect parameter here when not enabled
	}

  // Service pedal
  pedal.service();

}	

```


## Configuring the Knobs (aka Pots)
------
Just like we have functions to detect when a user has pressed a button, we also have a similar function to detect when a user has adjusted one of the knobs (also known as potentiometerss or just "pots").

The fucnctions `pedal.pot_left.has_changed()`, `pedal.pot_center.has_changed()` and `pedal.pot_right.has_changed()` will return true if the corresponding knob / pot has been adjusted.  

We then have two options for reading the value of the pot: `pedal.pot_0.val` and `pedal.pot_0.val_log`.  In both cases, the range of values will be from 0.0 (full left / counter-clock-wise) to 1.0 (full right / clock-wise).  However, the `pedal.pot_0.val_log` function applies a logarithmic curve to the value.  This can be useful when you want a lot of precision at the low-end of the pot and less at the high end (such as setting frequencies).

The value of the pots will always be between 0.0 and 1.0 so in many cases, we'll need to scale these based on what aspect of the effect we are changing.  For example, if want to use a pot to change the frequency range of a filter from 200.0Hz to 800.0Hz, we'll need to add an offset of 200.0 and then multiply by 600.0 (to map 0.0->1.0 to 200.0->800.0).

Let's add some pots to our delay effect to control time / length, feedback, and the wet/dry mix.  Let's also add the ability to tap a delay and update the tap flash rate when the center pot has changed.

``` C
void loop() {

  // If new delay time has been tapped in, use that
  if (pedal.new_tap_interval()) { 
    my_delay.set_length_ms(pedal.get_tap_interval_ms());
  } 

  // Left pot changes the volume of the first loop
  if (pedal.pot_left.has_changed()) {
    my_delay.set_feedback(pedal.pot_left.val);
  }
  
  // Right pot changes the wet / dry mix
  if (pedal.pot_right.has_changed()) {
    my_delay.set_dry_mix(1.0 - pedal.pot_right.val);
    my_delay.set_wet_mix(pedal.pot_right.val);
  }  
  
  // Center pot can also be used to change the delay length 
  // from 100ms to 3000ms
  if (pedal.pot_center.has_changed()) {
    float new_length_ms = 100.0 + pedal.pot_center.val*2900.0;
    my_delay.set_length_ms(new_length_ms);
    pedal.set_tap_blink_rate_ms(new_length_ms);
  }    
  
  // Service pedal
  pedal.service();
}
```


## Turning on and off the Lights (aka LEDs)
------

The two lights (aka LEDs) next to each footswtich can be controlled using the Arduino `digitalWrite()` function.  The first argument is the footswitch LED `PIN_FOOTSW_LED_LEFT` or `PIN_FOOTSW_LED_RIGHT` and the next argument is whether is should be on / `HIGH` or off `LOW`.

To turn on the left LED, we'd do this: `digitalWrite(PIN_FOOTSW_LED_LEFT, HIGH)`.  And to turn off that LED, we'd do this: `digitalWrite(PIN_FOOTSW_LED_LEFT, LOW)`.


