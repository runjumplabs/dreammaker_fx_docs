// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_PHASE_SHIFTER_H
#define DM_FX_PHASE_SHIFTER_H

/**
 * @brief      Effect: Phase shifter for creating rich phase shifts
 * 
 * These create a series of phase shifts connected to an LFO.  Phase shifters were pioneered probably in the 1960s and 1970s and create a familiar effect.  Search for phase shifter pedals on youtube for some examples of what they sound like.  
 * 
 * Internally, the phase shifter relies on eight notch filters that are swept with frequencies at  100.0, 330.0, 500.0, 700.0, 900.0, 1500.0, 2000.0, 2500.0Hz.  These location of these filters and depth of modulation for each is programmable and will be exposed in an upcoming release.
 * 
 * Phase shifters create a particularly pronounced effect when the audio running through them has higher frequency components.  They sound pretty interesting after a distortion or a synth sound.
 * 
 * Here's an example that creates a basic phase shifter pedal.
 * 
 * ```CPP
 * #include <dreammakerfx.h>
 * 
 * fx_phase_shifter    phase_shift(1.0,    // 1Hz
 *                                 0.7,    // 0.7 depth
 *                                 0.4,    // 0.4 feedback
 *                                 0.0,    // 0 degrees initial phase
 *                                 OSC_TRIANGLE);  // Modulated with triangle wave
 * 
 * void setup() {
 *   // put your setup code here, to run once:
 * 
 *   // Initialize the pedal!
 *   pedal.init();
 * 
 *   // Route audio from instrument -> pitch_shift -> amp
 *   pedal.route_audio(pedal.instr_in, phase_shift.input); 
 *   pedal.route_audio(phase_shift.output, pedal.amp_out);
 * 
 *   // left footswitch is bypass
 *   pedal.add_bypass_button(FOOTSWITCH_LEFT);
 * 
 *    // Run this effect
 *   pedal.run();
 * }
 * 
 * void loop() {
 * 
 *   // Left pot changes the depth
 *   if (pedal.pot_left.has_changed()) {
 *     phase_shift.set_depth(pedal.pot_left.val);
 *   }
 *   
 *   // Center pot changes the modulation rate
 *   if (pedal.pot_center.has_changed()) {
 *      phase_shift.set_rate_hz(0.5+pedal.pot_center.val*4);
 *   }
 * 
 *   // Right pot changes the feedback
 *   if (pedal.pot_right.has_changed()) {
 *     phase_shift.set_feedback(2.0*pedal.pot_right.val-1.0);   
 *   }    
 *   
 *   // Service pedal
 *   pedal.service();
 * }
 *  
 * ```
 * 
 */
class fx_phase_shifter: public fx_effect {

  private:

    // Parameters
    float param_depth;
    float param_rate_hz;
    float param_feedback;
    float param_initial_phase_deg;
    OSC_TYPES param_type;


    void init() {
	    // Set class
	    type = FX_PHASE_SHIFTER;

	    // Set name
	    strcpy(effect_name, "phase shifter");      

			// Assign programmable node names
	    input = &node_input;
	    output = &node_output;

		  // Initialize parameter stack
	    int indx = 1;
	    param_stack[indx] = &param_rate_hz;
	    param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_depth;
      param_stack_types[indx++] = T_FLOAT;

	    param_stack[indx] = &param_feedback;
	    param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_initial_phase_deg;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_type;
      param_stack_types[indx++] = T_INT16;      

	    total_params = indx;         

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_depth;
      control_node_stack[total_control_nodes++] = &node_ctrl_rate_hz;
      control_node_stack[total_control_nodes++] = &node_ctrl_feedback;

	    // Assign controls
	    depth = &node_ctrl_depth;
	    rate_hz = &node_ctrl_rate_hz;
	    feedback = &node_ctrl_feedback;
	    
	  } 

	  // Control nodes
	  fx_control_node node_ctrl_depth;
	  fx_control_node node_ctrl_rate_hz;
	  fx_control_node node_ctrl_feedback;

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
     * Control routing node: phase shifter depth (should be between 0.0 and 1.0)
     */
    fx_control_node * depth;
    /**
     * Control routing node: phase shifter rate (Hz) (i.e. 1.0 = once per second)
     */
    fx_control_node * rate_hz;
    /**
     * Control routing node: phase shifter feedback (should be between -1.0 and 1.0)
     */
    fx_control_node * feedback;

    /**
     * @brief      Basic constructor/initializer for the phase shifter
     *
     * @param[in]  rate_hz   The rate hz of the LFO modulating the phase shifter
     * @param[in]  depth     The depth of the phase shifter
     * @param[in]  feedback  The feedback of the phase shifter
     */
    fx_phase_shifter(float rate_hz, float depth, float feedback) :
        node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_PHASE_SHIFTER_PARAM_ID_DEPTH),
        node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_PHASE_SHIFTER_PARAM_ID_RATE_HZ),
        node_ctrl_feedback(NODE_IN, NODE_FLOAT, "node_ctrl_feedback", this, FX_PHASE_SHIFTER_PARAM_ID_FEEDBACK) {

        // Set parameters
        param_depth = depth;
        param_rate_hz = rate_hz;
        param_feedback = feedback;
        param_initial_phase_deg = 0.0;
        param_type = OSC_SINE;

        init();

    }

    /**
     * @brief      Constructs a new instance.
     *
     * @param[in]  rate_hz       The rate hz of the LFO modulating the phase shifter
     * @param[in]  depth         The depth of the phase shifter (0.0 -> 1.0)
     * @param[in]  feedback      The feedback of the phase shifter (-1.0 -> 1.0)
     * @param[in]  inital_phase  The inital phase in degrees of the LFO
     * @param[in]  mod_type      The modifier type (OSC_TYPES)
     */
    fx_phase_shifter(float rate_hz, float depth, float feedback, float inital_phase, OSC_TYPES mod_type) :
      node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_PHASE_SHIFTER_PARAM_ID_DEPTH),
      node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_PHASE_SHIFTER_PARAM_ID_RATE_HZ),
      node_ctrl_feedback(NODE_IN, NODE_FLOAT, "node_ctrl_feedback", this, FX_PHASE_SHIFTER_PARAM_ID_FEEDBACK) {

        // Set parameters
        param_depth = depth;
        param_rate_hz = rate_hz;
        param_feedback = feedback;
        param_initial_phase_deg = inital_phase;
        param_type = mod_type;

        init();

    }

 
    /**
     * @brief      Enable the phase shifter (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_PHASE_SHIFTER, instance_id, T_BOOL, FX_PHASE_SHIFTER_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the phase shifter (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_PHASE_SHIFTER, instance_id, T_BOOL, FX_PHASE_SHIFTER_PARAM_ID_ENABLED, (void *) &param_enabled);
    }  

    /**
     * @brief      Sets the depth of the phase shifter
     *
     * @param[in]  depth  The depth fom 0.0 -> 1.0.  0.0 is no modulation at all,
     *                    1.0 is full modulation.
     */
    void set_depth(float depth) { 

      CHECK_LAST(depth, param_depth);

      if (node_ctrl_depth.connected) {
        return; 
      }

      param_depth = depth; 
      parent_canvas->spi_transmit_param(FX_PHASE_SHIFTER, instance_id, T_FLOAT, FX_PHASE_SHIFTER_PARAM_ID_DEPTH, &param_depth);
    }

    /**
     * @brief      Sets the rate of the phase shifter in Hertz (cycles per second)
     *
     * @param[in]  rate_hz  The rate hz
     */
    void set_rate_hz(float rate_hz) { 

      CHECK_LAST(rate_hz, param_rate_hz);

      if (node_ctrl_rate_hz.connected) {
        return; 
      }

      param_rate_hz = rate_hz; 
      parent_canvas->spi_transmit_param(FX_PHASE_SHIFTER, instance_id, T_FLOAT, FX_PHASE_SHIFTER_PARAM_ID_RATE_HZ, &param_rate_hz);
    }    

    /**
     * @brief      Sets the feedback of the phase shifter 
     *
     * @param[in]  feedback  Feedback value (between -1.0 and 1.0)
     */
    void set_feedback(float feedback) { 

      CHECK_LAST(feedback, param_feedback);

      if (node_ctrl_rate_hz.connected) {
        return; 
      }

      param_feedback = feedback; 
      parent_canvas->spi_transmit_param(FX_PHASE_SHIFTER, instance_id, T_FLOAT, FX_PHASE_SHIFTER_PARAM_ID_FEEDBACK, &param_feedback);
    }    

    /**
     * @brief      Sets the the type of oscillator used as the LFO
     *
     * @param[in]  new_type  The new type of LFO (OSC_TYPES)
     */
    void set_lfo_type(OSC_TYPES new_type) {

      CHECK_LAST(new_type, param_type);

      param_type = new_type; 
      parent_canvas->spi_transmit_param(FX_PHASE_SHIFTER, instance_id, T_INT16, FX_PHASE_SHIFTER_PARAM_ID_MOD_TYPE, &param_type);

    }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /**
   * @brief      Print the parameters for this effect
   */
  void  print_params(void) {

    // void print_parameter( void * val, char * name, PARAM_TYPES type)
    Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );
    print_parameter( &param_depth, "Depth", T_FLOAT );
    print_parameter( &param_rate_hz, "Rate (Hz)", T_FLOAT );
    print_parameter( &param_feedback, "Feedback", T_FLOAT );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_depth);
    print_ctrl_node_status(&node_ctrl_rate_hz);
    print_ctrl_node_status(&node_ctrl_feedback);

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);

    Serial.println();
  }    
#endif
};

#endif // DM_FX_PHASE_SHIFTER_H