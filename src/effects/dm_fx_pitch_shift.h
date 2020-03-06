// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_PITCH_SHIFT_H
#define DM_FX_PITCH_SHIFT_H


/**
 * @brief      Effect: Pitch shifter - shifts audio up or down in pitch
 * 
 * 
 * These shift the pitch of the audio by a relative amount.  A value of 2.0 will shift the pitch up one octave.  A value of 0.5 will shift the pitch down an octave.  A value of 1.5 will shift the pitch up a fifth, and 0.75 will shift it down a fifth.  And 1.189207 will shift it up a minor third.  
 * 
 * Follow this link for a list of pitch shift values for other common intervals: https://en.wikipedia.org/wiki/Equal_temperament#Comparison_with_Just_Intonation 
 * 
 * Note that pitch shifters are somewhat processor intensive. At the moment, the platform can support "a few" pitch shifters.
 * 
 * Here's an example that uses the center to pot to select between different pitch shift ranges.  The left and right pots set the mix of the clean signal and the pitch shifted signal.
 * 
 * ```CPP
 * #include <dreammakerfx.h>
 * 
 * fx_pitch_shift    pitch_shift(1.0);     
 * 
 * fx_mixer_2    mix2;
 * fx_gain       clean_mix(1.0), pitch_shift_mix(1.0);
 * 
 * void setup() {
 *   // put your setup code here, to run once:
 * 
 *   // Initialize the pedal!
 *   pedal.init();
 * 
 *   // Route audio from instrument -> pitch_shift -> amp
 *   pedal.route_audio(pedal.instr_in, pitch_shift.input);
 *   pedal.route_audio(pitch_shift.output, pitch_shift_mix.input);
 *   pedal.route_audio(pitch_shift_mix.output, mix2.input_1);
 *   
 *   pedal.route_audio(pedal.instr_in, clean_mix.input);
 *   pedal.route_audio(clean_mix.output, mix2.input_2);
 *   
 *   pedal.route_audio(mix2.output, pedal.amp_out);
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
 *   // Left pot changes dry signal gain
 *   if (pedal.pot_left.has_changed()) {
 *     clean_mix.set_gain(pedal.pot_left.val);
 *   }
 *   
 *   // Center pot changes pitch shift amount
 *   if (pedal.pot_center.has_changed()) {
 *     if (pedal.pot_center.val < 0.15) {
 *        // full octave down
 *        pitch_shift.set_freq_shift(0.5);
 *     } else if (pedal.pot_center.val < 0.3) {
 *       // Fifth down
 *       pitch_shift.set_freq_shift(0.75);
 *     } else if (pedal.pot_center.val < 0.5) {
 *       // Third down
 *       pitch_shift.set_freq_shift(0.84);
 *     } else if (pedal.pot_center.val < 0.7) {
 *       // Third up
 *       pitch_shift.set_freq_shift(1.1891);
 *     } else if (pedal.pot_center.val < 0.9) {
 *       // Fifth up
 *       pitch_shift.set_freq_shift(1.5);
 *     } else {
 *       // Octave up
 *       pitch_shift.set_freq_shift(2.0);
 *     }
 *   }  
 *   
 *   // Right pot changes mix gain
 *   if (pedal.pot_center.has_changed()) {
 *     pitch_shift_mix.set_gain(pedal.pot_left.val);   
 *   }    
 *   
 *   // Service pedal
 *   pedal.service();
 * }
 *  * 
 * ```
 */
class fx_pitch_shift: public fx_effect {

  private:

    // Ring modulator parameters
    float param_freq_shift;

    // Control nodes
    fx_control_node node_ctrl_freq_shift;

 public:

    /**
     * Audio routing node: primary audio input
     */
    fx_audio_node * input;

    /**
     * Audio routing node: primary audio output
     */
    fx_audio_node * output;


    // Control node names that users will be using
    fx_control_node * freq_shift;

    /**
     * @brief      Bsaic constructor for the pitch shifter
     *
     * @param[in]  pitch_shift_freq  The amount of relative pitch shift to apply as described above.  A freq_shift of 0.5 will drop down one octave.  A value of 2.0 will go up one octave.  A value of 1.0 will play at current pitch (no shift). 
     */
    fx_pitch_shift(float pitch_shift_freq) : 
        node_ctrl_freq_shift(NODE_IN, NODE_FLOAT, "node_ctrl_freq_shift", this, FX_PITCH_SHIFT_PARAM_ID_FREQ_SHIFT) {
      
      // Set class
      type = FX_PITCH_SHIFT;

      // Set name
      strcpy(effect_name, "pitch shift");

      // Set parameters
      param_freq_shift = pitch_shift_freq;

      // Assign programmable node names
      input = &node_input;
      output = &node_output;

      // Defaults
      param_enabled = true;

      // Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_freq_shift;
      param_stack_types[indx++] = T_FLOAT;
      total_params = indx;

      // Add addiitonal notes to the control stack
      control_node_stack[total_control_nodes++] = &node_ctrl_freq_shift;

      freq_shift = &node_ctrl_freq_shift;

    }

    /**
     * @brief      Enable the pitch shifter (it is enabled by default)
     */
    void enable() {
      CHECK_LAST_ENABLED();
      param_enabled = true; 
      parent_canvas->spi_transmit_param(FX_PITCH_SHIFT, instance_id, T_BOOL, FX_PITCH_SHIFT_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Bypass the pitch shifter (will just pass clean audio through)
     */
    void bypass() {
      CHECK_LAST_DISABLED();
      param_enabled = false; 
      parent_canvas->spi_transmit_param(FX_PITCH_SHIFT, instance_id, T_BOOL, FX_PITCH_SHIFT_PARAM_ID_ENABLED, (void *) &param_enabled);
    }

    /**
     * @brief      Update the pitch shifter value.  A freq_shift of 0.5 will drop down one octave.  A value of 
     *             2.0 will go up one octave.  A value of 1.0 will play at current pitch (no shift). 
     *
     * @param[in]  freq_shift  The frequency shift
     */
    void set_freq_shift(float freq_shift) { 

      CHECK_LAST(freq_shift, param_freq_shift);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_freq_shift.connected) {
        return;
      }

      param_freq_shift = freq_shift;
      parent_canvas->spi_transmit_param(FX_PITCH_SHIFT, instance_id, T_FLOAT, FX_PITCH_SHIFT_PARAM_ID_FREQ_SHIFT, (void *) &param_freq_shift);
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /**
     * @brief      Print the parameters for this effect
     */
    void  print_params(void) {
      char buf[64];
      const char * num;
      String val;

      sprintf(buf," Enabled: %s", param_enabled ? "true" : "false");  Serial.println(buf);
      sprintf(buf," Freq shift ratio: %.2f", param_freq_shift);  Serial.println(buf);

      Serial.println(" Routing:");
      
      Serial.print("  + node_ctrl_freq_shift: ");
      if (node_ctrl_freq_shift.connected) {
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
#endif     
};

#endif 	// DM_FX_PITCH_SHIFT_H
