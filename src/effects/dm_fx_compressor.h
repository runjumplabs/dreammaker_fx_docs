// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_COMPRESSOR_H
#define DM_FX_COMPRESSOR_H


/**
 * @brief      Effect: Compressor/Limiter
 * 
 * Think of a compressor as a small robot that controls a volume knob based on how loud you’re playing.  When you strike a loud chord, the robot immediately turns the volume down and as the chord rings out, the robot turns the volume up progressively, so it sounds like you’re just sustaining the chord.  Instead of dying off, it sounds steady for a few seconds as the robot is turning up the volume.  Compressors are used a lot with acoustic instruments and vocals but also with electric guitars too.  A common in country music is running a Telecaster through a compressor.
 *
 * ``` CPP
 * 
 * #include <dreammakerfx.h>
 * 
 * fx_compressor compressor_1(-30.0,    // Initial threshold in dB
 *                            8,        // Initial ratio (1:8)
 *                            10.0,     // Attack (10ms)
 *                            100.0,    // Release (100ms)
 *                            2.0);     // Initial output gain (2x);
 * 
 * void setup() {
 *   pedal.init(); // Initialize pedal
 *  
 *   // Route audio through effects
 *   pedal.route_audio(pedal.instr_in, compressor_1.input);
 *   pedal.route_audio(compressor_1.output, pedal.amp_out);
 *   
 *   pedal.add_bypass_button(FOOTSWITCH_LEFT); // Use left footswitch/LED to bypass effect 
 * 
 *   pedal.run();  // Run effects
 * }
 * 
 * void loop() {
 *   // Run pedal service to take care of stuff
 *   pedal.service(); 
 * 
 *   if (pedal.pot_left.has_changed()) {.   // Left pot sets threshold from -20dB to -70dB
 *     compressor_1.set_threshold(-20 – (50.0 * pedal.pot_left.val); 
 *   }
 *   if (pedal.pot_center.has_changed()) {    // Center pot sets compression ration from 1:1 to 40:1
 *     compressor_1.set_ratio(1.0+ (40.0 * pedal.pot_center.val)); 
 *   }
 *  if (pedal.pot_right.has_changed()) {    // Right pot sets output gain from 1.0 to 6.0
 *     compressor_1.set_output_gain(1.0 + pedal.pot_right.val*5.0); 
 *   }
 * }
 *  
 * 
 * ```
 *
 * There are several cool things to do with compressors: Add a compressor on either side of a clipper to create more dynamics, run two compressors through a LPF and HPF to create a multi-band compressor (where low end and high end are compressed independently), vary compressor parameters with an LFO to get some wild sounds. 
 *  
 */	
class fx_compressor: public fx_effect {

  private:

  	// Parameters
  	float	param_threshold;
  	float	param_ratio;
  	float	param_attack;
  	float param_release;
  	float param_gain_out;

		// Control nodes
    fx_control_node node_ctrl_threshold;
    fx_control_node node_ctrl_ratio;
    fx_control_node node_ctrl_attack;
    fx_control_node node_ctrl_release;
    fx_control_node node_ctrl_out_gain;

	  void init(void) {

	    // Set class
	    type = FX_COMPRESSOR;

	    // Set name
	    strcpy(effect_name, "compressor");


	    // Assign programmable node names
	    input = &node_input;
	    output = &node_output;
	    
			// Initialize parameter stack
	    int indx = 1;
	    param_stack[indx] = &param_threshold;
	    param_stack_types[indx++] = T_FLOAT;
	    param_stack[indx] = &param_ratio;
	    param_stack_types[indx++] = T_FLOAT;
	    param_stack[indx] = &param_attack;
	    param_stack_types[indx++] = T_FLOAT;
	    param_stack[indx] = &param_release;
	    param_stack_types[indx++] = T_FLOAT;
	    param_stack[indx] = &param_gain_out;
	    param_stack_types[indx++] = T_FLOAT;    
	    total_params = indx;    

	    // Add addititonal nodes to the control stack
	    control_node_stack[total_control_nodes++] = &node_ctrl_threshold;
	    control_node_stack[total_control_nodes++] = &node_ctrl_ratio;
	    control_node_stack[total_control_nodes++] = &node_ctrl_attack;
	    control_node_stack[total_control_nodes++] = &node_ctrl_release;
	    control_node_stack[total_control_nodes++] = &node_ctrl_out_gain;

	    // Assign controls
	    threshold = &node_ctrl_threshold;
	    ratio = &node_ctrl_ratio;
	    attack = &node_ctrl_attack;
	    release = &node_ctrl_release;
	    out_gain = &node_ctrl_out_gain;

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
     * Control routing node [input]: Compressor/limiter threshold in dB (i.e. -30.0)
     */        
    fx_control_node * threshold;


    /**
     * Control routing node [input]: Compressor/limiter compression ratio (a value of 100.0 would be a ratio of 1:100)
     */        
    fx_control_node * ratio;

    /**
     * Control routing node [input]: Compressor/limiter attack rate in milliseconds
     */        
    fx_control_node * attack;

    /**
     * Control routing node [input]: Compressor/limiter release rate in milliseconds
     */        
    fx_control_node * release;

    /**
     * Control routing node [input]: Compressor/limiter output gain (linear value so a value of 2.0 would double the signal amplitude)
     */        
    fx_control_node * out_gain;


	 /**
    * @brief      Constructs a new instance.
    *
    * @param[in]  thresh    Where the robot starts turning down the volume.  This value is in decibels so a good place to start is between -60.0 and -30.0  
    * @param[in]  ratio     How aggressively the robot will turn down the volume when the input exceeds the threshold.  Values from 2-16 create a softer effect.  A very high value of 100.0 creates a hard ceiling.
    * @param[in]  attack    Time in milliseconds for robot to respond when a note exceeds the threshold.  Setting this to 20-30 will allow a bit of a peak to sneak through.
    * @param[in]  release   how long before the robot stops controlling volume after volume goes below threshold
    * @param[in]  gain_out  output volume (from 1.0 and up)
    */
	  fx_compressor(float thresh, float ratio, float attack, float release, float gain_out):
	    node_ctrl_threshold(NODE_IN, NODE_FLOAT, "node_ctrl_threshold", this, FX_COMPRESSOR_PARAM_ID_THRESH),
	    node_ctrl_ratio(NODE_IN, NODE_FLOAT, "node_ctrl_ratio", this, FX_COMPRESSOR_PARAM_ID_RATIO),
	    node_ctrl_attack(NODE_IN, NODE_FLOAT, "node_ctrl_threshold", this, FX_COMPRESSOR_PARAM_ID_ATTACK),
	    node_ctrl_release(NODE_IN, NODE_FLOAT, "node_ctrl_release", this, FX_COMPRESSOR_PARAM_ID_RELEASE),
	    node_ctrl_out_gain(NODE_IN, NODE_FLOAT, "node_ctrl_out_gain", this, FX_COMPRESSOR_PARAM_ID_OUT_GAIN) {

	    	param_threshold = thresh;
	    	param_ratio = ratio;
	    	param_attack = attack;
	    	param_release = release;
	    	param_gain_out = gain_out;

	    	init();

    }


    /**
     * @brief      Enable the __this_effect__ (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_BOOL, FX_COMPRESSOR_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the __this_effect__ (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_BOOL, FX_COMPRESSOR_PARAM_ID_ENABLED, (void *) &param_enabled);
    }  

    /**
     * @brief      Sets the compressor threshold
     *
     * @param[in]  threshold  The threshold is where the robot starts turning down the volume.  This value is in decibels so a good place to start is between -60.0 and -30.0  
     */
    void set_threshold(float threshold) { 
      CHECK_LAST(threshold, param_threshold);

      if (node_ctrl_threshold.connected) {
        return; 
      }

      param_threshold = threshold; 
      parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_THRESH, &param_threshold);
    }  

    /**
     * @brief      Sets the compression ratio
     *
     * @param[in]  ratio  The ratio is how aggressively the robot will turn down the volume when the input exceeds the threshold.  Values from 2-16 create a softer effect.  A very high value of 100.0 creates a hard ceiling.
     */
    void set_ratio(float ratio) { 
      CHECK_LAST(ratio, param_ratio);

      if (node_ctrl_ratio.connected) {
        return; 
      }
      param_ratio = ratio; 
      parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_RATIO, &param_ratio);
    }  

    /**
     * @brief      Sets the time it takes for the compressor to be fully engaged after volume exceeds threshold.
     *
     * @param[in]  attack  The attack is the time in milliseconds for robot to respond when a note exceeds the threshold.  Setting this to 20-30 will allow a bit of a peak to sneak through.
     */
    void set_attack(float attack) { 
      CHECK_LAST(attack, param_attack);

      if (node_ctrl_attack.connected) {
        return; 
      }
      param_attack = attack; 
      parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_ATTACK, &param_attack);
    }    

    /**
     * @brief      Sets the time it takes for the compressor to release the volume control when the volume goes back below the threshold
     *
     * @param[in]  release  The release is the time in milliseconds for robot to respond when a note falls below the threshold.
     */
    void set_release(float release) { 
      CHECK_LAST(release, param_release);

      if (node_ctrl_release.connected) {
        return; 
      }
      param_release = release; 
      parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_RELEASE, &param_release);
    }  

    /**
     * @brief      Sets the output gain of the compressor
     *
     * @param[in]  gain_out  The gain out (typically 1.0 for no gain adjustment and higher to increase gain)
     */
    void set_output_gain(float gain_out) { 
      CHECK_LAST(gain_out, param_gain_out);
      
      if (node_ctrl_out_gain.connected) {
        return; 
      }
      param_gain_out = gain_out; 
      parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_OUT_GAIN, &param_gain_out);
    }  

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    void  print_params(void) {

  		// void print_parameter( void * val, char * name, PARAM_TYPES type)
  		Serial.println("Parameters:");
      print_parameter( &param_enabled, "Enabled", T_BOOL );
      print_parameter( &param_threshold, "Threshold (db)", T_FLOAT );
      print_parameter( &param_ratio, "Ratio", T_FLOAT );
  		print_parameter( &param_attack, "Attack time (ms)", T_FLOAT );
  		print_parameter( &param_release, "Release (ms)", T_FLOAT );
  		print_parameter( &param_gain_out, "Output gain", T_INT16 );

      Serial.println("Control Routing:");      
      print_ctrl_node_status(&node_ctrl_threshold);
      print_ctrl_node_status(&node_ctrl_ratio);
      print_ctrl_node_status(&node_ctrl_attack);
      print_ctrl_node_status(&node_ctrl_release);
      print_ctrl_node_status(&node_ctrl_out_gain);

      Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);
      print_audio_node_status(&node_output);

      Serial.println();
    }    
#endif
};

#endif 	// DM_FX_COMPRESSOR_H