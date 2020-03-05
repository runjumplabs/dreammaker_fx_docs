# The basics of controlling effects
------
We've been talking a lot about getting effects set up and running.  Now let's talk about how to change the proverbial knobs on the effects once they're running.

Now, take a deep breath and get comfortable because this next sentence is important.  There are two ways we can control effects:
 1. we can use other effects that generate control signals (like the envelope tracker) to control the parameters of other effects or...
 1. we can control the parameters directly from our Arduino program.

## Option 1: Using effect control nodes
------
Similar to our audio nodes, all effects have several *control nodes* that are inputs for controlling their individual parameters (like delay length and feedback).  Some effects have *control node* outputs like the envelop filter which are control signals based on the audio going through these effects.

Remember when you read "an envelope tracker is just measuring an audio signal and may just have an audio `input` node but no audio `output` node"?  Well, the envelop tracker has an *audio* `input` node and a *control* `output` node.  Similarly, a synth have a *control* `input` node (like a musical node to play) but have an *audio* `output` node where the synthesized audio is sent.

We can route these control signals just like we do audio signals using the `route_control` function.

For example, let's say we wanted to create a sweet envelope filter.  Essentially what an envelope filter is a bandpass filter that changes frequency based on how loud you are playing.  So what we want to do is take the output of the envelop tracker (which tracks how loud the notes we play are) and send this to the center frequency control parameter of a bandbass filter.

Before we get into building this effect, here's one important detail about how we use the `route_control` function: The `route_control()` function takes two additional values beyond the input and output node: an offset and a scale factor.  Here's what that means.  The envelop tracker will generate a control signal between 0 and 1.0 indicating the current volume of the notes we're playing.  However, we want to sweep our filter from say 600Hz to 1400Hz (typical range of a wah pedal).  So we want to scale our signal that goes from 0 to 1 to one that goes from 600 to 1400.  So we'll use an offset of 600.0 and then the signal by 800.0.  Here's the equation to keep in the back of your brain:

`output = (input x scale_factor) + offset_factor`

So let's create our envelop filter:

``` C
fx_biquad_filter    auto_wah_filter(800.0,               // Initial frequency is
                                    FILTER_WIDTH_NARROW, // Filter is narrow
                                    BIQUAD_TYPE_BPF);    // Filter type is bandpass

fx_envelope_tracker vol_tracker(10,       // 10ms attack 
                                1000);    // 1000ms / 1s release                   

void setup() {
  edal.init();   // Initialize the system

  // input -> filter -> output
  pedal.route_audio(pedal.instr_in, auto_wah_filter.input);
  pedal.route_audio(auto_wah_filter.output, pedal.amp_out);

  // Also send audio in to our envelope tracker to measure the signal
  pedal.route_audio(pedal.instr_in, vol_tracker.input);

  // Finally, route the control signals so the envelope tracker can control
  // the filter with scale and offset values
  pedal.route_control(vol_tracker.envelope, auto_wah_filter.freq, 800.0, 600.0);

  pedal.run();    // Run the effect
}                                 
```

## Option 2: Directly controlling parameters
------
All effects also include dedicated routines for controlling their parameters.  When these routines are called, the effects running on the DSP are immediately updated so these happen in real time.

The `loop()` function is where we can make these modifications.

Now the question is what value would we use to modify these effects.  We happen to have three knobs or "pots" as they're known (short for potentiometer which is a variable resistor).  In our loop function, we can check if these knobs have changed and updated parameters accordingly.

Let's return to our delay effect.  We want our first knob (aka pot0) to control the length of the delay and the second knob to control the feedback.  The pot values vary from 0.0 (all the way left) to 1.0 (all the way right).

```
// Include our library of effects routines
#include "dm_fx.h"

// Create/declare one echo effect and configure it
fx_delay   my_echo_1(1000.0,  // 1 second echo
                     0.7);    // 0.7 feedback ratio

void setup() {

  pedal.init();   // Initialize the system

  // Connect our effect(s) to input and output jacks
  pedal.route_audio(pedal.instr_in, my_echo_1.input);
  pedal.route_audio(my_echo_1.output, pedal.amp_out);

  pedal.run();    // Run the effect

}

void loop() {
  // put your main code here, to run repeatedly:

  // Control delay length with pot0
  if (pedal.pot_left.has_changed()) {
    my_echo_1.set_length_ms(pedal.pot_left.val * 1000.0);
  }

  // Control delay feedback with pot1
  if (pedal.pot_right.has_changed()) {
    my_echo_1.set_length_ms(pedal.pot_right.val);
  }

  // sweet nothings to/from DSP
  pedal.service();

}

```

The next section has more details on how to use the knobs and pots.


## Option 3: Controlling effects with external sensors
------
Where things get really cool is when we begin using sensors and other sources outside the pedal to set parameters.  We could use a motion sensor to control a parameter like so

```
void loop() {
  my_echo_1.set_length_ms(motion_sensor_position);

  // sweet nothings to/from DSP
  pedal.service();
}
```

