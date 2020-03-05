
.. _program_listing_file_src_effects_dm_fx_gain.h:

Program Listing for File dm_fx_gain.h
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_gain.h>` (``src/effects/dm_fx_gain.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   
   #ifndef DM_FX_GAIN_H
   #define DM_FX_GAIN_H
   
   class fx_gain: public fx_effect {
     
     private:
   
       // Parameters
       uint16_t param_speed;
       float param_gain;
   
       // Control nodes
       fx_control_node node_ctrl_gain;
   
     
       void init(void) {
         // Set class
         type = FX_GAIN;
   
         // Set name
         strcpy(effect_name, "gain");
   
         // Assign programmable node names
         input = &node_input;
         output = &node_output;
   
   
         // Initialize parameter stack
         int indx = 1;
         param_stack[indx] = &param_gain;
         param_stack_types[indx++] = T_FLOAT;
         param_stack[indx] = &param_speed;
         param_stack_types[indx++] = T_INT16;
         total_params = indx;
   
         // Add addiitonal notes to the control stack
         control_node_stack[total_control_nodes++] = &node_ctrl_gain;
   
         gain = &node_ctrl_gain;
       }    
    public:
   
       fx_audio_node * input;
   
       fx_audio_node * output;
   
       fx_control_node * gain;
   
       fx_gain(float gain_val) : 
         node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain_value", this, FX_GAIN_PARAM_ID_GAIN)  {
   
         // Set parameters
         param_gain = gain_val;
         param_speed = (uint16_t) TRANS_MED;
   
         init();
       }
   
   
       fx_gain(float gain_val, EFFECT_TRANSITION_SPEED gain_trans_speed) : 
         node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain_value", this, FX_GAIN_PARAM_ID_GAIN)  {
   
         // Set parameters
         param_gain = gain_val;
         param_speed = (uint16_t) gain_trans_speed;
   
         init();
       }
   
   
   
       void enable() {
         CHECK_LAST_ENABLED();
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_BOOL, FX_GAIN_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_BOOL, FX_GAIN_PARAM_ID_ENABLED, (void *) &param_enabled);
       }    
   
       void set_gain(float new_gain) { 
   
         CHECK_LAST(new_gain, param_gain);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_gain.connected) {
           return; 
         }
   
         param_gain = new_gain; 
         parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_FLOAT, FX_GAIN_PARAM_ID_GAIN, &param_gain);
       }
   
       void set_gain_db(float new_gain_db) { 
   
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_gain.connected) {
           return; 
         }
   
         new_gain_db = powf(10.0, new_gain_db*(1.0/20.0)); 
         CHECK_LAST(new_gain_db, param_gain);
   
         param_gain = new_gain_db; 
         parent_canvas->spi_transmit_param(FX_GAIN, instance_id, T_FLOAT, FX_GAIN_PARAM_ID_GAIN, &param_gain);
       }    
   
       void  print_params(void) {
   
         // void print_parameter( void * val, char * name, PARAM_TYPES type)
         Serial.println("Parameters:");
         print_parameter( &param_enabled, "Enabled", T_BOOL );
         print_parameter( &param_gain, "Gain (linear)", T_FLOAT );
         print_parameter( &param_speed, "Transition rate", T_INT16 );
   
         Serial.println("Control Routing:");      
         print_ctrl_node_status(&node_ctrl_gain);
   
         Serial.println("Audio Routing:");      
         print_audio_node_status(&node_input);
         print_audio_node_status(&node_output);
   
         Serial.println();
       }
   
   };
   #endif  // DM_FX_GAIN_H
