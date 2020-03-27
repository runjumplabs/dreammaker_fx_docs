# The basics of routing audio
------
Get ready because we're going to start using the word _node_ a lot.  I hope that's okay.  A node is what it sounds like: it's a *node*.  Or a point of connection.  

## Effect audio nodes
------
Each effect has one or more *nodes* that can pipe audio into it or out of it.  All effects that process audio have both an `input` node and an `output` node.  Things like an envelope tracker that are just measuring an audio signal may just have an audio `input` node but no audio `output` node.  Also, some effects have additional nodes beyond `input` and `output` and this is where shit gets real.  Did you see the movie Inception?  That question will make sense eventually.  

Details on the nodes that each effect has can be found in Appendix A.

## System audio nodes
------
And the system has *nodes* for input from instrument and output to amp.

 * `pedal.instr_in` is the input jack of the pedal.  This might blow your mind, but this is actually an output jack in the sense that it is outputting audio that we can send to the inputs of other effects.

 * `pedal.amp_out` is the output hack of the pedal.  This might blow your mind again, but this is actually an input jack in the sense that it is receiving audio from other effects (and then sending to the amp).

## Connecting nodes
------
 As we just saw in the echo example, there is a function called `route_audio` that we use to connect our effects to the input and output jacks of the pedal and also to each other.  The first *argument* of this function is an output node and the second *argument* is an input node.

 Let's use it in a (programming) sentence.  In this example, we're going to have a tremelo that then feeds into a delay.  It'll be like having your guitar plugged into a tremelo pedal that then plugs into a delay pedal that then plugs into your amp.  

 (*note: if it's not yet obvious, you can call each effect you create just about whatever you want*).

``` C
// Create objects for these effects
fx_amplitude_mod  happy_tremelo(1.0, 0.5);      // 1Hz rate, 0.5 depth
fx_delay          sweet_baby_echo(1000.0, 0.7); // 1000ms, 0.7 feedback  


void setup() {
  pedal.init();   // Initialize the system

  // Route tremelo through echo/delay effect
  pedal.route_audio(pedal.instr_in, happy_tremelo.input);
  pedal.route_audio(happy_tremelo.output, sweet_baby_echo.input);
  pedal.route_audio(sweet_baby_echo.output, pedal.amp_out);

  pedal.run();    // Run the effect
}

```

Or let's get more crazy.  Let's say we have a delay pedal and each time through the delay, we're going to pitch shift up.  So it would sound like this: ECHO Echo echo echo (where each time you say 'echo' you say it in a lower pitch voice).

The fx_delay has two additional nodes called `fx_send` and `fx_receive`.  We're going to run these through our handy-dandy pitch shifter.  For this, we're going to use the more advanced delay setup function that allows us to pass a few additional parameters (more info in Appendix A on this):

``` C
// Create objects for these effects
fx_delay          echoey_snail(1000.0,     // Delay length: 1000ms
                              1000.0,     // Max delay length: 1000ms
                              0.7,        // Feedback: 0.7
                              1.0,        // Feedthrough: 1.0
                              true);      // Enable delay fx loop
fx_pitch_shift    shift_down(0.85);       // Pitch shift down 0.85 x current pitch


void setup() {
  pedal.init();   // Initialize the system

  // input -> delay -> output
  pedal.route_audio(pedal.instr_in, echoey_snail.input);
  pedal.route_audio(echoey_snail.output, pedal.amp_out);

  // Now patch in pitch shifter into delay fx loop
  pedal.route_audio(echoey_snail.fx_send, shift_down.input);
  pedal.route_audio(shift_down.output, echoey_snail.fx_receive);

  pedal.run();    // Run the effect
}

```

Pretty cool, right?

## A few routing rules
------
Obey these rules to avoid humiliation and sadness:

An output node can be routed to multiple input nodes
``` C
pedal.route_audio(pedal.instr_in, delay_1.input);
pedal.route_audio(pedal.instr_in, delay_2.input);
```
An input node can only have one input.  However, you can use the `fx_mixer` nodes if you want to send multiple outputs to an input.
``` C
pedal.route_audio(delay_1.output, my_mixer_2.input_1);
pedal.route_audio(delay_2.output, my_mixer_2.input_2);
pedal.route_audio(my_mixer_2, pedal.amp_out);
```
And you can't route input nodes to other input nodes, or output nodes to other output nodes.  It's always output->input.
