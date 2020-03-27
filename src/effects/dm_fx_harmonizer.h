// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_HARMONIZER_H
#define DM_FX_HARMONIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define PARAM_STACK(PARAM, TYPE)  param_stack[total_params] = &PARAM ; param_stack_types[total_params++] = TYPE; 
#define CTRL_STACK(NODE, ALIAS)  control_node_stack[total_control_nodes++] = &NODE; ALIAS = &NODE ;
#define CHECK_NODE_CONNECTED(NODE)  if (  NODE.connected) { return; }

/**
 * @brief      Effect: Harmonizer - works in conjuction with fx_pitch_shift_fd to create harmonies
 * 
 * Not yet fully supported
 * 
 * 
 */
class fx_harmonizer: public fx_effect {
  
  private:

    // Parameters
    int16_t     param_offset;
    MUSIC_KEY 	param_key;
    float       param_vol;
    MUSIC_MODE 	param_mode;

    // Control nodes
    fx_control_node node_ctrl_key;
    fx_control_node node_ctrl_mode;
	  fx_control_node node_ctrl_offset;    
    fx_control_node node_ctrl_vol_in;
    fx_control_node node_ctrl_vol_out;
    fx_control_node node_ctrl_freq_out;

    void init(void) {

      // Set class
      type = FX_HARMONIZER;

      // Set name
      strcpy(effect_name, "harmonizer");

      PARAM_STACK(param_key,    T_INT16);
      PARAM_STACK(param_mode,   T_INT16);
      PARAM_STACK(param_offset, T_INT16);
      PARAM_STACK(param_vol,    T_FLOAT);

      CTRL_STACK(node_ctrl_key,       key);
      CTRL_STACK(node_ctrl_mode,      mode);
      CTRL_STACK(node_ctrl_offset,    offset);
      CTRL_STACK(node_ctrl_vol_in,    vol_in);
      CTRL_STACK(node_ctrl_vol_out,   vol_out);
      CTRL_STACK(node_ctrl_freq_out,  freq_out);

    }

  public:
    /**
     * Control routing node: The key being played in (of type MUSIC_KEY).
     */
    fx_control_node * key;

    /**
     * Control routing node: The mode being played in (of type MUSIC_MODE).
     */
    fx_control_node * mode;

    /**
     * Control routing node: The offset from the note being played (steps in the scale, not semitones).
     */
    fx_control_node * offset;

    /**
     * Control routing node: The volume of the harmonized audio
     */
    fx_control_node * vol_in;

    /**
     * Control routing node: The harmonized frequency (connect to fx_pitch_shift_fd)
     */
    fx_control_node * freq_out;

    /**
     * Control routing node: The harmonized volume (connect to fx_pitch_shift_fd)
     */
    fx_control_node * vol_out;


    /**
     * @brief      Constructor for the harmonizer
     *
     * Be sure to use this harmonizer in conjunction with fx_pitch_shift_fd per the example above.
     *
     * @param[in]  harm_key     The key (see MUSIC_KEY type)
     * @param[in]  harm_mode    The mode (see MUSIC_MODE type)
     * @param[in]  harm_offset  The number of steps in the scale to harmonize on.  1 would be 2nd, 2 would be 3rd, 3 would be fourth, -1 would be 7th (lower)
     * @param[in]  harm_volume  The volume of harmonized notes
     */
    fx_harmonizer( MUSIC_KEY harm_key, MUSIC_MODE harm_mode, int16_t harm_offset, float harm_volume) : 
      node_ctrl_key(NODE_IN, NODE_INT32, "node_ctrl_key", this, FX_HARMONIZER_PARAM_ID_KEY),
      node_ctrl_mode(NODE_IN, NODE_INT32, "node_ctrl_mode", this, FX_HARMONIZER_PARAM_ID_MODE),
      node_ctrl_offset(NODE_IN, NODE_INT32, "node_ctrl_offset", this, FX_HARMONIZER_PARAM_ID_OFFSET),
      node_ctrl_vol_in(NODE_IN, NODE_FLOAT, "node_ctrl_vol_in", this, FX_HARMONIZER_PARAM_ID_VOL),
      node_ctrl_freq_out(NODE_OUT, NODE_FLOAT, "node_ctrl_freq_out", this, FX_HARMONIZER_PARAM_ID_FREQ_OUT),
      node_ctrl_vol_out(NODE_OUT, NODE_FLOAT, "node_ctrl_vol_out", this, FX_HARMONIZER_PARAM_ID_VOL_OUT) 
      {

      	param_key = harm_key;
      	param_mode = harm_mode;
      	param_offset = harm_offset;
      	param_vol = harm_volume;

        init();      	

      }

    /**
     * @brief      Sets the key of the harmonizer.
     *
     * @param[in]  new_key  The new key of type MUSIC_KEY
     */
    void set_key(MUSIC_KEY new_key) { 

      CHECK_LAST(new_key, param_key);
      CHECK_NODE_CONNECTED(node_ctrl_key);

      param_key = new_key;  
      parent_canvas->spi_transmit_param(FX_HARMONIZER, instance_id, T_INT16, FX_HARMONIZER_PARAM_ID_KEY, &param_key);
    }      

  /**
   * @brief  Prints the parameters for the delay effect
   */
  void  print_params(void) {

    // void print_parameter( void * val, char * name, PARAM_TYPES type)
    Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );

    print_parameter( &param_key, "Key", T_INT16 );
    print_parameter( &param_mode, "Mode", T_INT16 );
    print_parameter( &param_offset, "Offset", T_INT16 );
    print_parameter( &param_vol, "Volume", T_FLOAT );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_key);
    print_ctrl_node_status(&node_ctrl_mode);
    print_ctrl_node_status(&node_ctrl_offset);
    print_ctrl_node_status(&node_ctrl_vol_in);
    print_ctrl_node_status(&node_ctrl_freq_out);
    print_ctrl_node_status(&node_ctrl_vol_out);


    Serial.println();
  }


};
#endif // DOXYGEN_SHOULD_SKIP_THIS
#endif // 	DM_FX_HARMONIZER_H