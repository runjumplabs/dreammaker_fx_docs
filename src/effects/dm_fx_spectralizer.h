// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_SPECTRALIZER_H
#define DM_FX_SPECTRALIZER_H


/**
 * @brief      Effect: Pitch shifter - shifts audio up or down in pitch  
 * 
 * This effect uses a phase vocoder to perform the pitch shift so it can perform multiple
 * pitch shifts at the same time.
 * 
 */
class fx_pitch_shift_fd: public fx_effect {

  private:

    // Ring modulator parameters
    float param_freq_shift_1;
    float param_freq_shift_2;
    float param_vol_1;
    float param_vol_2;
    float param_vol_clean;


    // Control nodes
    fx_control_node node_ctrl_freq_shift_1;
    fx_control_node node_ctrl_freq_shift_2;
    fx_control_node node_ctrl_vol_1;
    fx_control_node node_ctrl_vol_2;
    fx_control_node node_ctrl_vol_clean;

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
     * Control routing node: first pitch shift amount
     */
    fx_control_node * freq_shift_1;

   /**
     * Control routing node: second pitch shift amount
     */
    fx_control_node * freq_shift_2;

    /**
     * Control routing node: volume of first pitch shift channel
     */
    fx_control_node * vol_1;

    /**
     * Control routing node: volume of second pitch shift channel
     */
    fx_control_node * vol_2;

    /**
     * Control routing node: clean mix
     */
    fx_control_node * vol_clean;

    /**
     * @brief      Basic constructor/initializer for the frequency-domain based pitch shifter
     *
     * When setting the frequency, you can utilize a set of constants that define various semitone
     * relationships.  The constant `SEMI_TONE_10`, for example, is 10 semitones above the current
     * note.  Adding an `N` for negative moves the tones below the note.  For example, the constant
     * `SEMI_TONE_N17` is 17 semitones below the current note.
     *
     * ``` CPP
     * 
     * fx_pitch_shift_fd    pitch_shift(SEMI_TONE_7,  // Set pitch shift to fifth above (7 semitones) or approx 1.5
     *                                  0.7,  // Pitch shift mix
     *                                  1.0); // Clean mix
     * ```
     *
     * @param[in]  freq       The relative frequency shift (2.0 would be an octave up, 0.5 would be an octave down)
     * @param[in]  vol        The volume/mix of frequency shifted audio
     * @param[in]  vol_clean  The volume/mix of the clean audio 
     */
    fx_pitch_shift_fd(float freq, float volume, float volume_clean) : 
        node_ctrl_freq_shift_1(NODE_IN, NODE_FLOAT, "node_ctrl_freq_shift_1", this, FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_1),
        node_ctrl_freq_shift_2(NODE_IN, NODE_FLOAT, "node_ctrl_freq_shift_2", this, FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_2),
        node_ctrl_vol_1(NODE_IN, NODE_FLOAT, "node_ctrl_vol_1", this, FX_SPECTRALIZER_PARAM_ID_VOL_1),
        node_ctrl_vol_2(NODE_IN, NODE_FLOAT, "node_ctrl_vol_2", this, FX_SPECTRALIZER_PARAM_ID_VOL_2),
        node_ctrl_vol_clean(NODE_IN, NODE_FLOAT, "node_ctrl_vol_clean", this, FX_SPECTRALIZER_PARAM_ID_VOL_CLEAN)
     {
      
      // Set class
      type = FX_SPECTRALIZER;

      // Set name
      strcpy(effect_name, "pitch shift (frequency domain)");

      // Set parameters
      param_freq_shift_1 = freq;
      param_freq_shift_2 = 0.0;
      param_vol_1 = volume;
      param_vol_2 = 0.0;
      param_vol_clean = volume_clean;

      // Assign programmable node names
      input = &node_input;
      output = &node_output;

      // Defaults
      param_enabled = true;

      // Initialize parameter stack
      param_stack[total_params] = &param_freq_shift_1;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_freq_shift_2;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_vol_1;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_vol_2;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_vol_clean;
      param_stack_types[total_params++] = T_FLOAT;

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_freq_shift_1;
      control_node_stack[total_control_nodes++] = &node_ctrl_freq_shift_2;
      control_node_stack[total_control_nodes++] = &node_ctrl_vol_1;
      control_node_stack[total_control_nodes++] = &node_ctrl_vol_2;
      control_node_stack[total_control_nodes++] = &node_ctrl_vol_clean;

      freq_shift_1 = &node_ctrl_freq_shift_1;
      freq_shift_2 = &node_ctrl_freq_shift_2;
      vol_1 = &node_ctrl_vol_1;
      vol_2 = &node_ctrl_vol_2;
      vol_clean = &node_ctrl_vol_clean;
      
    }

    /**
     * @brief       Advanced constructor/initializer for the frequency-domain based pitch shifter
     *
     * When setting the frequency, you can utilize a set of constants that define various semitone
     * relationships.  The constant `SEMI_TONE_10`, for example, is 10 semitones above the current
     * note.  Adding an `N` for negative moves the tones below the note.  For example, the constant
     * `SEMI_TONE_N17` is 17 semitones below the current note.
     * 
     * ``` CPP
     * 
     * fx_pitch_shift_fd    pitch_shift(SEMI_TONE_7,  // First shift is a fifth above
     *                                  0.7,  // First tone volume is 0.7
     *                                  SEMI_TONE_12,  // Second shift is an octave above
     *                                  0.5,  // Second tone volume is 0.5
     *                                  1.0); // Clean mix set to 1.0
     * ```  
     * 
     * @param[in]  freq_1        The first relative frequency shift (2.0 would be an octave up, 0.5 would be an octave down)
     * @param[in]  volume_1      The volume of the first frequency shifted tone
     * @param[in]  freq_2        The second relative frequency shift (2.0 would be an octave up, 0.5 would be an octave down)
     * @param[in]  volume_2      The volume of the second frequency shifted tone
     * @param[in]  volume_clean  The clean volume
     */
    fx_pitch_shift_fd(float freq_1, float volume_1, float freq_2, float volume_2, float volume_clean) : 
        node_ctrl_freq_shift_1(NODE_IN, NODE_FLOAT, "node_ctrl_freq_shift_1", this, FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_1),
        node_ctrl_freq_shift_2(NODE_IN, NODE_FLOAT, "node_ctrl_freq_shift_2", this, FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_2),
        node_ctrl_vol_1(NODE_IN, NODE_FLOAT, "node_ctrl_vol_1", this, FX_SPECTRALIZER_PARAM_ID_VOL_1),
        node_ctrl_vol_2(NODE_IN, NODE_FLOAT, "node_ctrl_vol_2", this, FX_SPECTRALIZER_PARAM_ID_VOL_2),
        node_ctrl_vol_clean(NODE_IN, NODE_FLOAT, "node_ctrl_vol_clean", this, FX_SPECTRALIZER_PARAM_ID_VOL_CLEAN)

     {
      
      // Set class
      type = FX_SPECTRALIZER;

      // Set name
      strcpy(effect_name, "pitch shift (frequency domain)");

      // Set parameters
      param_freq_shift_1 = freq_1;
      param_freq_shift_2 = freq_2;
      param_vol_1 = volume_1;
      param_vol_2 = volume_2;
      param_vol_clean = volume_clean;      

      // Assign programmable node names
      input = &node_input;
      output = &node_output;

      // Defaults
      param_enabled = true;

      // Initialize parameter stack
      param_stack[total_params] = &param_freq_shift_1;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_freq_shift_2;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_vol_1;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_vol_2;
      param_stack_types[total_params++] = T_FLOAT;
      param_stack[total_params] = &param_vol_clean;
      param_stack_types[total_params++] = T_FLOAT;

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_freq_shift_1;
      control_node_stack[total_control_nodes++] = &node_ctrl_freq_shift_2;
      control_node_stack[total_control_nodes++] = &node_ctrl_vol_1;
      control_node_stack[total_control_nodes++] = &node_ctrl_vol_2;
      control_node_stack[total_control_nodes++] = &node_ctrl_vol_clean;

      freq_shift_1 = &node_ctrl_freq_shift_1;
      freq_shift_2 = &node_ctrl_freq_shift_2;
      vol_1 = &node_ctrl_vol_1;
      vol_2 = &node_ctrl_vol_2;
      vol_clean = &node_ctrl_vol_clean;

    }

    /**
     * @brief      Enable the pitch shifter (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_SPECTRALIZER, instance_id, T_BOOL, FX_PITCH_SHIFT_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the pitch shifter (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_SPECTRALIZER, instance_id, T_BOOL, FX_PITCH_SHIFT_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Sets the pitch shifter value.  A freq_shift of 0.5 will drop down one octave.  A value of 
     *             2.0 will go up one octave.  A value of 1.0 will play at current pitch (no shift). 
     *
     * When setting the frequency, you can also utilize a set of constants that define various semitone
     * relationships.  The constant `SEMI_TONE_10`, for example, is 10 semitones above the current
     * note.  Adding an `N` for negative moves the tones below the note.  For example, the constant
     * `SEMI_TONE_N17` is 17 semitones below the current note.
     *
     * @param[in]  new_freq_shift  The frequency shift
     */
    void set_freq_shift_1(float new_freq_shift) { 

      CHECK_LAST(new_freq_shift, param_freq_shift_1);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_freq_shift_1.connected) {
        return;
      }

      param_freq_shift_1 = new_freq_shift;
      parent_canvas->spi_transmit_param(FX_SPECTRALIZER, instance_id, T_FLOAT, FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_1, (void *) &param_freq_shift_1);
    }

    /**
     * @brief      Sets the second pitch shifter value.  A freq_shift of 0.5 will drop down one octave.  A value of 
     *             2.0 will go up one octave.  A value of 1.0 will play at current pitch (no shift). 
     *
     * When setting the frequency, you can also utilize a set of constants that define various semitone
     * relationships.  The constant `SEMI_TONE_10`, for example, is 10 semitones above the current
     * note.  Adding an `N` for negative moves the tones below the note.  For example, the constant
     * `SEMI_TONE_N17` is 17 semitones below the current note.
     *
     * @param[in]  new_freq_shift  The frequency shift
     */
    void set_freq_shift_2(float new_freq_shift) { 

      CHECK_LAST(new_freq_shift, param_freq_shift_2);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_freq_shift_2.connected) {
        return;
      }

      param_freq_shift_2 = new_freq_shift;
      parent_canvas->spi_transmit_param(FX_SPECTRALIZER, instance_id, T_FLOAT, FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_2, (void *) &param_freq_shift_2);
    }

    /**
     * @brief      Sets the volume/gain of the first pitch shift channel
     *
     * @param[in]  vol_1  The volume level (0.0 to 1.0)
     */
    void set_vol_1(float new_vol_1) { 

      CHECK_LAST(new_vol_1, param_vol_1);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_vol_1.connected) {
        return;
      }

      param_vol_1 = new_vol_1;
      parent_canvas->spi_transmit_param(FX_SPECTRALIZER, instance_id, T_FLOAT, FX_SPECTRALIZER_PARAM_ID_VOL_1, (void *) &param_vol_1);
    }

    /**
     * @brief      Sets the volume/gain of the second pitch shift channel (if used)
     *
     * @param[in]  vol_2  The volume level (0.0 to 1.0)
     */
    void set_vol_2(float new_vol_2) { 

      CHECK_LAST(new_vol_2, param_vol_2);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_vol_2.connected) {
        return;
      }

      param_vol_2 = new_vol_2;
      parent_canvas->spi_transmit_param(FX_SPECTRALIZER, instance_id, T_FLOAT, FX_SPECTRALIZER_PARAM_ID_VOL_2, (void *) &param_vol_2);
    }

    /**
     * @brief      Sets the clean mix 
     *
     * @param[in]  vol_2  The volume level (0.0 to 1.0)
     */
    void set_vol_clean(float new_vol_clean) { 

      CHECK_LAST(new_vol_clean, param_vol_clean);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_vol_clean.connected) {
        return;
      }

      param_vol_clean = new_vol_clean;
      parent_canvas->spi_transmit_param(FX_SPECTRALIZER, instance_id, T_FLOAT, FX_SPECTRALIZER_PARAM_ID_VOL_CLEAN, (void *) &param_vol_clean);
    }    

    /**
     * @brief      Print the parameters for this effect
     */
    void  print_params(void) {

      Serial.println("Parameters:");
      print_parameter( &param_enabled, "Enabled", T_BOOL );

      print_parameter( &param_freq_shift_1, "Pitch shift #1", T_FLOAT );
      print_parameter( &param_vol_1, "Volume #1", T_FLOAT );
      print_parameter( &param_freq_shift_2, "Pitch shift #2", T_FLOAT );
      print_parameter( &param_vol_2, "Volume #2", T_FLOAT );
      print_parameter( &param_vol_clean, "Clean mix/volume", T_FLOAT );

      Serial.println("Control Routing:");      
      print_ctrl_node_status(&node_ctrl_freq_shift_1);
      print_ctrl_node_status(&node_ctrl_vol_1);
      print_ctrl_node_status(&node_ctrl_freq_shift_2);
      print_ctrl_node_status(&node_ctrl_vol_2);
      print_ctrl_node_status(&node_ctrl_vol_clean);

      Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);
      print_audio_node_status(&node_output);

    }
};

#endif 	// DM_FX_SPECTRALIZER_H
