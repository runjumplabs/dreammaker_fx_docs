// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_GAIN_H
#define DM_FX_GAIN_H

/**
 * @brief      Effect: Gain - used to increase or decrease the volume of an audio signal
 * 
 * 
 * 
 */
class fx_gain: public fx_effect {
  
  private:

    // Parameters
    uint16_t param_speed;
    float param_gain;

    // Control nodes
    fx_control_node node_ctrl_gain;

  
    void init(void) {
      // Set class
      type = FX_GAIN;

      // Set name
      strcpy(effect_name, "gain");

      // Assign programmable node names
      input = &node_input;
      output = &node_output;


      // Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_gain;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_speed;
      param_stack_types[indx++] = T_INT16;
      total_params = indx;

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_gain;

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
     * Control routing node: gain value input - you can then link the envelope filter to this to create slow swell effects
     */
    fx_control_node * gain;

    /**
     * @brief      Basic constructor/initializer for gain
     *
     * @param[in]  gain_val  The gain value
     */
    fx_gain(float gain_val) : 
      node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain_value", this, FX_GAIN_PARAM_ID_GAIN)  {

      // Set parameters
      param_gain = gain_val;
      param_speed = (uint16_t) TRANS_MED;

      init();
    }


    /**
     * @brief      Advanced constructor for the gain
     *
     * @param[in]  gain_val          The gain value (typically between 0.0->1.0 to make a signal quiter and > 1.0 to make a signal louder)
     * @param[in]  gain_trans_speed  The gain transaction speed based on `EFFECT_TRANSITION_SPEED` defined above (i.e. slow -> fast)
     */
    fx_gain(float gain_val, EFFECT_TRANSITION_SPEED gain_trans_speed) : 
      node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain_value", this, FX_GAIN_PARAM_ID_GAIN)  {

      // Set parameters
      param_gain = gain_val;
      param_speed = (uint16_t) gain_trans_speed;

      init();
    }



    /**
     * @brief      Enable the __this_effect__ (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_BOOL, FX_GAIN_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the __this_effect__ (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_BOOL, FX_GAIN_PARAM_ID_ENABLED, (void *) &param_enabled);
    }    

    /**
     * @brief      Sets the gain multiplier.  For example, a value of 2 will
     *             double the volume/amplitude and a value of 0.5 will halve the
     *             volume/amplitude.
     *
     * @param[in]  new_gain  The new gain value (0.0 -> 4.0)
     */
    void set_gain(float new_gain) { 

      CHECK_LAST(new_gain, param_gain);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_gain.connected) {
        return; 
      }

      param_gain = new_gain; 
      parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_FLOAT, FX_GAIN_PARAM_ID_GAIN, &param_gain);
    }

    /**
     * @brief      Sets the gain multiplier using decibles.  For example, a
     *             value of 0 will keep volume the same, a value of 6 will
     *             double the amplitude/volume, a value of -6 will halve the
     *             amplitude/volume.
     *
     * @param[in]  new_gain_db  The new gain value (dB)
     */
    void set_gain_db(float new_gain_db) { 


      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_gain.connected) {
        return; 
      }

      new_gain_db = powf(10.0, new_gain_db*(1.0/20.0)); 
      CHECK_LAST(new_gain_db, param_gain);

      param_gain = new_gain_db; 
      parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_FLOAT, FX_GAIN_PARAM_ID_GAIN, &param_gain);
    }    

    /**
     * @brief  Prints the parameters for the delay effect
     */
    void  print_params(void) {

      // void print_parameter( void * val, char * name, PARAM_TYPES type)
      Serial.println("Parameters:");
      print_parameter( &param_enabled, "Enabled", T_BOOL );
      print_parameter( &param_gain, "Gain (linear)", T_FLOAT );
      print_parameter( &param_speed, "Transition rate", T_INT16 );

      Serial.println("Control Routing:");      
      print_ctrl_node_status(&node_ctrl_gain);

      Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);
      print_audio_node_status(&node_output);

      Serial.println();
    }

};
#endif  // DM_FX_GAIN_H
