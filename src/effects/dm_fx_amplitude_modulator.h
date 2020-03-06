// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_AMP_MOD_H
#define DM_FX_AMP_MOD_H

/**
 * @brief      Effect: Amplitude modulator for creating tremelo-like effects.
 * 
 * Amplitude modulators are the basic building blocks of tremelos and rhythmic effects.  
 * They essentially use an oscillator / waveform or an external control signal to vary 
 * the amplitude / volume of a signal.
 * 
 * 
 * ``` CPP
 * #include <dreammakerfx.h>
 * 
 * fx_amplitude_mod    mod1(1.0,     // Rate (Hz) is once per second
 *                          0.8,     // Depth (0.0->1.0)
 *                          0,       // Initial phase (degrees)
 *                          OSC_SINE,// Oscillator type is a sine wave
 *                          false);  // Don't use external LFO
 * 
 * void setup() {
 * 
 *   pedal.init();   // Initialize pedal
 * 
 *   // Route audio through effects
 *   pedal.route_audio(pedal.instr_in, mod1.input);
 *   pedal.route_audio(mod1.output, pedal.amp_out);
 * 
 *   pedal.add_bypass_button(FOOTSWITCH_LEFT); // Use left footswitch/LED to bypass effect
 * 
 *   pedal.run();    // Run effects
 * }
 * 
 * void loop() {
 * 
 *   // Pot 0 changes the rate of the tremelo from 0 to 4Hz
 *   if (pedal.pot_0.has_changed()) {
 *     mod1.set_rate_hz(pedal.pot_0.val*4.0);
 *   }
 * 
 *   // Pot 1 changes the depth from 0.0 to 1.0
 *   if (pedal.pot_1.has_changed()) {
 *     mod1.set_depth(pedal.pot_1);
 *   }
 * 
 *    pedal.service(); // Run pedal service to take care of stuff
 * }
 * ```
 * 
 * There are lots of cool things you can try with amplitude modulators: use tap function to set rate, use a instrument input through a pitch shifter as the external modulator, use high modulation frequency like 440.0Hz, try a few in parallel running through filters with different initial phase values (to create harmonic tremelos).
 * 
 * 
 */
class fx_amplitude_mod: public fx_effect {

  private:

    // Parameters
    float     param_depth;
    float     param_rate_hz;
    OSC_TYPES param_type;
    float     param_phase_deg;
    bool      param_ext_modulator;

    OSC_TYPES last_type;

    void init() {
	    // Set class
	    type = FX_AMPLITUDE_MODULATOR;

	    // Set name
	    strcpy(effect_name, "amplitude modulator");      

			// Assign programmable node names
	    input = &node_input;
	    output = &node_output;
	    ext_mod_in = &node_loop_ext_mod;      

		  	// Initialize parameter stack
	    int indx = 1;
	    param_stack[indx] = &param_rate_hz;
	    param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_phase_deg;
      param_stack_types[indx++] = T_FLOAT;

	    param_stack[indx] = &param_depth;
	    param_stack_types[indx++] = T_FLOAT;

	    param_stack[indx] = &param_type;
	    param_stack_types[indx++] = T_INT16;

	    param_stack[indx] = &param_ext_modulator;
	    param_stack_types[indx++] = T_BOOL;
	    total_params = indx;         

	   	// Add additional nodes to the audio stack
	    audio_node_stack[total_audio_nodes++] = &node_loop_ext_mod;

	    // Assign controls
	    depth = &node_ctrl_depth;
	    rate_hz = &node_ctrl_rate_hz;
	    
	  } 

		// Additional audio nodes
	  fx_audio_node node_loop_ext_mod;

	  // Control nodes
	  fx_control_node node_ctrl_depth;
	  fx_control_node node_ctrl_rate_hz;

 public:



	/**
	 * Audio routing node: primary audio input
	 */
  fx_audio_node * input;
  /**
   * Audio routing node: primary audio output
   */
  fx_audio_node * output;
  /**
   * Audio routing node: external modulator audio input
   */
  fx_audio_node * ext_mod_in;


  /**
   * Control routing node: amplifude modulator depth (should be between 0.0 and 1.0)
   */
  fx_control_node * depth;
  /**
   * Control routing node: amplitide modulator rate (Hz) (i.e. 1.0 = once per second)
   */
  fx_control_node * rate_hz;

  /**
   * @brief      Basic constructor/initializer for amplitude modulator
   *
   * ``` CPP
   * fx_amplitude_mod    mod1(1.0,     // Rate (Hz) is once per second
   *                          0.8);     // Depth (0.0->1.0)
   * ```
   * @param[in]  modulation_rate   When using an internal oscillator, the “modulation” rate is oscillation (cycles per second).  When in doubt, start with 1.0 (one cycle per second)   
   * @param[in]  modulation_depth  How much the volume is “modulated”.  A value of 0.0 is none at all and a value of 1.0  means  full volume to zero volume.
   */
	fx_amplitude_mod(float rate_hz, float depth) : 
    node_loop_ext_mod(NODE_IN, "external modulator", this),
    node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_AMP_MOD_PARAM_ID_MOD_DEPTH),
    node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_AMP_MOD_PARAM_ID_MOD_FREQ) {
			
			// Set parameters
	    param_depth = depth;
	    param_rate_hz = rate_hz;
	    last_type = param_type = OSC_SINE;
	    param_ext_modulator = false;
      param_phase_deg = 0;

	    init();
  }

  /**
   * @brief      Advanced constructor for the amplitude modulator
   *
   *
   * ``` CPP
   * fx_amplitude_mod    mod1(1.0,     // Rate (Hz) is once per second
   *                          0.8,     // Depth (0.0->1.0)
   *                          0,       // Initial phase (degrees)
   *                          OSC_SINE,// Oscillator type is a sine wave
   *                          false);  // Don't use external LFO
   * ```
   * @param[in]  rate_hz            When using an internal oscillator, the “modulation” rate is oscillation (cycles per second).  When in doubt, start with 1.0 (one cycle per second)
   * @param[in]  depth              How much the volume is “modulated”.  A value of 0.0 is none at all and a value of 1.0  means  full volume to zero volume.
   * @param[in]  initial_phase_deg  The initial phase of the oscillator in degrees.  When in doubt, use 0.0.  This is useful when you want to have multiple oscillators running at different phases such as in harmonic tremelo where one may be at 0.0 and the other at 180.0.                                 
   * @param[in]  modulation_type    See `OSC_TYPES` for available waveforms (sine, square, triangle, random, pulse, etc.) as the modulation source.
   * @param[in]  use_ext_modulator  Rather than using an internal modulator, you can also use an external audio source.  Route audio to the .ext_mod_in audio to use it as the external modulator.
   */
  fx_amplitude_mod(float rate_hz, float depth, float initial_phase_deg, OSC_TYPES modulation_type, bool use_ext_modulator) :
    node_loop_ext_mod(NODE_IN, "external modulator", this),
    node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_AMP_MOD_PARAM_ID_MOD_DEPTH),
    node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_AMP_MOD_PARAM_ID_MOD_FREQ) {

			// Set parameters
	    param_depth = depth;
	    param_rate_hz = rate_hz;
      param_phase_deg = initial_phase_deg;      
	    last_type = param_type = modulation_type;
	    param_ext_modulator = use_ext_modulator;

	    init();
    }
  
  /**
   * @brief      Enable the amplitude modululator (it is enabled by default)
   */
  void enable() {    
    CHECK_LAST_ENABLED();
    param_enabled = true; 
    parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_BOOL, FX_AMP_MOD_PARAM_ID_ENABLED, (void *) &param_enabled);
  }

  /**
   * @brief      Bypass the amplitude modululator  (will just pass clean audio through)
   */
  void bypass() {
    CHECK_LAST_DISABLED();
    param_enabled = false; 
    parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_BOOL, FX_AMP_MOD_PARAM_ID_ENABLED, (void *) &param_enabled);
  }  


  /**
   * @brief      Sets the depth of the amplitude modululator 
   *
   * ``` CPP
   * 
   *   mod1.set_depth(0.5);  // Sets the depth of the modulator to a fixed value
   * ```
   *
   * @param[in]  depth  The depth fom 0.0 -> 1.0.  0.0 is no modulation at all,
   *                    1.0 is full modulation.
   */
  void set_depth(float depth) { 
    CHECK_LAST(depth, param_depth);
    
    if (node_ctrl_depth.connected) {
      return; 
    }

    param_depth = depth; 
    parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_FLOAT, FX_AMP_MOD_PARAM_ID_MOD_DEPTH, &param_depth);
  }


  /**
   * @brief      Sets the rate of the modulator in Hertz (cycles per second)
   *
   * @param[in]  rate_hz  The rate hz
   */
  void set_rate_hz(float rate_hz) { 
    CHECK_LAST(rate_hz, param_rate_hz);

    if (node_ctrl_rate_hz.connected) {
      return; 
    }

    param_rate_hz = rate_hz; 
    parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_FLOAT, FX_AMP_MOD_PARAM_ID_MOD_FREQ, &param_rate_hz);
  }    


  /**
   * @brief      Sets the the type of oscillator used as the LFO
   *
   * @param[in]  new_type  The new type of LFO (OSC_TYPES)
   */
  void set_lfo_type(OSC_TYPES new_type) {
    CHECK_LAST(new_type, param_type);

    param_type = new_type; 
    
    if (new_type != last_type) {
      parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_INT16, FX_AMP_MOD_PARAM_ID_MOD_TYPE, &param_type);
    }
    last_type = new_type;

  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /**
   * @brief      Print the parameters for this effect
   */
  void  print_params(void) {

		// void print_parameter( void * val, char * name, PARAM_TYPES type)
		Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );
    print_parameter( &param_depth, "Depth", T_FLOAT );
    print_parameter( &param_rate_hz, "Rate (Hz)", T_FLOAT );
    print_parameter( &param_phase_deg, "Initial phase (degrees)", T_FLOAT );
    print_parameter( &param_type, "Oscillator type", T_INT16 );
    print_parameter( &param_ext_modulator, "External modulator", T_BOOL );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_depth);
    print_ctrl_node_status(&node_ctrl_rate_hz);

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);
    print_audio_node_status(&node_loop_ext_mod);

    Serial.println();
  }
#endif
};

#endif 	// DM_FX_AMP_MOD_H