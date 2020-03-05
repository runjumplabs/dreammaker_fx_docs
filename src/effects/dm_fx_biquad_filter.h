// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_BIQUAD_FILTER_H
#define DM_FX_BIQUAD_FILTER_H

/**
 * @brief      Effect: Biquad filter for implementing various types of filters (low
 *             pass, high pass, band pass, etc.)
 *             
 * The biquad filter can be used to create static filters such as equalizers and
 * dynamic filters such as auto-wahs and other interesting swept filtering 
 * effects.
 * 
 * Here's a nice video about three different parameters of a biquad: 
 * f: cutoff/center frequency, q: filter width, and g: filter gain
 * 
 * <iframe width="560" height="315" src="https://www.youtube.com/embed/uTGU9jkkYwk" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
 *             
 */
class fx_biquad_filter: public fx_effect {

  private:

    // Parameters
    // important safety tip - don't put 8-bit enum types together as they need to exist on 16 or 32-bit memory boundaries
    BIQUAD_FILTER_TYPE			param_type;
    float                   param_freq;
    BIQUAD_FILTER_ORDER     param_order;
    float                   param_q;
    float                   param_gain;
    EFFECT_TRANSITION_SPEED param_speed;

	  
	  fx_control_node node_ctrl_freq;
	  fx_control_node node_ctrl_q;
	  fx_control_node node_ctrl_gain;

    void init(void) {

	    // Set class
	    type = FX_BIQUAD_FILTER;

	    // Set name
	    strcpy(effect_name, "biquad filter");


	    // Assign programmable node names
	    input = &node_input;
	    output = &node_output;
	    
			// Initialize parameter stack
	    int indx = 1;
	    param_stack[indx] = &param_type;
	    param_stack_types[indx++] = T_INT16;
	    param_stack[indx] = &param_speed;
	    param_stack_types[indx++] = T_INT16;
	    param_stack[indx] = &param_freq;
	    param_stack_types[indx++] = T_FLOAT;
	    param_stack[indx] = &param_q;
	    param_stack_types[indx++] = T_FLOAT;
	    param_stack[indx] = &param_gain;
	    param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_order;
      param_stack_types[indx++] = T_INT16;
	    total_params = indx;    

	    // Add addititonal nodes to the control stack
	    control_node_stack[total_control_nodes++] = &node_ctrl_freq;
	    control_node_stack[total_control_nodes++] = &node_ctrl_q;
	    control_node_stack[total_control_nodes++] = &node_ctrl_gain;

	    // Assign controls
	    freq = &node_ctrl_freq;
	    q = &node_ctrl_q;
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
   * Control routing node: center/critical frequency of the filter in Hz (i.e. 800.0 for 800Hz)
   */
  fx_control_node * freq;


  /**
   * Control routing node: width of the filter
   */
  fx_control_node * q;


  /**
   * Control routing node: gain of the filter (used in shelving filters)
   */
  fx_control_node * gain;    


	/**
	 * @brief      Basic constructor for biquad filter
	 *
	 * @param[in]  filt_freq   The filter critical/cutoff frequency
	 * @param[in]  filt_resonance  The filter resonance, 1.0 is standard resonance, > 1.0 is more resonant, < 1.0 is less resonant
	 * @param[in]  filt_type   The filter type, see BIQUAD_FILTER_TYPE for options
	 */
  fx_biquad_filter(float filt_freq, float filt_resonance, BIQUAD_FILTER_TYPE filt_type) :
    node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
    node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
    node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {

    // Set parameters
    param_freq = filt_freq;
    if (filt_resonance <= 0.1) {
      filt_resonance = 0.1;
    } else if (filt_resonance > 20.0) {
      filt_resonance = 20.0;
    }
    param_q = filt_resonance * 0.7071;
    param_gain = 0.0;    
    param_type = filt_type; 
    param_speed = TRANS_MED;
    param_order = BIQUAD_ORDER_2;

    init();

  } 

  /**
   * @brief      Basic constructor for biquad filter
   *
   * @param[in]  filt_freq   The filter critical/cutoff frequency
   * @param[in]  filt_resonance  The filter resonance, 1.0 is standard resonance, > 1.0 is more resonant, < 1.0 is less resonant
   * @param[in]  filt_type   The filter type, see BIQUAD_FILTER_TYPE for options
   * @param[in]  order        The order of the filter (higher = tighter)   
   */
  fx_biquad_filter(float filt_freq, float filt_resonance, BIQUAD_FILTER_TYPE filt_type, BIQUAD_FILTER_ORDER order) :
    node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
    node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
    node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {

    // Set parameters
    param_freq = filt_freq;
    if (filt_resonance <= 0.1) {
      filt_resonance = 0.1;
    } else if (filt_resonance > 20.0) {
      filt_resonance = 20.0;
    }
    param_q = filt_resonance * 0.7071;
    param_gain = 0.0;    
    param_type = filt_type; 
    param_speed = TRANS_MED;
    param_order = order;

    init();

  }   


  /**
   * @brief      Advanced constructor for biquad filter
   *
   * @param[in]  filt_freq    The filter critical/cutoff frequency
   * @param[in]  filt_resonance   The filter resonance, 1.0 is standard resonance (0.7071), > 1.0 is more resonant, < 1.0 is less resonant
   * @param[in]  filter_gain  The filter gain in dB
   * @param[in]  filt_type    The filter type, see BIQUAD_FILTER_TYPE for
   *                          options
   * @param[in]  trans_speed  The transaction speed when new filter parameters
   *                          are suppliued, see EFFECT_TRANSITION_SPEED for
   *                          options
   */
  fx_biquad_filter(float filt_freq, float filt_resonance, float filter_gain, BIQUAD_FILTER_TYPE filt_type, EFFECT_TRANSITION_SPEED trans_speed) :
    node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
    node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
    node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {

    // Set parameters
    param_freq = filt_freq;
    if (filt_resonance <= 0.1) {
      filt_resonance = 0.1;
    } else if (filt_resonance > 20.0) {
      filt_resonance = 20.0;
    }
    param_q = filt_resonance * 0.7071;
    param_gain = filter_gain;    
    param_type = filt_type; 
    param_speed = trans_speed;
    param_order = BIQUAD_ORDER_2;

    init();

  } 

  /**
   * @brief      Advanced constructor for biquad filter
   *
   * @param[in]  filt_freq    The filter critical/cutoff frequency
   * @param[in]  filt_resonance  The filter resonance, 1.0 is standard resonance (0.7071), > 1.0 is more resonant, < 1.0 is less resonant
   * @param[in]  filter_gain_db  The filter gain in dB
   * @param[in]  filt_type    The filter type, see BIQUAD_FILTER_TYPE for
   *                          options
   * @param[in]  trans_speed  The transaction speed when new filter parameters
   *                          are suppliued, see EFFECT_TRANSITION_SPEED for
   *                          options
   * @param[in]  order        The order of the filter (higher = tighter)                           
   */
  fx_biquad_filter(float filt_freq, float filt_resonance, float filter_gain_db, BIQUAD_FILTER_TYPE filt_type, EFFECT_TRANSITION_SPEED trans_speed, BIQUAD_FILTER_ORDER order) :
    node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
    node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
    node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {

    // Set parameters
    param_freq = filt_freq;
    if (filt_resonance <= 0.1) {
      filt_resonance = 0.1;
    } else if (filt_resonance > 20.0) {
      filt_resonance = 20.0;
    }
    param_q = filt_resonance * 0.7071;
    param_gain = filter_gain_db;    
    param_type = filt_type; 
    param_speed = trans_speed;
    param_order = order;

    init();

  } 

  /**
   * @brief      Enable the biquad filter (it is enabled by default)
   */
  void enable() {
    CHECK_LAST_ENABLED();
    param_enabled = true; 
    parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_BOOL, FX_BIQUAD_PARAM_ID_ENABLED, (void *) &param_enabled);
  }

  /**
   * @brief      Bypass the biquad filter (will just pass clean audio through)
   */
  void bypass() {
    CHECK_LAST_DISABLED();
    param_enabled = false; 
    parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_BOOL, FX_BIQUAD_PARAM_ID_ENABLED, (void *) &param_enabled);
  }  


  /**
   * @brief      Sets a new cutoff/critical frequency (Hz). 
   *
   * @param[in]  freq  The new center frequency for the filter in Hz (must be lower than 24000.0)
   */
  void set_freq(float freq) { 

    CHECK_LAST(freq, param_freq);

    // If this node is being controlled by a controller, don't allow a direct write to it
    if (node_ctrl_freq.connected) {
      return; 
    }

    param_freq = freq; 
    parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_FREQ, &param_freq);
  }

  /**
   * @brief      Sets a new Q factor for the filter.  For more information on Q
   *             factor, read this: https://en.wikipedia.org/wiki/Q_factor
   *
   * @param[in]  q     The Q factor (must be between 0.01 and 100.0)
   */
  void set_q(float q) { 

    CHECK_LAST(q, param_q)

    if (node_ctrl_q.connected) {
      return; 
    }

    param_q = q; 
    parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_Q, &param_q);
  }    

  /**
   * @brief      Sets the resonance; 1.0 is none (0.7071)
   *
   * @param[in]  resonance     The resonance (must be between 0.01 and 100.0)
   */
  void set_resonance(float filt_resonance) { 

    if (node_ctrl_q.connected) {
      return; 
    }

    if (filt_resonance <= 0.1) {
      filt_resonance = 0.1;
    } else if (filt_resonance > 20.0) {
      filt_resonance = 20.0;
    }

    filt_resonance = filt_resonance * 0.7071;

    CHECK_LAST(filt_resonance, param_q);

    param_q = filt_resonance * 0.7071;

    parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_Q, &param_q);
  }   

  /**
   * @brief      Sets the filter gain.  This is only used in shelving filters
   *
   * @param[in]  gain  The gain in dB
   */
  void set_gain(float gain) { 

    CHECK_LAST(gain, param_gain);
    if (node_ctrl_gain.connected) {
      return; 
    }

    param_gain = gain; 
    parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_GAIN, &param_gain);
  }   

  /**
   * @brief      Print the parameters for this effect
   */
  void  print_params(void) {

		// void print_parameter( void * val, char * name, PARAM_TYPES type)
		Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );
    print_parameter( &param_freq, "Frequency (Hz)", T_FLOAT );
    print_parameter( &param_q, "Width/Q", T_FLOAT );
		print_parameter( &param_gain, "Gain (db)", T_FLOAT );
    print_parameter( &param_order, "Order (enumerated))", T_INT16 );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_freq);
    print_ctrl_node_status(&node_ctrl_q);
    print_ctrl_node_status(&node_ctrl_gain);

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);

    Serial.println();
  }    
};
#endif 	// DM_FX_BIQUAD_FILTER_H