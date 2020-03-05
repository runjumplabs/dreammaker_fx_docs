// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_DELAY_H
#define DM_FX_DELAY_H


/**
 * @brief      Effect: Delay/echo 
 * 
 * A delay effect is basically an echo machine.  Unlike other delay pedals, we have a massive amount of delay memory so you can create delays that are several seconds long.  Also, this delay block allows you to add your own effects to the “feedback” path of the echo so each echo can run though an effects chain.  Put a pitch shifter in here and each echo changes pitch.  Add a phase shifter and each echo gets progressively “phasey”.  Put another echo effect in there and create effects like the movie Inception.
 *
 * This example creates a delay and places a low-pass dampening filter in the feedback loop so each echo gets darker and darker.  
 * 
 * ``` CPP
 * 
 * #include <dreammakerfx.h>
 * 
 * fx_delay    delay_1(1000.0, // Initial delay length of 1 second / 1000ms
 *                          5000.0, // Max delay of 5 seconds
 *                          0.7,    // Initial feedback value of 0.7
 *                          1.0,    // Clean mix
 *                          0.7,    // Delay / echo mix
 *                          true);  // Enable fx send/receive loop
 * 
 * fx_biquad_filter  fb_filt(1200.0,                // 1200 Hz starting frequency
 *                           FILTER_WIDTH_NORMAL,   // Width of the filter is narrow
 *                           BIQUAD_TYPE_LPF);      // Type is low-pass
 * 
 * void setup() {
 *   pedal.init(); // Initialize pedal
 *   
 *   // Route audio through effects
 *   pedal.route_audio(pedal.instr_in, delay_1.input);
 *   pedal.route_audio(delay_1.output, pedal.amp_out);
 *   
 *   // Route filter through delay fx send/receive loop
 *   pedal.route_audio(delay_1.fx_send, fb_filt.input);
 *   pedal.route_audio(fb_filt.output, delay_1.fx_receive);
 * 
 *   pedal.add_bypass_button(FOOTSWITCH_LEFT); // Use left footswitch/LED to bypass effect 
 *   
 *   pedal.run();    // Run effects
 * }
 * 
 * void loop() {
 *   pedal.service(); // Run pedal service to take care of stuff
 * }
 * 
 * ```
 * 
 * There are lots of cool things you can do with delays: Create a set of delays in parallel with lengths (1000ms, 750ms, 333ms) to create cool rhythmic echoes, create elaborate effects chains in the delay’s feedback loop, add delays into the feedback fx send/receive loop of the delay, control a filter from a delayed version of a signal
 *  */
class fx_delay: public fx_effect {

  private:

    // Delay parameters
    float param_len_ms;
    float param_len_max_ms;
    float param_feedback;
    float param_wet_mix;
    float param_dry_mix;
    bool  param_ext_fb_processing;  


    // Additional audio nodes
    fx_audio_node node_delay_tx;
    fx_audio_node node_delay_rx;

    // Control nodes
    fx_control_node node_ctrl_len_ms;
    fx_control_node node_ctrl_feedback;
    fx_control_node node_ctrl_dry_mix;
    fx_control_node node_ctrl_wet_mix;


    void init(void) {
      
      // Set class
      type = FX_DELAY;

      // Set name
      strcpy(effect_name, "delay");

      // Assign programmable node names
      input = &node_input;
      output = &node_output;
      fx_send = &node_delay_tx;
      fx_receive = &node_delay_rx;

      // Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_len_ms;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_len_max_ms;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_feedback;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_dry_mix;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_wet_mix;
      param_stack_types[indx++] = T_FLOAT;

      param_stack[indx] = &param_ext_fb_processing;
      param_stack_types[indx++] = T_BOOL;
      total_params = indx;

      // Add additional nodes to the audio stack
      audio_node_stack[total_audio_nodes++] = &node_delay_rx;
      audio_node_stack[total_audio_nodes++] = &node_delay_tx;    

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_len_ms;
      control_node_stack[total_control_nodes++] = &node_ctrl_feedback;
      control_node_stack[total_control_nodes++] = &node_ctrl_dry_mix;
      control_node_stack[total_control_nodes++] = &node_ctrl_wet_mix;

      length_ms = &node_ctrl_len_ms;
      feedback = &node_ctrl_feedback;
      dry_mix = &node_ctrl_dry_mix;
      wet_mix = &node_ctrl_wet_mix;

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
     * Audio routing node [output]: effect loop send before entering delay line of this effect
     * 
     * ``` CPP
     *  
     *   // Route audio through effects
     *   pedal.route_audio(pedal.instr_in, delay_1.input);
     *   pedal.route_audio(delay_1.output, pedal.amp_out);
     *   
     *   // Route filter through delay fx send/receive loop
     *   pedal.route_audio(delay_1.fx_send, fb_filt.input);
     *   pedal.route_audio(fb_filt.output, delay_1.fx_receive);
     *   
     * ```  
     */
    fx_audio_node * fx_send;

    /**
     * Audio routing node [output]: effect loop return before entering delay line of this effect
     * 
     * ``` CPP
     *  
     *   // Route audio through effects
     *   pedal.route_audio(pedal.instr_in, delay_1.input);
     *   pedal.route_audio(delay_1.output, pedal.amp_out);
     *   
     *   // Route filter through delay fx send/receive loop
     *   pedal.route_audio(delay_1.fx_send, fb_filt.input);
     *   pedal.route_audio(fb_filt.output, delay_1.fx_receive);
     *   
     * ``` 
     */
    fx_audio_node * fx_receive;


    /**
     * Control routing node [input]: Length of delay line in milliseconds (1/1000s of a second)
     */        
    fx_control_node * length_ms;

    /**
     * Control routing node [input]: Feedback ratio (between 0.0 and 1.0)
     */            
    fx_control_node * feedback;

    /**
     * Control routing node [input]: Dry mix (between 0.0 and 1.0)
     */            
    fx_control_node * dry_mix;

    /**
     * Control routing node [input]: Wet mix (between 0.0 and 1.0)
     */            
    fx_control_node * wet_mix;



    /**
     * @brief      Basic constructor for delay effect
     *
     * ``` CPP
     * 
     * // Set up a basic 1 second echo
     * fx_delay    delay_1(1000.0, // Initial delay length of 1 second / 1000ms
     *                     0.7);    // Initial feedback value of 0.7
     *
     * ```
     *
     * @param[in]  delay_len_ms   The length of the echo in milliseconds (1000.0 milliseconds = 1 second).  For the advanced constructor, the delay_len_max_ms determines the total memory allocated for this delay and will be the max length.  In the basic constructor, the initial length is also the maximum delay length.
     * @param[in]  feedback       How much of the output is feedback to the input.  A value of 0.0 will product a single delay.  A value of 1.0 will produce endless echoes.  0.5-0.7 is a nice decaying echo.
     *                            
     */
    fx_delay(float delay_len_ms, float feedback) : 
        node_delay_tx(NODE_OUT, "delay_fb_tx", this), 
        node_delay_rx(NODE_IN, "delay_fb_rx", this),
        node_ctrl_len_ms(NODE_IN, NODE_FLOAT, "node_ctrl_len_ms", this, FX_DELAY_PARAM_ID_LEN_MS), 
        node_ctrl_feedback(NODE_IN, NODE_FLOAT, "node_ctrl_feedback", this, FX_DELAY_PARAM_ID_FEEDBACK), 
        node_ctrl_dry_mix(NODE_IN, NODE_FLOAT, "node_ctrl_dry_mix", this, FX_DELAY_PARAM_ID_DRY_MIX),
        node_ctrl_wet_mix(NODE_IN, NODE_FLOAT, "node_ctrl_wet_mix", this, FX_DELAY_PARAM_ID_WET_MIX) {

        // Set parameters
        param_len_ms = delay_len_ms;
        param_len_max_ms = delay_len_ms;
        param_feedback = feedback;
        param_dry_mix = 1.0;
        param_wet_mix = 0.8;

        // Defaults
        param_ext_fb_processing = false;

        init();

    }

    /**
     * @brief      Advanced constructor for delay effect
     * 
     * ``` CPP
     * 
     * // Set up a delay with max delay of 5 seconds and an fx send/receive loop
     * fx_delay    delay_1(1000.0, // Initial delay length of 1 second / 1000ms
     *                     5000.0, // Max delay of 5 seconds
     *                     0.7,    // Initial feedback value of 0.7
     *                     1.0,    // Clean mix
     *                     0.7,    // Delay / echo mix
     *                     true);  // Enable fx send/receive loop
     *
     * ```
     * 
     * @param[in]  delay_len_ms      The length of the echo in milliseconds (1000.0 milliseconds = 1 second).  For the advanced constructor, the delay_len_max_ms determines the total memory allocated for this delay and will be the max length.  In the basic constructor, the initial length is also the maximum delay length.
     * @param[in]  delay_len_max_ms  The maximum length of the delay (if the delay length is modified)
     * @param[in]  feedback          How much of the output is feedback to the input.  A value of 0.0 will product a single delay.  A value of 1.0 will produce endless echoes.  0.5-0.7 is a nice decaying echo.
     * @param[in]  mix_dry           The mix of the clean signal (0.0 to 1.0)
     * @param[in]  mix_wet           The mix of the delayed/echo signal (0.0 to 1.0)
     * @param[in]  enable_ext_fx     Whether or not to enable the fx send / receive loop (true or false)
     */
    fx_delay(float delay_len_ms, float delay_len_max_ms, float feedback, float mix_dry, float mix_wet, bool enable_ext_fx) : 
        node_delay_tx(NODE_OUT, "delay_fb_tx", this), 
        node_delay_rx(NODE_IN, "delay_fb_rx", this),
        node_ctrl_len_ms(NODE_IN, NODE_FLOAT, "node_ctrl_len_ms", this, FX_DELAY_PARAM_ID_LEN_MS), 
        node_ctrl_feedback(NODE_IN, NODE_FLOAT, "node_ctrl_feedback", this, FX_DELAY_PARAM_ID_FEEDBACK), 
        node_ctrl_dry_mix(NODE_IN, NODE_FLOAT, "node_ctrl_dry_mix", this, FX_DELAY_PARAM_ID_DRY_MIX),
        node_ctrl_wet_mix(NODE_IN, NODE_FLOAT, "node_ctrl_wet_mix", this, FX_DELAY_PARAM_ID_WET_MIX) {
      
      // Set parameters
      param_len_ms = delay_len_ms;
      param_len_max_ms = delay_len_max_ms;
      param_feedback = feedback;
      param_ext_fb_processing = enable_ext_fx;
      param_dry_mix = mix_dry;
      param_wet_mix = mix_wet;

      init();

    };


    /**
     * @brief      Enables the delay effect
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_BOOL, FX_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the delay effect (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_BOOL, FX_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Update the length of the delay.  
     * Note, if you used the simple constructor, the length of the delay needs to be less than or equal to the initial delay value.
     * If you want the ability to set a longer delay than the initial value, use the advanced constructor as this will allow you
     * to also specify the total amount of delay space to allocate which is then the maximum length of a delay.
     */
    void set_length_ms(float len_ms) { 

      CHECK_LAST(len_ms, param_len_ms);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_len_ms.connected) {
        return;
      }

      param_len_ms = len_ms; 
      parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_LEN_MS, (void *) &param_len_ms);
    }

    /**
     * @brief       Updates the feedback parameter of the delay
     *
     * @param[in]  feedback  How much of the output is feedback to the input.  A value of 0.0 will product a single delay.  A value of 1.0 will produce endless echoes.  0.5-0.7 is a nice decaying echo.
     */
    void set_feedback(float feedback) { 

      CHECK_LAST(feedback, param_feedback);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_feedback.connected) {
        return; 
      }

      param_feedback = feedback; 
      parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_FEEDBACK, &param_feedback);
    }

    /**
     * @brief      Sets the dry mix.
     *
     * @param[in]  dry_mix  The mix of the clean signal (0.0 to 1.0)
     */
    void set_dry_mix(float dry_mix) { 
      
      CHECK_LAST(dry_mix, param_dry_mix);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_dry_mix.connected) {
        return; 
      }

      param_dry_mix = dry_mix; 
      parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_DRY_MIX, &param_dry_mix);
    }


    /**
     * @brief      
     */

    /**
     * @brief      Updates the wet / delay mix of the delay (0.0 to 1.0)
     *
     * @param[in]  wet_mix  The mix of the delayed/echo signal (0.0 to 1.0)
     */
    void set_wet_mix(float wet_mix) { 
      
      CHECK_LAST(wet_mix, param_wet_mix);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_wet_mix.connected) {
        return; 
      }

      param_wet_mix = wet_mix; 
      parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_WET_MIX, &param_wet_mix);
    }

  

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /**
     * @brief  Prints the parameters for the delay effect
     */
    void  print_params(void) {
      char buf[64];
      const char * num;
      String val;

      // sprintf(buf," [%#08x] -> %#08x : %d", param_stack[1], * (uint32_t *) param_stack[1], (int) param_stack_types[1]); Serial.println(buf);

      sprintf(buf," Enabled: %s", param_enabled ? "true" : "false");  Serial.println(buf);
      val = String(param_len_ms); num = val.c_str();
      sprintf(buf," Length (ms): %s", num);  Serial.println(buf);
      sprintf(buf," Max length (ms): %.2f", param_len_max_ms);  Serial.println(buf);
      sprintf(buf," Feedback: %.2f", param_feedback);  Serial.println(buf);
      sprintf(buf," Dry mix: %.2f", param_dry_mix);  Serial.println(buf);
      sprintf(buf," Wet mix: %.2f", param_wet_mix);  Serial.println(buf);
      sprintf(buf," External feedback link enabled: %s", param_ext_fb_processing ? "true" : "false");  Serial.println(buf);
      Serial.println(" Routing:");
      
      Serial.print("  + node_ctrl_len_ms: ");
      if (node_ctrl_len_ms.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }

      Serial.print("  + node_ctrl_feedback: ");
      if (node_ctrl_feedback.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }

      Serial.print("  + node_ctrl_dry_mix: ");      
      if (node_ctrl_dry_mix.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }  

      Serial.print("  + node_ctrl_wet_mix: ");      
      if (node_ctrl_wet_mix.connected) {
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

      Serial.print("  * node_delay_rx: ");      
      if (node_delay_rx.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }  
      Serial.print("  * node_delay_tx: ");      
      if (node_delay_tx.connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }  


      Serial.println();
    }
#endif 
};

#endif // DM_FX_DELAY_H