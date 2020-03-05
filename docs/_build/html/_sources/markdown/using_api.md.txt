### Using the API

There is an ever-growing list of effects and synth objects that can be routed and controlled in very interesting and novel ways.  These building blocks can be routed together and novel ways.  And, some building blocks and control the parameters of other building blocks.

 * `fx_amplitude_mod` - Amplitude modulation continuously changes the volume of the audio running through it and can be used to create tremelo effects and more advanced gating effects.
 * `fx_biquad_filter` - A basic audio filter that filters out certain frequency ranges of audio.  This can be configured as a low-pass, high-pass, band-pass filter, etc.  This block can be  used for general EQ, wah pedals, auto-wahs and envelope filters.  
 * `fx_clipper` - Provides soft and hard clipping functions that can be used to create a wide variety of distortions.  When combined with the `fx_biquad_filter`, a wide range of tones can be realized to recreate the sounds of tube-amps and distortion pedals.
 * `fx_compressor` - A compressor/limiter block that provides dynamic volume control.  This can be used to create a longer *sustain* effect on guitars and basses.  It can also be used to keep our audio signals within range so we don't end up with output distortion.
 * `fx_delay` - Used to create delay-based effects like echoes and reverbs.
 * `fx_envelope_tracker` - Tracks the volume of the input signal and can be used to control other effects like `fx_biquad_filter` for creating an auto-wah / envelope filter.
 * `gain` - Increases or decreases the volume of the audio signal.
 * `fx_instr_synth` - Basically a guitar/bass synth.  Generates synth tones based on the note that is being played.  Follows string bends too.  This will be vastly enhanced in next version of API to support polyphonic note tracking.
 * `fx_looper` - A looper effect can capture a sample of audio and loop it indefinitely.  
 * `fx_mixer` - Provides a variety of mixers to mix multiple effect outputs into a single signal.
 * `fx_oscillator` - Various oscillators that can be used to generate audio or control effects.
 * `phase shift` - A phase shifter connected to an LFO.
 * `fx_pitch_shift` - Adjusts the pitch of the incoming signal.
 * `fx_ring_mod` - This one is bananas.  It basically modulates the incoming signal with a sine wave to create wild harmonics.
 * `fx_shaper` - Basically a synth that generates a wave at the same frequency, one octave below, and two octaves below.  Each of the three synthesized signals has their own output channel so they can either be mixed or sent through different effects.
 * `fx_slicer` - Chops up audio in time domain and sends to differnt channels.  Great for making rhymthmic effects.
 * `fx_variable_delay` - This is a short delay line that is varied in real time.  This is core building block for flanger, chorus, and vibrato effects.

 There are several other modules in development including reverbs, all-pass filter, automatic loop detector, etc.

#### Special parameters and constants

Some objects take inputs that aren't numbers but rather a constant that is chosen from a list.  Below is a list of the constants that may be used when initializing an effect.

`BIQUAD_FILTER_TYPE`: Types of filters that can be implemented with `fx_biquad_fiter`
 * `BIQUAD_TYPE_LPF` - low-pass filter (cuts out high frequencies)
 * `BIQUAD_TYPE_HPF` - high-pass filter (cuts out low frequencies)
 * `BIQUAD_TYPE_BPF` - band-pass filter (only lets a limited range of frequencies through)
 * `BIQUAD_TYPE_NOTCH` - opposite of band-pass
 * `BIQUAD_TYPE_PEAKING` - don't worry about it
 * `BIQUAD_TYPE_L_SHELF` - similar to low pass
 * `BIQUAD_TYPE_H_SHELF` - similar to high pass

`BIQUAD_FILTER_WIDTH`: How "wide" a filter is for use with `fx_biquad_fiter`
 * `FILTER_WIDTH_VERY_NARROW` - very narrow indeed
 * `FILTER_WIDTH_NARROW` - like a wah filter
 * `FILTER_WIDTH_MEDIUM` - a bit narrow
 * `FILTER_WIDTH_WIDE` - wide with flat response (q=0.7071)
 * `FILTER_WIDTH_VERY_WIDE` - very wide

`ENV_TRACKER_TYPE`: Type of envelop tracking
 * `ENV_TRACK_PEAKS` - haha, there is only one option - rides the peaks like riding the lion

`EFFECT_TRANSITION_SPEED`: How quickly to transition parameters when a parameter is modified (used in several effects)
 * `TRANS_VERY_FAST` - rabbit
 * `TRANS_FAST`
 * `TRANS_MED`
 * `TRANS_SLOW`
 * `TRANS_VERY_SLOW` - turtle

`OSC_TYPES`: Types of oscillators (used in several effects)
  * `OSC_SINE` - sine wave
  * `OSC_TRI` - triangle wave
  * `OSC_SQUARE` - square wave
  * `OSC_PULSE` - pulse wave
  * `OSC_RAMP` - ramp wave

`POLY_CLIP_FUNC`: Various clipping functions for use with `fx_clipper`
  * `POLY_SMOOTHSTEP` - try it and see if you like it
  * `POLY_SMOOTHERSTEP` - try it and see if you like it
  * `POLY_SIMPLE_1` - try it and see if you like it
  * `POLY_SIMPLE_2` - try it and see if you like it

