
.. _program_listing_file_src_effects_dm_fx_compressor.h:

Program Listing for File dm_fx_compressor.h
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_compressor.h>` (``src/effects/dm_fx_compressor.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DM_FX_COMPRESSOR_H
   #define DM_FX_COMPRESSOR_H
   
   
   class fx_compressor: public fx_effect {
   
     private:
   
       // Parameters
       float   param_threshold;
       float   param_ratio;
       float   param_attack;
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
   
       fx_audio_node * input;
       
       fx_audio_node * output; 
   
   
       fx_control_node * threshold;
   
   
       fx_control_node * ratio;
   
       fx_control_node * attack;
   
       fx_control_node * release;
   
       fx_control_node * out_gain;
   
   
         
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
   
   
       void enable() {
         CHECK_LAST_ENABLED();
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_BOOL, FX_COMPRESSOR_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_BOOL, FX_COMPRESSOR_PARAM_ID_ENABLED, (void *) &param_enabled);
       }  
   
       void set_threshold(float threshold) { 
         CHECK_LAST(threshold, param_threshold);
   
         if (node_ctrl_threshold.connected) {
           return; 
         }
   
         param_threshold = threshold; 
         parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_THRESH, &param_threshold);
       }  
   
       void set_ratio(float ratio) { 
         CHECK_LAST(ratio, param_ratio);
   
         if (node_ctrl_ratio.connected) {
           return; 
         }
         param_ratio = ratio; 
         parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_RATIO, &param_ratio);
       }  
   
       void set_attack(float attack) { 
         CHECK_LAST(attack, param_attack);
   
         if (node_ctrl_attack.connected) {
           return; 
         }
         param_attack = attack; 
         parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_ATTACK, &param_attack);
       }    
   
       void set_release(float release) { 
         CHECK_LAST(release, param_release);
   
         if (node_ctrl_release.connected) {
           return; 
         }
         param_release = release; 
         parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_RELEASE, &param_release);
       }  
   
       void set_output_gain(float gain_out) { 
         CHECK_LAST(gain_out, param_gain_out);
         
         if (node_ctrl_out_gain.connected) {
           return; 
         }
         param_gain_out = gain_out; 
         parent_canvas->spi_transmit_param(FX_COMPRESSOR, instance_id, T_FLOAT, FX_COMPRESSOR_PARAM_ID_OUT_GAIN, &param_gain_out);
       }  
   
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
   
   };
   
   #endif  // DM_FX_COMPRESSOR_H
