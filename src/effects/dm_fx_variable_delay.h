// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_VARIABLE_DELAY_H
#define DM_FX_VARIABLE_DELAY_H

/**
 * @brief      Effect: Variable delay - foundational block of flangers and choruses 
 * 
 * The variable delay effect is the basis for a number of time-varying delay
 * effects like chorus, flanger, phaser, vibrato, Leslie simulator, etc.
 *
 * Here's a nice tutorial on how variable delays work in these various building
 * blocks:
 * https://www.dsprelated.com/freebooks/pasp/Time_Varying_Delay_Effects.html
 * 
 * Example:
 *    ___var_del_1.c___
 *    
 */
class fx_variable_delay: public fx_effect {

  private:

    // Parameters
    float param_depth;
    float param_rate_hz;
    float param_initial_phase_deg;
    float param_feedback;
    float param_mix_clean;
    float param_mix_delayed;
    OSC_TYPES param_type;
    float param_delay_buf_size_ms;
    bool  param_ext_modulator;

    // Control nodes
    fx_control_node node_ctrl_depth;
    fx_control_node node_ctrl_rate_hz;
    fx_control_node node_ctrl_feedback;
    fx_control_node node_ctrl_mix_clean;
    fx_control_node node_ctrl_mix_delayed;

    // Additional audio nodes
    fx_audio_node node_loop_ext_mod;
    fx_audio_node node_modulated_out;


    void init (void) {
			// Set class
      type = FX_VARIABLE_DELAY;

      // Set name
      strcpy(effect_name, "variable delay");      

      // initialize other variables
      param_enabled = true;

			// Assign programmable node names
      input = &node_input;
      output = &node_output;
      ext_mod_in = &node_loop_ext_mod;      
      modulated_out = &node_modulated_out;

 			// Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_rate_hz;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_depth;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_initial_phase_deg;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_feedback;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_type;
      param_stack_types[indx++] = T_INT16;

      param_stack[indx] = &param_ext_modulator;
      param_stack_types[indx++] = T_BOOL;

      param_stack[indx] = &param_delay_buf_size_ms;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_mix_clean;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_mix_delayed;
      param_stack_types[indx++] = T_FLOAT;

      total_params = indx;      

      // Add additional nodes to the audio stack
      audio_node_stack[total_audio_nodes++] = &node_loop_ext_mod;
      audio_node_stack[total_audio_nodes++] = &node_modulated_out;

      // Add addititonal nodes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_depth;
      control_node_stack[total_control_nodes++] = &node_ctrl_rate_hz;
      control_node_stack[total_control_nodes++] = &node_ctrl_feedback;
      control_node_stack[total_control_nodes++] = &node_ctrl_mix_clean;
      control_node_stack[total_control_nodes++] = &node_ctrl_mix_delayed;

      depth = &node_ctrl_depth;
      rate_hz = &node_ctrl_rate_hz;
      feedback = &node_ctrl_feedback;
      mix_clean = &node_ctrl_mix_clean;
      mix_delayed = &node_ctrl_mix_delayed;
    }

  public:

    /**
     * Audio routing node [input]: primary audio input
     */
    fx_audio_node * input;

    /**
     * Audio routing node [output]: primary audio output
     */
    fx_audio_node * output;

    /**
     * Audio routine node [input]: use another signal as the modulator source
     * such as an fx_oscillator.  The oscillator can be run though the clipper
     * for example to create new types of waveforms.
     */
    fx_audio_node * ext_mod_in;


    /**
     * Audio routing node [output]: just the pitch modulated signal without
     * mixing in the original signal
     */
    fx_audio_node * modulated_out;


    /**
     * Control routing node [input]: modulation depth
     */
    fx_control_node * depth;

    /**
     * Control routing node [input]: modulation rate in Hz
     */
    fx_control_node * rate_hz;


    /**
     * Control routing node [input]: feedback
     */
    fx_control_node * feedback;

    /**
     * Control routing node [input]: clean signal mix
     */
    fx_control_node * mix_clean;

    /**
     * Control routing node [input]: delayed signal mix
     */
    fx_control_node * mix_delayed;


    /**
     * @brief      Basic constructor/initializer for variable delay
     *
     * @param[in]  rate_hz   The modulation rate in Hz
     * @param[in]  depth     The modulation depth (0.0 -> 1.0)
     * @param[in]  feedback  The feedback from output to input (-1.0 ->
     *                       1.0)
     * @param[in]  mod_type  The shape of the waveform used to modulate
     *                       (e.g. OSC_SINE, OSC_TRI, etc.)
     */
    fx_variable_delay(float rate_hz, float depth, float feedback, OSC_TYPES mod_type) :
      node_loop_ext_mod(NODE_IN, "external modulator", this),
      node_modulated_out(NODE_OUT, "modulated output", this),
      node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_VAR_DELAY_PARAM_ID_MOD_DEPTH),
      node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_VAR_DELAY_PARAM_ID_MOD_FREQ),
      node_ctrl_mix_clean(NODE_IN, NODE_FLOAT, "node_ctrl_mix_clean", this, FX_VAR_DELAY_PARAM_ID_MIX_CLEAN),
      node_ctrl_mix_delayed(NODE_IN, NODE_FLOAT, "node_ctrl_mix_delayed", this, FX_VAR_DELAY_PARAM_ID_MIX_DELAYED),
      node_ctrl_feedback(NODE_IN, NODE_FLOAT, "node_ctrl_feedback", this, FX_VAR_DELAY_PARAM_ID_FEEDBACK) { 

      // Set parameters
      param_initial_phase_deg = 0;
      param_depth = depth;
      param_rate_hz = rate_hz;
      param_feedback = feedback;
      param_mix_clean = 0.7;
      param_mix_delayed = 0.5;
      param_delay_buf_size_ms = 30.0;
      param_type = mod_type;
      param_ext_modulator = false;

      init();

    }

    /**
     * @brief      Basic constructor/initializer for variable delay
     *
     * @param[in]  rate_hz      The modulation rate in Hz
     * @param[in]  depth        The modulation depth (0.0 -> 1.0)
     * @param[in]  feedback     The feedback from output to input (-1.0
     *                          ->1.0)
     * @param[in]  buf_size_ms  The size of the audio in milliseconds
     *                          (start wtih a value around 30)
     * @param[in]  mix_clean    The clean mix.  If this is set to 0.0,
     *                          then you'll just get the pitch changing
     *                          aspect of the wave that can used for
     *                          tape delay simulators, etc.
     * @param[in]  mix_delayed  The delayed signal mix. 
     * @param[in]  mod_type     The shape of the waveform used to modulate (e.g. OSC_SINE, OSC_TRI, etc.)
     * @param[in]  ext_mod      whether to use an external modulation source (set to true or false)
     */
    fx_variable_delay(float rate_hz, float depth, float feedback, float buf_size_ms, float mix_clean, float mix_delayed, OSC_TYPES mod_type, bool ext_mod ) :
      node_loop_ext_mod(NODE_IN, "external modulator", this),
      node_modulated_out(NODE_OUT, "modulated output", this),
      node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_VAR_DELAY_PARAM_ID_MOD_DEPTH),
      node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_VAR_DELAY_PARAM_ID_MOD_FREQ),
      node_ctrl_mix_clean(NODE_IN, NODE_FLOAT, "node_ctrl_mix_clean", this, FX_VAR_DELAY_PARAM_ID_MIX_CLEAN),
      node_ctrl_mix_delayed(NODE_IN, NODE_FLOAT, "node_ctrl_mix_delayed", this, FX_VAR_DELAY_PARAM_ID_MIX_DELAYED),      
      node_ctrl_feedback(NODE_IN, NODE_FLOAT, "node_ctrl_feedback", this, FX_VAR_DELAY_PARAM_ID_FEEDBACK) { 

      // Set parameters
      param_initial_phase_deg = 0;
      param_depth = depth;
      param_rate_hz = rate_hz;
      param_feedback = feedback;
      param_mix_clean = mix_clean;
      param_mix_delayed = mix_delayed;
      param_delay_buf_size_ms = buf_size_ms;
      param_type = mod_type;
      param_ext_modulator = ext_mod;

      init();

    }
      
    /**
     * @brief      Basic constructor/initializer for variable delay
     *
     * @param[in]  rate_hz      The modulation rate in Hz
     * @param[in]  depth        The modulation depth (0.0 -> 1.0)
     * @param[in]  feedback     The feedback from output to input (-1.0
     *                          ->1.0)
     * @param[in]  buf_size_ms  The size of the audio in milliseconds
     *                          (start wtih a value around 30)
     * @param[in]  mix_clean    The clean mix.  If this is set to 0.0,
     *                          then you'll just get the pitch changing
     *                          aspect of the wave that can used for
     *                          tape delay simulators, etc.
     * @param[in]  mix_delayed  The delayed signal mix. 
     * @param[in]  mod_type     The shape of the waveform used to modulate (e.g. OSC_SINE, OSC_TRI, etc.)
     * @param[in]  ext_mod      whether to use an external modulation source (set to true or false)
     * @param[in]  initial_phase Initial phase in degrees
     */
    fx_variable_delay(float rate_hz, float depth, float feedback, float buf_size_ms, float mix_clean, float mix_delayed, OSC_TYPES mod_type, bool ext_mod, float initial_phase ) :
      node_loop_ext_mod(NODE_IN, "external modulator", this),
      node_modulated_out(NODE_OUT, "modulated output", this),
      node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_VAR_DELAY_PARAM_ID_MOD_DEPTH),
      node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_VAR_DELAY_PARAM_ID_MOD_FREQ),
      node_ctrl_mix_clean(NODE_IN, NODE_FLOAT, "node_ctrl_mix_clean", this, FX_VAR_DELAY_PARAM_ID_MIX_CLEAN),
      node_ctrl_mix_delayed(NODE_IN, NODE_FLOAT, "node_ctrl_mix_delayed", this, FX_VAR_DELAY_PARAM_ID_MIX_DELAYED),      
      node_ctrl_feedback(NODE_IN, NODE_FLOAT, "node_ctrl_feedback", this, FX_VAR_DELAY_PARAM_ID_FEEDBACK) { 

      // Set parameters
      param_initial_phase_deg = initial_phase;
      param_depth = depth;
      param_rate_hz = rate_hz;
      param_feedback = feedback;
      param_mix_clean = mix_clean;
      param_mix_delayed = mix_delayed;
      param_delay_buf_size_ms = buf_size_ms;
      param_type = mod_type;
      param_ext_modulator = ext_mod;

      init();

    }      

    /**
     * @brief      Enable the __this_effect__ (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_BOOL, FX_VAR_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the __this_effect__ (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_BOOL, FX_VAR_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
    }  

    /**
     * @brief      Updates the depth of the variable delay
     *
     * @param[in]  depth  The new depth value
     */
    void set_depth(float depth) { 

      CHECK_LAST(depth, param_depth);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_depth.connected) {
        return; 
      }

      param_depth = depth; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_FLOAT, FX_VAR_DELAY_PARAM_ID_MOD_DEPTH, &param_depth);
    }

    /**
     * @brief      Updates the rate (Hz) of the variable delay
     *
     * @param[in]  rate_hz  The new rate hz
     */
    void set_rate_hz(float rate_hz) { 

      CHECK_LAST(rate_hz, param_rate_hz);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_rate_hz.connected) {
        return; 
      }

      param_rate_hz = rate_hz; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_FLOAT, FX_VAR_DELAY_PARAM_ID_MOD_FREQ, &param_rate_hz);
    }    

    /**
     * @brief      Updates the feedback parameter of the variable delay
     *
     * @param[in]  feedback  The new feedback value (-1.0->1.0)
     */
    void set_feedback(float feedback) { 

      CHECK_LAST(feedback, param_feedback);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_rate_hz.connected) {
        return; 
      }

      param_feedback = feedback; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_FLOAT, FX_VAR_DELAY_PARAM_ID_FEEDBACK, &param_feedback);
    }   

    /**
     * @brief      Updates the clean mix of the variable delay
     *
     * @param[in]  mix_clean  The new clean mix value
     */
    void set_mix_clean(float mix_clean) { 

      CHECK_LAST(mix_clean, param_mix_clean);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_mix_clean.connected) {
        return; 
      }

      param_mix_clean = mix_clean; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_FLOAT, FX_VAR_DELAY_PARAM_ID_MIX_CLEAN, &param_mix_clean);
    }     

    /**
     * @brief      Updates the delayed signal mix of the variable delay
     *
     * @param[in]  mix_delayed  The new delayed mix value
     */
    void set_mix_delayed(float mix_delayed) { 

      CHECK_LAST(mix_delayed, param_mix_delayed);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_mix_delayed.connected) {
        return; 
      }

      param_mix_delayed= mix_delayed; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_FLOAT, FX_VAR_DELAY_PARAM_ID_MIX_DELAYED, &param_mix_delayed);
    }     

    /**
     * @brief      Sets the the type of oscillator used as the LFO
     *
     * @param[in]  new_type  The new type of LFO (OSC_TYPES)
     */
    void set_lfo_type(OSC_TYPES new_type) {

      CHECK_LAST(new_type, param_type);

      param_type = new_type; 
      parent_canvas->spi_transmit_param(FX_VARIABLE_DELAY, instance_id, T_INT16, FX_VAR_DELAY_PARAM_ID_MOD_TYPE, &param_type);

    }

   /**
   * @brief  Prints the parameters for this effect
   */
  void  print_params(void) {

		// void print_parameter( void * val, char * name, PARAM_TYPES type)
		Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );
    print_parameter( &param_depth, "Depth", T_FLOAT );
    print_parameter( &param_rate_hz, "Rate (Hz)", T_FLOAT );
    print_parameter( &param_initial_phase_deg, "Initial phase (degrees)", T_FLOAT );
    print_parameter( &param_mix_clean, "Clean signal mix", T_FLOAT );
    print_parameter( &param_mix_delayed, "Delayed signal mix", T_FLOAT );
    print_parameter( &param_feedback, "Feedback", T_FLOAT );
    print_parameter( &param_ext_modulator, "External modulator", T_BOOL );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_depth);
    print_ctrl_node_status(&node_ctrl_rate_hz);
    print_ctrl_node_status(&node_ctrl_feedback);
    print_ctrl_node_status(&node_ctrl_mix_clean);

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);
    print_audio_node_status(&node_loop_ext_mod);

    Serial.println();
  }
    
};



#endif 	// DM_FX_VARIABLE_DELAY_H
