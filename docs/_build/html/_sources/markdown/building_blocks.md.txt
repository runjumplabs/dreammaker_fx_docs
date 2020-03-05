# Building Blocks
------

There are several effect "modules" that can be used and combined in a pedal.  

Each module name has an `fx_` prefix such as `fx_delay` and `fx_phaser`.  A sketch can contain multiple "instances" of a module and multiple modes.

Each module also has a combination of audio nodes and control nodes.  More complete documentation for each module can be found in the cheatsheet, a lighter weight reference, or the full API documentation.  Both of these are available on API Documentation page in the menu above.

Below is a brief summary of the available modules:

 * `fx_adsr_envelope` - An "envelope" or volume control functon that determines how the volume of a signal changes over time after a new note/pluck is detected.  ADSR stands for Attack, Decay, Sustain, Release.  Here's a nice video explaining the basics of ADSRs: https://www.youtube.com/watch?v=JT6rixgu4s4.  The ADSR can also control things other than volume.  For example, it could be connected to a filter to sweep the filter over the incoming instrument signal.
 * `fx_amplitude_modulator` - Varies the volume of the signal over time using a low frequency oscillator (LFO).  Amplitude modulation is what creates the tremelo sound (varying the volume of an instrument signal using a sine wave function).
 * `fx_biquad_filter` - Provides various common types of audio filters such as low-pass, high-pass, band-pass, parametric and several others.  Audio filters can be static (not changing) or dynamic (changing).  The frequency and resonance of a filter can be tied to several different types of control sources such as pot on the pedal, or an oscillator, or the volume of the incoming signal.
 * `fx_compressor` - Provides "dynamic" gain control.  In essence when the volume of the incoming signal exceeds a threshold, the compressor will dynamically adjust the volume down.  It can create long sustaining sounds rather than a pluck followed by a quickly diminishing signal.  Here is a nice video explaining the basics of compression: https://www.youtube.com/embed/8nM5GsNNbyA.
 * `fx_delay` - A basic echo effect with lots of adjustable parameters and a send/receive loop to inject additional effects into the echo loop.
 * `fx_destructor` - A variety of distortion functions including soft, polynomial tube-amp-like distortion, fuzz, bit-crushing, sample-rate crushing, shaping, digital distortion and more.
 * `fx_envelope_tracker` - An input-only module that tracks the volume of the incoming signal.  Its control output can be connected to other modules to create dynamic, volume controlled effects like envelope filters and auto-wahs.
 * `fx_gain` - A basic volume control element.  This element is also useful when mixing between different branches of an effect or turning on/off different aspects of an effect.
 * `fx_looper` - A very flexible looper that supports variable playback rate and reverse playback.  Multiple loopers can be used to create multi-layer loopers.
 * `fx_mixer` - A collection of mixer modules for mixing signals together.  Presently available in 2->1, 3->1 and 4->1 configurations.
 * `fx_octave` - A synth octave generator that creates a synth note at the current frequncy, one octave down, and two octaves down.  Synth mix and synth waveforms are configurable.
 * `fx_oscillator` - A collection of oscillators that can be used either for audio synthesis or control
 * `fx_phase_shifter` - A phase shifter
 * `fx_pitch_shift` - A pitch shifter
 * `fx_ring_mod` - A ring modulator / analog pitch shifter
 * `fx_slicer` - Rhymically sequences incoming audio through different effects.  Great for creating interesting rhytmic effects.
 * `fx_variable_delay` - A super useful building block that can be used to create flange and chorus effects, vibrato, and general modulation.  Fun to use in feedback loops and fx send/receive loops.


