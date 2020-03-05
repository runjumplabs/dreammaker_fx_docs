
.. _program_listing_file_src_effects_dm_fx_pitch_shift.h:

Program Listing for File dm_fx_pitch_shift.h
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_pitch_shift.h>` (``src/effects/dm_fx_pitch_shift.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   
   #ifndef DM_FX_PITCH_SHIFT_H
   #define DM_FX_PITCH_SHIFT_H
   
   
   class fx_pitch_shift: public fx_effect {
   
     private:
   
       // Ring modulator parameters
       float param_freq_shift;
   
       // Control nodes
       fx_control_node node_ctrl_freq_shift;
   
    public:
   
       fx_audio_node * input;
   
       fx_audio_node * output;
   
   
       // Control node names that users will be using
       fx_control_node * freq_shift;
   
       // Constructor
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
   
       void enable() {
         CHECK_LAST_ENABLED();
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_PITCH_SHIFT, instance_id, T_BOOL, FX_PITCH_SHIFT_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_PITCH_SHIFT, instance_id, T_BOOL, FX_PITCH_SHIFT_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void set_freq_shift(float freq_shift) { 
   
         CHECK_LAST(freq_shift, param_freq_shift);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_freq_shift.connected) {
           return;
         }
   
         param_freq_shift = freq_shift;
         parent_canvas->spi_transmit_param(FX_PITCH_SHIFT, instance_id, T_FLOAT, FX_PITCH_SHIFT_PARAM_ID_FREQ_SHIFT, (void *) &param_freq_shift);
       }
   
   
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
   };
   
   #endif  // DM_FX_PITCH_SHIFT_H
