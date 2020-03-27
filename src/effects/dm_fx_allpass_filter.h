// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_ALLPASS_FILTER_H
#define DM_FX_ALLPASS_FILTER_H

/**
 * @brief      Effect: Allpass filter
 *
 * Allpass filters have a flat frequency response but displace signals in the 
 * time domain as a function of frequency.  They're a common building block
 * in reverbs but are also used in phase shifters as well.
 * 
 */

class fx_allpass_filter: public fx_effect {

private:

  // Parameters
  float param_gain;
  float param_length_ms;

  // Control nodes
  fx_control_node node_ctrl_gain;

  void init(void) {

    // Set class
    type = FX_ALLPASS_FILTER;

    // Set name
    strcpy(effect_name, "allpass filter");

    // Assign programmable node names
    input = &node_input;
    output = &node_output;

      // Initialize parameter stack
    int indx = 1;
    param_stack[indx] = &param_gain;
    param_stack_types[indx++] = T_FLOAT;
    param_stack[indx] = &param_length_ms;
    param_stack_types[indx++] = T_FLOAT;

    total_params = indx;         

    // Assign controls
    gain = &node_ctrl_gain;
  }

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
   * Control routing node: allpass filter gain
   */
  fx_control_node * gain;

  /**
   * @brief      Basic constructor/initializer for the allpass filter
   *
   * @param[in]  gain       The gain of the allpass filter (-1.0 to 1.0)
   * @param[in]  length_ms  The length of the allpass filter in milliseconds
   */
  fx_allpass_filter(float gain, float length_ms) : 
    node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_ALLPASS_PARAM_ID_GAIN) {
      
      // Set parameters
      param_gain = gain;
      param_length_ms = length_ms;

      init();
  }


  /**
   * @brief      Enanle the allpass filter
   */
  void enable() {    
    CHECK_LAST_ENABLED();
    param_enabled = true; 
    parent_canvas->spi_transmit_param(FX_ALLPASS_FILTER, instance_id, T_BOOL, FX_ALLPASS_PARAM_ID_ENABLED, (void *) &param_enabled);
  }

  /**
   * @brief      Bypass the allpass filter  (will just pass clean audio through)
   */
  void bypass() {
    CHECK_LAST_DISABLED();
    param_enabled = false; 
    parent_canvas->spi_transmit_param(FX_ALLPASS_FILTER, instance_id, T_BOOL, FX_ALLPASS_PARAM_ID_ENABLED, (void *) &param_enabled);
  }  

  /**
   * @brief      Sets the gain of the allpass filter
   *
   * @param[in]  gain  Gain of the offpass filter
   */
  void set_gain(float gain) { 
    CHECK_LAST(gain, param_gain);
    
    if (node_ctrl_gain.connected) {
      return; 
    }

    param_gain = gain; 
    parent_canvas->spi_transmit_param(FX_ALLPASS_FILTER, instance_id, T_FLOAT, FX_ALLPASS_PARAM_ID_GAIN, &param_gain);
  }

  /**
   * @brief  Prints the parameters for the delay effect
   */
  void  print_params(void) {

    // void print_parameter( void * val, char * name, PARAM_TYPES type)
    Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );

    print_parameter( &param_gain, "Gain", T_FLOAT );
    print_parameter( &param_length_ms, "Length (ms)", T_FLOAT );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_gain);

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);

    Serial.println();
  }
};


#endif 	// DM_FX_ALLPASS_FILTER_H