// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_ARPEGGIATOR_H
#define DM_FX_ARPEGGIATOR_H

/**
 * @brief      Effect: Arpeggiator which can sequence rhythmic patterns of pitch, gain and parameters
 *
 * An arpeggiator is a control source that can play sequences of notes and other control signals.
 * 
 * The arpeggiator relies on a structure that you define at the top of your Arduino sketch that contains
 * the sequence that the arpeggiator will run through.  This sequence can have up to 16 steps.  In
 * this example, our sequence is called `steps2` but you can name it whatever you'd like.
 * 
 * You can also add `.param_1` and `.param_2` to this struct for any additional control values.  These control
 * values can be wired into any type of control note (filters, distortions, etc.)
 * 
 * ``` CPP
 * ARP_STEP steps2[] = {
 * { .freq = SEMI_TONE_2, .vol = 0.3, .dur = 125.0 },
 * { .freq = SEMI_TONE_0, .vol = 0.0, .dur = 375.0 },
 * { .freq = SEMI_TONE_5, .vol = 0.3, .dur = 375.0 },
 * { .freq = SEMI_TONE_7, .vol = 0.9, .dur = 125.0 }, 
 * };
 *
 * ```
 */

class fx_arpeggiator: public fx_effect {

  private:
    // Parameters
    float    param_time_scale;
    float    param_period_ms;
    uint16_t param_total_steps;
    ARP_STEP param_arp_steps[ARP_MAX_STEPS];

    // Control nodes
    fx_control_node node_ctrl_time_scale;
    fx_control_node node_ctrl_period_ms;
    fx_control_node node_ctrl_freq;
    fx_control_node node_ctrl_vol;
    fx_control_node node_ctrl_param_1;
    fx_control_node node_ctrl_param_2;
    fx_control_node node_ctrl_start;

    void init(void) {

      // Set class
      type = FX_ARPEGGIATOR;

      // Set name
      strcpy(effect_name, "arpeggiator");

        // Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_total_steps;
      param_stack_types[indx++] = T_INT16;

      for (int i=0;i<param_total_steps;i++) {
        param_stack[indx] = &param_arp_steps[i].freq;
        param_stack_types[indx++] = T_FLOAT;
        param_stack[indx] = &param_arp_steps[i].vol;
        param_stack_types[indx++] = T_FLOAT;
        param_stack[indx] = &param_arp_steps[i].dur;
        param_stack_types[indx++] = T_FLOAT;
        param_stack[indx] = &param_arp_steps[i].param_1;
        param_stack_types[indx++] = T_FLOAT;
        param_stack[indx] = &param_arp_steps[i].param_2;
        param_stack_types[indx++] = T_FLOAT;
      }

      total_params = indx;            

      // Assign controls
      time_scale = &node_ctrl_time_scale;
      period_ms = &node_ctrl_period_ms;
      freq = &node_ctrl_freq;
      vol = &node_ctrl_vol;
      param_1 = &node_ctrl_param_1;
      param_2 = &node_ctrl_param_2;
      start = &node_ctrl_start;

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_time_scale;
      control_node_stack[total_control_nodes++] = &node_ctrl_period_ms;
      control_node_stack[total_control_nodes++] = &node_ctrl_freq;
      control_node_stack[total_control_nodes++] = &node_ctrl_vol;
      control_node_stack[total_control_nodes++] = &node_ctrl_param_1;
      control_node_stack[total_control_nodes++] = &node_ctrl_param_2;
      control_node_stack[total_control_nodes++] = &node_ctrl_start;

    }

    void serialize_arps(void) {

    }

  public:

    /**
     * Control routing node: Time scale of arpeggiator (aka playback rate).  A value of 1.0 runs arpeggiator at default speed.  Lower is slower, higher is faster.
     */
    fx_control_node * time_scale;

    /**
     * Control routing node: Target duration of the arpeggiator.  Arpeggiator will be scaled so the whole sequence fits within this time.
     */
    fx_control_node * period_ms;

    /**
     * Control routing node: Frequency value for each stage of the arpeggiator
     */
    fx_control_node * freq;

    /**
     * Control routing node: Volume value for each stage of the arpeggiator
     */
    fx_control_node * vol;

    /**
     * Control routing node: Auxiliary parameter #1 for each stage of the arpeggiator
     */
    fx_control_node * param_1;    

    /**
     * Control routing node: Auxiliary parameter #2 for each stage of the arpeggiator
     */
    fx_control_node * param_2;    

    /**
     * Control routing node: Restarts the arpeggiator - wire to new note event to start arp sequence with each note
     */
    fx_control_node * start;    


    /**
     * @brief      Simple constructor for arpeggiator
     *
     * See above for a description of how to define an arpeggiator sequence.
     * 
     * ``` CPP 
     * // Define arp sequence with 4 steps with a total duration of 1 second
     * ARP_STEP steps2[] = {
     *   { .freq = SEMI_TONE_2, .vol = 0.3, .dur = 125.0 },
     *   { .freq = SEMI_TONE_0, .vol = 0.0, .dur = 375.0 },
     *   { .freq = SEMI_TONE_5, .vol = 0.3, .dur = 375.0 },
     *   { .freq = SEMI_TONE_7, .vol = 0.9, .dur = 125.0 }, 
     * };
     * 
     * // Define our arpeggiator
     * fx_arpeggiator  arp2(4,            // Total number of steps
     *                      &steps2[0]);  // Reference to our sequence
     * 
     * ```
     *
     * @param[in]  total_steps  The total arpeggiator steps
     * @param      steps        A pointer to an array of ARP_STEP containing the steps
     */
    fx_arpeggiator( int total_steps, ARP_STEP * steps) : 
      node_ctrl_time_scale(NODE_IN, NODE_FLOAT, "node_ctrl_time_scale", this, FX_ARPEGGIATOR_PARAM_ID_TIME_SCALE),
      node_ctrl_period_ms(NODE_IN, NODE_FLOAT, "node_ctrl_period_ms", this, FX_ARPEGGIATOR_PARAM_ID_PERIOD),
      node_ctrl_freq(NODE_OUT, NODE_FLOAT, "node_ctrl_freq", this, FX_ARPEGGIATOR_PARAM_ID_FREQ),
      node_ctrl_vol(NODE_OUT, NODE_FLOAT, "node_ctrl_vol", this, FX_ARPEGGIATOR_PARAM_ID_VOL),
      node_ctrl_param_1(NODE_OUT, NODE_FLOAT, "node_ctrl_param_1", this, FX_ARPEGGIATOR_PARAM_ID_PARAM_1),
      node_ctrl_param_2(NODE_OUT, NODE_FLOAT, "node_ctrl_param_2", this, FX_ARPEGGIATOR_PARAM_ID_PARAM_2),
      node_ctrl_start(NODE_IN, NODE_FLOAT, "node_ctrl_start", this, FX_ARPEGGIATOR_PARAM_ID_RESTART) {
        
        if (total_steps > ARP_MAX_STEPS) {
          DEBUG_MSG("Maximum number of arpeggiator steps is 16", MSG_ERROR);
          display_error_status(ERROR_CODE_ILLEGAL_ROUTING);
        }

        param_total_steps = total_steps;
        param_time_scale = 1.0;

        for (int i=0;i<total_steps;i++) {
          param_arp_steps[i].freq = steps[i].freq;
          param_arp_steps[i].vol = steps[i].vol;
          param_arp_steps[i].dur = steps[i].dur;
          param_arp_steps[i].param_1 = steps[i].param_1;
          param_arp_steps[i].param_2 = steps[i].param_2;
        }

        init();
    }


    /**
     * @brief      Sets the time scale ratio of the arpeggiator
     *
     * @param[in]  new_time_scale  The new time scale ratio (1.0 is current time scale, > 1.0 is faster, < 1.0 is slower)
     */
    void set_time_scale(float new_time_scale) { 

      CHECK_LAST(new_time_scale, param_time_scale);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_time_scale.connected) {
        return;
      }


      param_time_scale = new_time_scale;
      parent_canvas->spi_transmit_param(FX_ARPEGGIATOR, instance_id, T_FLOAT, FX_ARPEGGIATOR_PARAM_ID_TIME_SCALE, (void *) &param_time_scale);
    }

    /**
     * @brief      Sets the duration of the arpeggiator in milliseconds
     *
     * @param[in]  new_duration  The new duration in milliseconds
     */
    void set_duration_ms(float new_duration) { 

      CHECK_LAST(new_duration, param_period_ms);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_period_ms.connected) {
        return;
      }

      param_period_ms = new_duration;
      parent_canvas->spi_transmit_param(FX_ARPEGGIATOR, instance_id, T_FLOAT, FX_ARPEGGIATOR_PARAM_ID_PERIOD, (void *) &param_period_ms);
    }


    void  print_params(void) {

        // void print_parameter( void * val, char * name, PARAM_TYPES type)
        Serial.println("Parameters:");
        print_parameter( &param_total_steps, "Total steps", T_INT16 );
        Serial.println("relative frequency, volume, duration (ms), param1, param2");
        for (int i=0;i<param_total_steps;i++) {
          Serial.print("  ");
          Serial.print(i);
          Serial.print(": ");
          Serial.print(param_arp_steps[i].freq);
          Serial.print(", ");
          Serial.print(param_arp_steps[i].vol);
          Serial.print(", ");
          Serial.print(param_arp_steps[i].dur);
          Serial.print(", ");
          Serial.print(param_arp_steps[i].param_1);
          Serial.print(", ");
          Serial.println(param_arp_steps[i].param_2);          
        }
        Serial.println("Control Routing:");      
        print_ctrl_node_status(&node_ctrl_time_scale);
        print_ctrl_node_status(&node_ctrl_period_ms);
        print_ctrl_node_status(&node_ctrl_freq);
        print_ctrl_node_status(&node_ctrl_vol);
        print_ctrl_node_status(&node_ctrl_param_1);
        print_ctrl_node_status(&node_ctrl_param_2);


        Serial.println(instance_id);
        Serial.println(total_params);

        Serial.println();
      }

};

#endif // DM_FX_ARPEGGIATOR_H