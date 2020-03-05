
.. _program_listing_file_src_effects_dm_fx_destructor.h:

Program Listing for File dm_fx_destructor.h
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_destructor.h>` (``src/effects/dm_fx_destructor.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DM_FX_DESTRUCTOR_H
   #define DM_FX_DESTRUCTOR_H
   
   class fx_destructor: public fx_effect {
   
     private:
   
       // Parameters
       DESTRUCTOR_TYPE  param_type;
       float                    param_param_1;
       float                        param_param_2;
       float            param_output_gain;
       bool                         param_upsample;
   
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
   
     fx_audio_node * input;
     
     fx_audio_node * output;
   
     fx_control_node * param_1;
   
     fx_control_node * param_2;
   
     fx_control_node * output_gain;
   
   
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
   
     void enable() {
       CHECK_LAST_ENABLED();
       param_enabled = true; 
       parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_BOOL, FX_DESTRUCTOR_PARAM_ID_ENABLED, (void *) &param_enabled);
     }
   
     void bypass() {
       CHECK_LAST_DISABLED();
       param_enabled = false; 
       parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_BOOL, FX_DESTRUCTOR_PARAM_ID_ENABLED, (void *) &param_enabled);
     }  
   
     void set_param_1(float new_param_1) { 
   
       CHECK_LAST(new_param_1, param_param_1);
       
       if (node_ctrl_param_1.connected) {
         return; 
       }
   
       param_param_1 = new_param_1; 
       parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_FLOAT, FX_DESTRUCTOR_PARAM_ID_PARAM_1, &param_param_1);
     }
   
   
   
   
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
   
     void set_param_2(float new_param_2) { 
   
       CHECK_LAST(new_param_2, param_param_2);
   
       if (node_ctrl_param_2.connected) {
         return; 
       }
   
       param_param_2 = new_param_2; 
       parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_FLOAT, FX_DESTRUCTOR_PARAM_ID_PARAM_2, &param_param_2);
     }  
   
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
   
   
     void set_output_gain(float new_gain) { 
   
       CHECK_LAST(new_gain, param_output_gain);
   
   
       if (node_ctrl_output_gain.connected) {
         return; 
       }
   
       param_output_gain = new_gain; 
       parent_canvas->spi_transmit_param(FX_DESTRUCTOR, instance_id, T_FLOAT, FX_DESTRUCTOR_PARAM_ID_OUT_GAIN, &param_output_gain);
     }    
   
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
   
