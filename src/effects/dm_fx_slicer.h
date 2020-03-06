// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_SLICER_H
#define DM_FX_SLICER_H

/**
 * @brief      Effect: Slicer - chops up audio in the time domain and pipes to different effects
 * 
 * Think of a slicer like a robot at a mixer that is send the incoming audio through multiple effects in a time sequenced manner.  For example, if 2 channels are used and the period is set to 1000 ms (or 1 second), for the first half second, the audio is routed to output_1 and for the second half second, the audio is routed to output_2.  These two outputs can run through different effects  so the result is a cool, rhythmic effect.
 * 
 * Here's an example where the slicer is sending audio through a distortion and through four different filters to create a "nee-naa-noo-nuu nee-naa-noo-nuu nee-naa-noo-nuu" sound?
 * 
 * ```CPP 
 * #include <dreammakerfx.h>
 * 
 * fx_slicer       slice4(750.0, 4);     
 * 
 * fx_mixer_4      mix4;
 * fx_destructor   tube_clip(0.1,           // Clipping level (from 0 to 1.0) - lower is heavier distortion
 *                           16.0,           // Input drive 
 *                           SMOOTH_CLIP);  // Distortion function = fuzz
 * 
 * fx_biquad_filter  filt1(200, 3.0, BIQUAD_TYPE_LPF);
 * fx_biquad_filter  filt2(800, 3.0, BIQUAD_TYPE_LPF);
 * fx_biquad_filter  filt3(1100, 3.0, BIQUAD_TYPE_LPF);
 * fx_biquad_filter  filt4(1800, 3.0, BIQUAD_TYPE_LPF);
 * 
 * 
 * void setup() {
 *   // put your setup code here, to run once:
 * 
 *   // Initialize the pedal!
 *   pedal.init();
 *    
 *   pedal.route_audio(pedal.instr_in, tube_clip.input);
 *   pedal.route_audio(tube_clip.output, slice4.input);
 *  
 *   pedal.route_audio(slice4.output_1, filt1.input);
 *   pedal.route_audio(slice4.output_2, filt2.input);
 *   pedal.route_audio(slice4.output_3, filt3.input);
 *   pedal.route_audio(slice4.output_4, filt4.input);
 * 
 *   pedal.route_audio(filt1.output, mix4.input_1);
 *   pedal.route_audio(filt2.output, mix4.input_2);
 *   pedal.route_audio(filt3.output, mix4.input_3);
 *   pedal.route_audio(filt4.output, mix4.input_4);
 *   
 *   pedal.route_audio(mix4.output, pedal.amp_out);
 * 
 *   // Reset the slicer position when a new note is played
 *   pedal.route_control(pedal.new_note, slice4.start);
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
 *   // Left pot changes filter resonance
 *   if (pedal.pot_left.has_changed()) {
 *     filt1.set_resonance(1.0 + pedal.pot_left.val * 5.0);
 *     filt2.set_resonance(1.0 + pedal.pot_left.val * 5.0);
 *     filt3.set_resonance(1.0 + pedal.pot_left.val * 5.0);
 *     filt4.set_resonance(1.0 + pedal.pot_left.val * 5.0);
 *   }
 *   
 *   // Center pot changes drive 
 *   if (pedal.pot_center.has_changed()) {
 *     tube_clip.set_input_drive(2.0 + pedal.pot_center.val * 32);
 *   }  
 *   
 *   // Right pot changes slicer period
 *   if (pedal.pot_right.has_changed()) {
 *     slice4.set_period_ms(500.0 + 1000 * (1.0 - pedal.pot_right.val));
 *   }    
 *   
 *   // Service pedal
 *   pedal.service();
 * }
 *  
 * ```
 */


class fx_slicer: public fx_effect {

private:

    // Parameters
    uint32_t param_channels;
    float param_period;

    // Additional audio nodes
    fx_audio_node node_output2;
    fx_audio_node node_output3;
    fx_audio_node node_output4;
    fx_audio_node node_output5;
    fx_audio_node node_output6;
    fx_audio_node node_output7;
    fx_audio_node node_output8;
    fx_audio_node node_dummy_input;
    
    // Control nodes
    fx_control_node node_ctrl_period;
    fx_control_node node_ctrl_start;

    void init() {
      // Set class
      type = FX_SLICER;

      // Set name
      strcpy(effect_name, "slicer");      

      // Assign programmable node names
      input = &node_input;
      output_1 = &node_output;
      output_2 = &node_output2;
      output_3 = &node_output3;
      output_4 = &node_output4;
      output_5 = &node_output5;
      output_6 = &node_output6;
      output_7 = &node_output7;
      output_8 = &node_output8;

      audio_node_stack[total_audio_nodes++] = &node_dummy_input;   // dummy output node since inputs and outputs go in pairs
      audio_node_stack[total_audio_nodes++] = &node_output2;
      audio_node_stack[total_audio_nodes++] = &node_dummy_input;   // dummy output node since inputs and outputs go in pairs
      audio_node_stack[total_audio_nodes++] = &node_output3;
      audio_node_stack[total_audio_nodes++] = &node_dummy_input;   // dummy output node since inputs and outputs go in pairs
      audio_node_stack[total_audio_nodes++] = &node_output4;
      audio_node_stack[total_audio_nodes++] = &node_dummy_input;   // dummy output node since inputs and outputs go in pairs
      audio_node_stack[total_audio_nodes++] = &node_output5;
      audio_node_stack[total_audio_nodes++] = &node_dummy_input;   // dummy output node since inputs and outputs go in pairs
      audio_node_stack[total_audio_nodes++] = &node_output6;
      audio_node_stack[total_audio_nodes++] = &node_dummy_input;   // dummy output node since inputs and outputs go in pairs
      audio_node_stack[total_audio_nodes++] = &node_output7;

      // Initialize parameter stack
      int indx = 1;

      param_stack[indx] = &param_period;
      param_stack_types[indx++] = T_INT32;

      param_stack[indx] = &param_channels;
      param_stack_types[indx++] = T_INT32;
      total_params = indx;         

      control_node_stack[total_control_nodes++] = &node_ctrl_period;
      control_node_stack[total_control_nodes++] = &node_ctrl_start;

      // Assign controls
      period = &node_ctrl_period;
      start = &node_ctrl_start;
      
    } 


 public:

  /**
   * Audio routing node: primary audio input
   */
  fx_audio_node * input;

  /**
   * Audio routing node: audio output for slicer channel 0
   */
  fx_audio_node * output_1;

  /**
   * Audio routing node: audio output for slicer channel 1
   */  
  fx_audio_node * output_2;

  /**
   * Audio routing node: audio output for slicer channel 2
   */  
  fx_audio_node * output_3;
  
  /**
   * Audio routing node: audio output for slicer channel 3
   */  
  fx_audio_node * output_4;
  
  /**
   * Audio routing node: audio output for slicer channel 4
   */  
  fx_audio_node * output_5;
  
  /**
   * Audio routing node: audio output for slicer channel 5
   */  
  fx_audio_node * output_6;
  
  /**
   * Audio routing node: audio output for slicer channel 6
   */  
  fx_audio_node * output_7;
  
  /**
   * Audio routing node: audio output for slicer channel 7
   */  
  fx_audio_node * output_8;


  /**
   * Control routing node: period in in milliseconds
   */
  fx_control_node * period;


  /**
   * Control routing node: restarts the sequence at position 0 for triggering with a new note
   */
  fx_control_node * start;

  


  /**
   * @brief      Basic constructor/initializer for the slicer
   *
   * @param[in]  period_ms  The period in milliseconds
   * @param[in]  channels   The number of channels to slice between during the period
   */
  fx_slicer(float period_ms, int32_t channels) : 
    node_output2(NODE_OUT, "output_2", this), 
    node_output3(NODE_OUT, "output_3", this), 
    node_output4(NODE_OUT, "output_4", this), 
    node_output5(NODE_OUT, "output_5", this), 
    node_output6(NODE_OUT, "output_6", this), 
    node_output7(NODE_OUT, "output_7", this), 
    node_output8(NODE_OUT, "output_8", this), 
    node_dummy_input(NODE_IN, "dummy", this),
    node_ctrl_start(NODE_IN, NODE_FLOAT, "node_ctrl_start", this, FX_SLICER_PARAM_ID_START),
    node_ctrl_period(NODE_IN, NODE_FLOAT, "node_ctrl_period", this, FX_SLICER_PARAM_ID_PERIOD)
    {
      
      // Set parameters
      param_channels = channels;
      param_period = period_ms;      

      init();
  }

  
  /**
   * @brief      Enable the slicer (it is enabled by default)
   */
  void enable() {
    CHECK_LAST_ENABLED();
    param_enabled = true; 
    parent_canvas->spi_transmit_param(FX_SLICER, instance_id, T_BOOL, FX_SLICER_PARAM_ID_ENABLED, (void *) &param_enabled);
  }

  /**
   * @brief      Bypass the slicer  (will just pass clean audio through)
   */
  void bypass() {
    CHECK_LAST_DISABLED();
    param_enabled = false; 
    parent_canvas->spi_transmit_param(FX_SLICER, instance_id, T_BOOL, FX_SLICER_PARAM_ID_ENABLED, (void *) &param_enabled);
  }  


  /**
   * @brief      Upates the period in milliseconds for the slicer
   *
   * @param[in]  period  The period in milliseconds (thousands of a second)
   */
  void set_period_ms(float period) { 

    CHECK_LAST(period, param_period);
          
    if (node_ctrl_period.connected) {
      return; 
    }

    param_period = period; 
    parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_FLOAT, FX_SLICER_PARAM_ID_PERIOD, &param_period);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /**
   * @brief      Print the parameters for this effect
   */
  void  print_params(void) {

  // void print_parameter( void * val, char * name, PARAM_TYPES type)
    Serial.println("Parameters:");
    print_parameter( &param_enabled, "Enabled", T_BOOL );
    print_parameter( &param_period, "Period", T_FLOAT );
    print_parameter( &param_channels, "# channels", T_INT32 );

    Serial.println("Control Routing:");      
    print_ctrl_node_status(&node_ctrl_start);
    print_ctrl_node_status(&node_ctrl_period);

    Serial.println("Audio Routing:");      
    print_audio_node_status(&node_input);
    print_audio_node_status(&node_output);
    print_audio_node_status(&node_output2);
    print_audio_node_status(&node_output3);
    print_audio_node_status(&node_output4);
    print_audio_node_status(&node_output5);
    print_audio_node_status(&node_output6);
    print_audio_node_status(&node_output7);
    print_audio_node_status(&node_output8);

    Serial.println();
  }
#endif 

};


#endif  // DM_FX_SLICER_H