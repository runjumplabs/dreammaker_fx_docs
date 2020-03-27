// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_RING_MODULATOR_H
#define DM_FX_RING_MODULATOR_H


/**
 * @brief      Effect: Ring modulator - frequency modulates the audio - crazy
 *             sounding
 *
 *             The following example is a full ring modulator pedal with tone
 *             control, wet/dry mix and of course ring modulator.  
 *
 *             ___ring_mod_1.c___
 */
class fx_ring_mod: public fx_effect {

  private:

    // Ring modulator parameters
    float param_freq;
    float param_depth;
    bool param_enable_filter;

    fx_control_node node_ctrl_freq;
    fx_control_node node_ctrl_depth;

    void init(void) {
      
      // Set class
      type = FX_RING_MOD;

      // Set name
      strcpy(effect_name, "ring modulator");


      // Assign programmable node names
      input = &node_input;
      output = &node_output;

      // Defaults
      param_enabled = true;

      // Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_freq;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_depth;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_enable_filter;
      param_stack_types[indx++] = T_BOOL;      
      total_params = indx;

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_freq;
      control_node_stack[total_control_nodes++] = &node_ctrl_depth;

      freq = &node_ctrl_freq;
      depth = &node_ctrl_depth;

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
     * Control routing node [input]: the carrier frequency of the ring moduator (Hz)
     */
    fx_control_node * freq;

    /**
     * Control routing node [input]: modulation depth
     */
    fx_control_node * depth;


    /**
     * @brief       Basic constructor/initializer for the ring modulator
     *
     * @param[in]  ring_mod_freq   The ring modifier frequency
     * @param[in]  ring_mod_depth  The ring modifier depth
     */
    fx_ring_mod(float ring_mod_freq, float ring_mod_depth) : 
        node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_RING_MOD_PARAM_ID_FREQ), 
        node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_RING_MOD_PARAM_ID_DEPTH) {

      // Set parameters
      param_freq = ring_mod_freq;
      param_depth = ring_mod_depth;
      param_enable_filter = false;

      init();
    }



    /**
     * @brief       Advanced constructor/initializer for the ring modulator
     *
     * @param[in]  ring_mod_freq   The ring modifier frequency
     * @param[in]  ring_mod_depth  The ring modifier depth
     * @param[in]  enable_filter   Removes lower harmonics and creates more of a pitch shifting effect (less crazy)
     */
    fx_ring_mod(float ring_mod_freq, float ring_mod_depth, bool enable_filter) : 
        node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_RING_MOD_PARAM_ID_FREQ), 
        node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_RING_MOD_PARAM_ID_DEPTH) {

      // Set parameters
      param_freq = ring_mod_freq;
      param_depth = ring_mod_depth;
      param_enable_filter = enable_filter;

      init();
    }

    /**
     * @brief      Enable the ring modulator (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_RING_MOD, instance_id, T_BOOL, FX_RING_MOD_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the ring modulator (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_RING_MOD, instance_id, T_BOOL, FX_RING_MOD_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Sets the carrier frequency of the ring moduator (Hz)
     *
     * @param[in]  new_freq  The new frequency
     */
    void set_freq(float new_freq) { 
      CHECK_LAST(new_freq, param_freq);
      if (node_ctrl_freq.connected) {
        return;
      }

      param_freq = new_freq;
      parent_canvas->spi_transmit_param(FX_RING_MOD, instance_id, T_FLOAT, FX_RING_MOD_PARAM_ID_FREQ, (void *) &param_freq);
    }

    /**
     * @brief      Sets the depth of the ring modulator (0.0 -> 1.0)
     *
     * @param[in]  new_depth  The new depth
     */
    void set_depth(float new_depth) { 
      
      CHECK_LAST(new_depth, param_depth);

      if (node_ctrl_depth.connected) {
        return; 
      }

      param_depth = new_depth; 
      parent_canvas->spi_transmit_param(FX_RING_MOD, instance_id, T_FLOAT, FX_RING_MOD_PARAM_ID_DEPTH, (void *) &param_depth);
    }

   
  

    /**
     * @brief  Prints the parameters for the delay effect
     */
    void  print_params(void) {
      char buf[64];
      const char * num;
      String val;

      sprintf(buf," Enabled: %s", param_enabled ? "true" : "false");  Serial.println(buf);
      sprintf(buf," Freq (Hz): %.2f", param_freq);  Serial.println(buf);
      sprintf(buf," Depth: %.2f", param_depth);  Serial.println(buf);

      Serial.println(" Routing:");
      
      Serial.print("  + node_ctrl_freq: ");
      if (node_ctrl_freq.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }

      Serial.print("  + node_ctrl_depth: ");
      if (node_ctrl_depth.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }

      Serial.print("  * node_input: ");      
      if (node_input.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }  

      Serial.print("  * node_output: ");      
      if (node_output.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }  

      Serial.println();
    }

};

#endif // DM_FX_DELAY_H