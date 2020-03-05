// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_DESTRUCTOR_H
#define DM_FX_DESTRUCTOR_H

/**
 * @brief      Effect: Destructor - provides various types of hard and soft destructors for creating different types of distortions
 * 
 * Here's a nice summary of clipping using polynomials to create various types of distortions
 * topic: http://sites.music.columbia.edu/cmc/music-dsp/FAQs/guitar_distortion_FAQ.html
 * 
 * 
 *  Example:
 *   ___destructor_1.c___
 */
class fx_destructor: public fx_effect {

  private:

    // Parameters
    DESTRUCTOR_TYPE  param_type;
    float 					 param_param_1;
    float						 param_param_2;
    float            param_output_gain;
    bool						 param_upsample;

    // Control nodes
    fx_control_node node_ctrl_param_1;
    fx_control_node node_ctrl_param_2;
    fx_control_node node_ctrl_output_gain;

	  void init(void) {

		    // Set class
		    type = FX_DESTRUCTOR;

		    // Set name
		    strcpy(effect_name, "destructor");


		    // Assign programmable node names
		    input = &node_input;
		    output = &node_output;
		    
				// Initialize parameter stack
		    int indx = 1;
		    param_stack[indx] = &param_type;
		    param_stack_types[indx++] = T_INT16;
        param_stack[indx] = &param_param_1;
        param_stack_types[indx++] = T_FLOAT;
		    param_stack[indx] = &param_param_2;
		    param_stack_types[indx++] = T_FLOAT;
		    param_stack[indx] = &param_output_gain;
		    param_stack_types[indx++] = T_FLOAT;
		    total_params = indx;    

		    // Add addititonal nodes to the control stack
		    control_node_stack[total_control_nodes++] = &node_ctrl_param_1;
		    control_node_stack[total_control_nodes++] = &node_ctrl_param_2;
        control_node_stack[total_control_nodes++] = &node_ctrl_output_gain;

		    // Assign controls
		    param_1 = &node_ctrl_param_1;
		    param_2 = &node_ctrl_param_2;
        output_gain = &node_ctrl_output_gain;
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
   * Control routing node [input]: clipping threshold (0.0 -> 1.0)
   */
  fx_control_node * param_1;

  /**
   * Control routing node [input]: input drive multiplier before destructor (up to 64.0)
   */
  fx_control_node * param_2;

  /**
   * Control routing node [input]: output gain (linear)
   */
  fx_control_node * output_gain;


  /**
   * @brief      Basic constructor for the destructor (for models with one parameter)
   *
   * @param[in]  param_1    The first parameter of the destructor (varies by destructor type)
   * @param[in]  clip_type  Destructor function; See `DESTRUCTOR_TYPE` in Special parameters and constants
   */
  fx_destructor(float param_1, DESTRUCTOR_TYPE clip_type) :
    node_ctrl_param_1(NODE_IN, NODE_FLOAT, "node_ctrl_param_1", this, FX_DESTRUCTOR_PARAM_ID_PARAM_1),
    node_ctrl_param_2(NODE_IN, NODE_FLOAT, "node_ctrl_param_2", this, FX_DESTRUCTOR_PARAM_ID_PARAM_2),
    node_ctrl_output_gain(NODE_IN, NODE_FLOAT, "node_ctrl_output_gain", this, FX_DESTRUCTOR_PARAM_ID_OUT_GAIN) {

    // Set parameters
    param_type = clip_type;
    param_param_1 = param_1;
    param_param_2 = 0.0; 
    param_output_gain = 1.0;
    param_upsample = true;

    init();

  } 

  /**
   * @brief      Basic constructor for the destructor (for models with one parameter)
   *
   * @param[in]  param_1        The first parameter of the destructor (varies by destructor type)
   * @param[in]  param_2        The second parameter of the destructor (varies by destructor type)
   * @param[in]  clip_type      Destructor function; See `DESTRUCTOR_TYPE` in Special parameters and constants 
   */
  fx_destructor(float param_1, float param_2, DESTRUCTOR_TYPE clip_type) :
    node_ctrl_param_1(NODE_IN, NODE_FLOAT, "node_ctrl_param_1", this, FX_DESTRUCTOR_PARAM_ID_PARAM_1),
    node_ctrl_param_2(NODE_IN, NODE_FLOAT, "node_ctrl_param_2", this, FX_DESTRUCTOR_PARAM_ID_PARAM_2),
    node_ctrl_output_gain(NODE_IN, NODE_FLOAT, "node_ctrl_output_gain", this, FX_DESTRUCTOR_PARAM_ID_OUT_GAIN) {

    // Set parameters
    param_type = clip_type;
    param_param_1 = param_1;
    param_param_2 = param_2; 
    param_output_gain = 1.0;
    param_upsample = true;

    init();

  } 

  /**
   * @brief      Advanced constructor for the destructor
   *
   * @param[in]  param_1        The first parameter of the destructor (varies by destructor type)
   * @param[in]  param_2        The second parameter of the destructor (varies by destructor type)
   * @param[in]  output_gain    The output stage gain (linear)
   * @param[in]  clip_type      Destructor function; See `DESTRUCTOR_TYPE` in Special parameters and constants 
   */
  fx_destructor(float param_1, float param_2, float output_gain, DESTRUCTOR_TYPE clip_type) :
    node_ctrl_param_1(NODE_IN, NODE_FLOAT, "node_ctrl_param_1", this, FX_DESTRUCTOR_PARAM_ID_PARAM_1),
    node_ctrl_param_2(NODE_IN, NODE_FLOAT, "node_ctrl_param_2", this, FX_DESTRUCTOR_PARAM_ID_PARAM_2),
    node_ctrl_output_gain(NODE_IN, NODE_FLOAT, "node_ctrl_output_gain", this, FX_DESTRUCTOR_PARAM_ID_OUT_GAIN) {

    // Set parameters
    param_type = clip_type;
    param_param_1 = param_1;
    param_param_2 = param_2;
    param_output_gain = output_gain;
    param_upsample = true;

    init();

  } 

  /**
   * @brief      Enable the destructor (it is enabled by default)
   */
  void enable() {
    CHECK_LAST_ENABLED();
    param_enabled = true; 
    parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_BOOL, FX_DESTRUCTOR_PARAM_ID_ENABLED, (void *) &param_enabled);
  }

  /**
   * @brief      Bypass the destructor (will just pass clean audio through)
   */
  void bypass() {
    CHECK_LAST_DISABLED();
    param_enabled = false; 
    parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_BOOL, FX_DESTRUCTOR_PARAM_ID_ENABLED, (void *) &param_enabled);
  }  

  /**
   * @brief      Sets the clipping threshold
   *
   * @param[in]  threshold  The threshold for clipping should be between 0.1 and
   *                        1.0.  A value of 0.1 will provide aggressive clipping where
   *                        as a value of 0.8 will provide more gentle clipping.
   */
  void set_param_1(float new_param_1) { 

    CHECK_LAST(new_param_1, param_param_1);
    
    if (node_ctrl_param_1.connected) {
      return; 
    }

    param_param_1 = new_param_1; 
    parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_FLOAT, FX_DESTRUCTOR_PARAM_ID_PARAM_1, &param_param_1);
  }




  /**
   * @brief      Sets the clipping threshold when using SMOOTH_CLIP, SMOOTHER_CLIP or SMOOTH_FUZZ
   *
   * @param[in]  new_clip  The new clipping threshold (typically around 0.1)
   */
  void set_clipping_threshold(float new_clip) {
    static bool warned = false;
    if (param_type  >= (int) BIT_CRUSHER) {
      if (!warned) {
        Serial.println("Warning: set_clipping_threshold() should only be used with SMOOTH_CLIP, SMOOTHER_CLIP and SMOOTH_FUZZ");
        warned = true;
      }
    }
    set_param_1(new_clip);
  }

  /**
   * @brief      Sets the input drive before the destructor
   *
   * @param[in]  drive  The drive a value that the incoming signal will get
   *                    multiplied by before entering the destructor.
   */
  void set_param_2(float new_param_2) { 

    CHECK_LAST(new_param_2, param_param_2);

    if (node_ctrl_param_2.connected) {
      return; 
    }

    param_param_2 = new_param_2; 
    parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_FLOAT, FX_DESTRUCTOR_PARAM_ID_PARAM_2, &param_param_2);
  }  

  /**
   * @brief      Sets the input drive when using SMOOTH_CLIP, SMOOTHER_CLIP or SMOOTH_FUZZ
   *
   * @param[in]  new_drive  The new input drive (1.0 is no input gain, >1 will drive input signal into saturation)
   */
  void set_input_drive(float new_drive) {
    static bool warned = false;
    if (param_type  >= (int) BIT_CRUSHER) {
      if (!warned) {
        Serial.println("Warning: set_input_drive() should only be used with SMOOTH_CLIP, SMOOTHER_CLIP and SMOOTH_FUZZ");
        warned = true;
      }
    }
    set_param_2(new_drive);
  }


  /**
   * @brief      Sets the output gain of the destructors
   *
   * @param[in]  gain  The gain is the value that will be multiplied at the output stage of the destructor. 
   */
  void set_output_gain(float new_gain) { 

    CHECK_LAST(new_gain, param_output_gain);


    if (node_ctrl_output_gain.connected) {
      return; 
    }

    param_output_gain = new_gain; 
    parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_FLOAT, FX_DESTRUCTOR_PARAM_ID_OUT_GAIN, &param_output_gain);
  }    

  /**
   * @brief      Print the parameters for this effect
   */
  void  print_params(void) {
		Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );
    print_parameter( &param_param_1, "Parameter #1", T_FLOAT );
    print_parameter( &param_param_2, "Parameter #2", T_FLOAT );
		print_parameter( &param_output_gain, "Output gain", T_FLOAT );
		print_parameter( &param_type, "Clipping preset (enumerated)", T_INT16 );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_param_1);
    print_ctrl_node_status(&node_ctrl_param_2);

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);

    Serial.println();
  }    
};
#endif  // DM_FX_DESTRUCTOR_H

