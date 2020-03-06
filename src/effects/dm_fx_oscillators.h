// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_OSCILLATOR_H
#define DM_FX_OSCILLATOR_H

/**
 * @brief      Utility: Oscillator that can has both audio and control outputs
 * 
 * These are basically waveform generators that can be used as audio sources (for creating tones, etc.) or control sources (for controlling other effects). 
 * 
 * When using this as a control source (such as an LFO), use the .value control node.
 * 
 * 
 */
class fx_oscillator: public fx_effect {

  private:
    OSC_TYPES	param_type;
    float 		param_freq;
    float 		param_amp;
    float 		param_offset;
    float 		param_osc_param1;
    float 		param_osc_param2;
    float     param_osc_initial_phase;

    // Control nodes
    fx_control_node node_ctrl_freq;
    fx_control_node node_ctrl_amp;
    fx_control_node node_ctrl_offset;
    fx_control_node node_ctrl_value;

    void init(void) {
       // Set class
      type = FX_OSCILLATOR;

      // Set name
      strcpy(effect_name, "oscillator");

      // Assign programmable node names
      output = &node_output;      

      // Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_freq;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_amp;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_offset;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_type;
      param_stack_types[indx++] = T_INT16;

      param_stack[indx] = &param_osc_param1;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_osc_param2;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_osc_initial_phase;
      param_stack_types[indx++] = T_FLOAT;

      total_params = indx;      


      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_freq;
      control_node_stack[total_control_nodes++] = &node_ctrl_amp;
      control_node_stack[total_control_nodes++] = &node_ctrl_offset;
      control_node_stack[total_control_nodes++] = &node_ctrl_value;

      freq = &node_ctrl_freq;
      amplitude = &node_ctrl_amp;
      offset = &node_ctrl_offset;
      value = &node_ctrl_value;      
    }


 public:

/**
 * Audio routing node: primary audio oscillator output
 */
  fx_audio_node * output;

 /**
   * Control routing node: frequency of the oscillator in Hz
   */
	fx_control_node * freq;

 /**
   * Control routing node: amplitude of the oscillator (linear, typically between 0.0 and 1.0)
   */
	fx_control_node * amplitude;

 /**
   * Control routing node: The DC offset of the amplifier.  Useful if you're using this to control parameters in ranges not centered around 0.0.
   */
	fx_control_node * offset;

 /**
   * Control routing node: The current value of the oscillator.  Connect this node to external oscillator nodes for effects.
   */
	fx_control_node * value;

  /**
   * @brief      Basic constructor for an oscillator when used as an audio source
   *
   * @param[in]  osc_type   This is the type of waveform that is generated (sine, square wave, triangle wave, ramp, etc.).  See the appendix for a full list of options
   * @param[in]  freq       The frequency of the waveform in Hertz (Hz).
   * @param[in]  amplitude  The (linear) volume of the waveform.  Should be between 0.0 and 1.0 (full volume) when used to generate audio.  And it can be any value when generating control signals.
   */
  fx_oscillator(OSC_TYPES osc_type, float freq, float amplitude ) : 
    node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_OSCILLATOR_PARAM_ID_FREQ),
    node_ctrl_amp(NODE_IN, NODE_FLOAT, "node_ctrl_amp", this, FX_OSCILLATOR_PARAM_ID_AMP),
    node_ctrl_offset(NODE_IN, NODE_FLOAT, "node_ctrl_offset", this, FX_OSCILLATOR_PARAM_ID_OFFSET),
    node_ctrl_value(NODE_OUT, NODE_FLOAT, "node_ctrl_value", this, FX_OSCILLATOR_PARAM_ID_OSC_CTRL_VAL)  {

      // Set parameters
      param_type = osc_type;
      param_freq = freq;
      param_amp = amplitude;
      param_offset = 0.0;

      param_osc_param1 = 0.0;
      param_osc_param2 = 0.0;

      init();

    }


  /**
   * @brief      Basic constructor for an oscillator used as a control source
   *
   * @param[in]  osc_type         This is the type of waveform that is generated (sine, square wave, triangle wave, ramp, etc.).  See the appendix for a full list of options
   * @param[in]  freq             The frequency of the waveform in Hertz (Hz).
   * @param[in]  amplitude        The (linear) volume of the waveform.  Should be between 0.0 and 1.0 (full volume) when used to generate audio.  And it can be any value when generating control signals.
   * @param[in]  initial phase    The initial phase of the oscillator in degrees (0-360)
   */
  fx_oscillator(OSC_TYPES	osc_type, float freq, float amplitude, float initial_phase ) : 
    node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_OSCILLATOR_PARAM_ID_FREQ),
    node_ctrl_amp(NODE_IN, NODE_FLOAT, "node_ctrl_amp", this, FX_OSCILLATOR_PARAM_ID_AMP),
    node_ctrl_offset(NODE_IN, NODE_FLOAT, "node_ctrl_offset", this, FX_OSCILLATOR_PARAM_ID_OFFSET),
    node_ctrl_value(NODE_OUT, NODE_FLOAT, "node_ctrl_value", this, FX_OSCILLATOR_PARAM_ID_OSC_CTRL_VAL)  {

      // Set parameters
      param_type = osc_type;
      param_freq = freq;
      param_amp = amplitude;
      param_offset = 0.0;
      param_osc_initial_phase = initial_phase;

      param_osc_param1 = 0.0;
      param_osc_param2 = 0.0;

      init();

    }

    /**
     * @brief      Enable the oscillator (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();      
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_BOOL, FX_OSCILLATOR_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the oscillator (it will provide just a constant value)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_BOOL, FX_OSCILLATOR_PARAM_ID_ENABLED, (void *) &param_enabled);
    }      
    

    /**
     * @brief      Upates the frequency in Hz of the current oscillator
     *
     * @param[in]  freq  The frequency in Hz
     */
    void set_frequency(float freq) { 

      CHECK_LAST(freq, param_freq)

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_freq.connected) {
        return; 
      }

      param_freq = freq; 
      parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_FLOAT, FX_OSCILLATOR_PARAM_ID_FREQ, &param_freq);
    }   


    /**
     * @brief      Updates the amplitude for the current oscillator
     *
     * @param[in]  amplitude  The amplitude (linear)
     */
    void set_amplitude(float amplitude) { 

      CHECK_LAST(amplitude, param_amp)

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_amp.connected) {
        return; 
      }

      param_amp = amplitude; 
      parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_FLOAT, FX_OSCILLATOR_PARAM_ID_AMP, &param_amp);
    }   

    /**
     * @brief      Sets the oscillator type
     *
     * @param[in]  new_type  The new type of oscillator (OSC_TYPES)
     */
    void set_oscillator_type(OSC_TYPES new_type) {

      CHECK_LAST(new_type, param_type)

      param_type = new_type; 
      parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_INT16, FX_OSCILLATOR_PARAM_ID_TYPE, &param_type);

    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /**
     * @brief      Print the parameters for this effect
     */
    void  print_params(void) {

      // void print_parameter( void * val, char * name, PARAM_TYPES type)
      Serial.println("Parameters:");
      print_parameter( &param_enabled, "Enabled", T_BOOL );
      print_parameter( &param_type, "Oscillator type", T_INT16 );
      print_parameter( &param_freq, "Frequency (Hz)", T_FLOAT );
      print_parameter( &param_amp, "Amplitude", T_FLOAT );
      print_parameter( &param_offset, "Offset", T_FLOAT );

      Serial.println("Control Routing:");      
      print_ctrl_node_status(&node_ctrl_freq);
      print_ctrl_node_status(&node_ctrl_amp);
      print_ctrl_node_status(&node_ctrl_offset);
      print_ctrl_node_status(&node_ctrl_value);

      Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);
      print_audio_node_status(&node_output);

      Serial.println();
    }    
#endif
};


#endif 	// DM_FX_OSCILLATOR_H