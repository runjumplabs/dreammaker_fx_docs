// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_MULTITAP_DELAY_H
#define DM_FX_MULTITAP_DELAY_H

/**
 * @brief      Effect: Multi-tap delay
 * 
 * A multi-tap delay is a delay line that has multiple read "taps" set a different delay lenghts.  
 * Multi-tap delays can be used to create interesting rhythmic effects and are also a foundational
 * building block of reverbs.
 * 
 * Here an example of using a multi-tap delay to generate early reflections in a reverb algorithm
 * 
 *  ``` CPP
 *  fx_multitap_delay     early_reflections(10.5, 0.2,     // Tap 1 (length and gain)
 *                                          13.5, 0.2,     // Tap 2 (length and gain)
 *                                          16.0, 0.2,     // Tap 3 (length and gain)
 *                                          19.5, 0.2,     // Tap 4 (length and gain)
 *                                          0.5,          // Dry mix
 *                                          0.5);         // Effect mix
 *  
 *  
 *  ```
 */
class fx_multitap_delay: public fx_effect {

  private:
    float param_tap_1_ms;
    float param_tap_2_ms;
    float param_tap_3_ms;
    float param_tap_4_ms;
    float param_gain_1;
    float param_gain_2;
    float param_gain_3;
    float param_gain_4;
    float param_wet_mix;
    float param_dry_mix;

    void init(void) {
      
      // Set class
      type = FX_DELAY_MULTITAP;

      // Set name
      strcpy(effect_name, "multitap delay");

      int indx = 1;
      param_stack[indx] = &param_tap_1_ms;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_gain_1;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_tap_2_ms;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_gain_2;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_tap_3_ms;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_gain_3;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_tap_4_ms;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_gain_4;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_dry_mix;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_wet_mix;
      param_stack_types[indx++] = T_FLOAT;
      total_params = indx;

		    // Assign programmable node names
		    input = &node_input;
		    output = &node_output;

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
     * @brief      Basic constructor for the multi-tap delay effect
     *
     * If a tap isn't being used, set its delay length to zero
     *
     * @param[in]  tap_len_1_ms  The tap 1 length 1 milliseconds
     * @param[in]  gain_1        The gain of tap
     * @param[in]  tap_len_2_ms  The tap 2 length 2 milliseconds
     * @param[in]  gain_2        The gain of tap
     * @param[in]  tap_len_3_ms  The tap 3 length 3 milliseconds
     * @param[in]  gain_3        The gain of tap
     * @param[in]  tap_len_4_ms  The tap 4 length 4 milliseconds
     * @param[in]  gain_4        The gain of tap 
     * @param[in]  dry_mix       The dry mix
     * @param[in]  wet_mix       The wet mix
     */
    fx_multitap_delay(float tap_len_1_ms, float gain_1,
    								  float tap_len_2_ms, float gain_2,
											float tap_len_3_ms, float gain_3,
											float tap_len_4_ms, float gain_4,
											float dry_mix, 
											float wet_mix) {

			param_tap_1_ms = tap_len_1_ms;
			param_tap_2_ms = tap_len_2_ms;
			param_tap_3_ms = tap_len_3_ms;
			param_tap_4_ms = tap_len_4_ms;

			param_gain_1 = gain_1;
			param_gain_2 = gain_2;
			param_gain_3 = gain_3;
			param_gain_4 = gain_4;

			param_wet_mix = wet_mix;
			param_dry_mix = dry_mix;

			init();

    }

    /**
     * @brief      Enable the multitap delay (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_DELAY_MULTITAP, instance_id, T_BOOL, FX_MULTITAP_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the multitap delay (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_DELAY_MULTITAP, instance_id, T_BOOL, FX_MULTITAP_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
    }    

    /**
     * @brief      Updates the dry / clean mix of the multitap delay (0.0 to 1.0)
     *
     * @param[in]  dry_mix  The new dry mix
     */
    void set_dry_mix(float dry_mix) { 
      
      CHECK_LAST(dry_mix, param_dry_mix);


      param_dry_mix = dry_mix; 
      parent_canvas->spi_transmit_param(FX_DELAY_MULTITAP, instance_id, T_FLOAT, FX_MULTITAP_DELAY_PARAM_ID_DRY_MIX, &param_dry_mix);
    }

    /**
     * @brief      Updates the wet / delay mix of the multitap delay (0.0 to 1.0)
     *
     * @param[in]  wet_mix  The new wet mix
     */
    void set_wet_mix(float wet_mix) { 
      
      CHECK_LAST(wet_mix, param_wet_mix);

      param_wet_mix = wet_mix; 
      parent_canvas->spi_transmit_param(FX_DELAY_MULTITAP, instance_id, T_FLOAT, FX_MULTITAP_DELAY_PARAM_ID_WET_MIX, &param_wet_mix);
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /**
   * @brief      Print the parameters for this effect
   */
  void  print_params(void) {
		Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );
    print_parameter( &param_tap_1_ms, "Tap 1 length (ms)", T_FLOAT );
    print_parameter( &param_gain_1, "Tap 1 gain", T_FLOAT );
    print_parameter( &param_tap_2_ms, "Tap 2 length (ms)", T_FLOAT );
    print_parameter( &param_gain_2, "Tap 2 gain", T_FLOAT );
    print_parameter( &param_tap_3_ms, "Tap 3 length (ms)", T_FLOAT );
    print_parameter( &param_gain_3, "Tap 3 gain", T_FLOAT );
    print_parameter( &param_tap_4_ms, "Tap 4 length (ms)", T_FLOAT );
    print_parameter( &param_gain_4, "Tap 4 gain", T_FLOAT );

    print_parameter( &param_dry_mix, "Dry mix", T_FLOAT );
    print_parameter( &param_wet_mix, "Wet mix", T_FLOAT );

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);

    Serial.println();
  }    
#endif     	

};

#endif 	// DM_FX_MULTITAP_DELAY_H


