 .. _examples:

*****************
Hear it in action
*****************

Here are a few examples of effects created on the DreamMaker FX Platform.


Perpetuity
##########

Link to effect on DreamMakerFx.com:
https://www.dreammakerfx.com/pedal-designs/Perpetuity

This effect allows you to "catch" a note which will ring out indefinitely.

Hold down the left footswtich to "catch" a note which will ring out indefinitely. Press and hold down the left footswitch to layer on more notes to create a sonic canvas to play over. Tap the right footswitch to release the held notes. When the left footswich is held down to capture notes, the clean channel is muted so you only hear the swell of the capture droning notes. This effect works by running the notes through an ADSR envelope so it quickly fades in and then fades out. This audio is then sent to four delay lines that are staggered so the attack / decay of the note becomes a solid wall of sound.

.. raw:: html

    <iframe width="100%" height="300" scrolling="no" frameborder="no" allow="autoplay" src="https://w.soundcloud.com/player/?url=https%3A//api.soundcloud.com/tracks/760776931&color=%23ff5500&auto_play=false&hide_related=false&show_comments=true&show_user=true&show_reposts=false&show_teaser=true&visual=true"></iframe>

-----

Pentatonic Theramin
###################

Link to effect on DreamMakerFx.com:
https://www.dreammakerfx.com/pedal-designs/Pentatonic-Theramin

This effect doesn't use an instrument at all.  Or rather, you are the instrument.  This effect requires a SparkFun distance sensor connected to the sensor port on the DreamMaker FX (https://www.sparkfun.com/products/14722).

It uses the synth engine along with some gentle effects to create a theramin that operates over the pentatonic scale.  And when there is nothing in front of the sensor, the tones turn off. 

Sensors can be wired to really any effect parameter so it creates a lot of interesting opportunities for new kinds of expression.

.. raw:: html

	<iframe width="560" height="315" src="https://www.youtube.com/embed/Pbnx5vn5iOc" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


-----

Multitudes 
##########

Link to effect on DreamMakerFx.com:
https://www.dreammakerfx.com/pedal-designs/Multitudes

A cool, multi-layer delay effect created by Joe Dougherty.  Consists of a variable length delay pedal where the feedback path of the main delay ("driver") is routed to a secondary delay ("propagator").

.. raw:: html

	<iframe width="100%" height="300" scrolling="no" frameborder="no" allow="autoplay" src="https://w.soundcloud.com/player/?url=https%3A//api.soundcloud.com/tracks/749480944&color=%23ff5500&auto_play=false&hide_related=false&show_comments=true&show_user=true&show_reposts=false&show_teaser=true&visual=true"></iframe>

-----


Stereo Reverb
#############

With the immense amount of processing power and on-board RAM, we can create very rich, intricate reverbs using a few of the building blocks (multi-tap delays, all-pass filters, biquad filters, variable delays, and regular delays).

Here's an example of a stereo reverb that consumes about 10% of the available processing resources on the SHARC DSP.

.. raw:: html


	<iframe width="100%" height="300" scrolling="no" frameborder="no" allow="autoplay" src="https://w.soundcloud.com/player/?url=https%3A//api.soundcloud.com/tracks/766499380&color=%23ff5500&auto_play=false&hide_related=false&show_comments=true&show_user=true&show_reposts=false&show_teaser=true&visual=true"></iframe>

-----

Polyphonic Guitar Synth 
#######################

Link to effect on DreamMakerFx.com:
https://www.dreammakerfx.com/pedal-designs/Polyphonic-guitar-synth-pedal

The DreamMaker FX platform can track multiple notes being played simultaneouly and create various synth effects from these notes.

This is a polyphonic guitar synth meaning that it tracks multiple strings. It uses an FM synth engine along with ADSR envelope generator and an output filter. The pedal is configured by default to use a triangle (OSC_TRIANGLE) wave that is modulated with a sine wave (OSC_SINE). However, lots of interesting sounds can be created by swapping these out with out types of oscillators (e.g. OSC_SQUARE, OSC_RAMP_POS, OSC_RAMP_NEG, OSC_RANDOM, etc).

.. raw:: html

	<iframe width="100%" height="300" scrolling="no" frameborder="no" allow="autoplay" src="https://w.soundcloud.com/player/?url=https%3A//api.soundcloud.com/tracks/756640294&color=%23ff5500&auto_play=false&hide_related=false&show_comments=true&show_user=true&show_reposts=false&show_teaser=true&visual=true"></iframe>