
.. _program_listing_file_src_effects_dm_fx_slicer.h:

Program Listing for File dm_fx_slicer.h
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_slicer.h>` (``src/effects/dm_fx_slicer.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   
   #ifndef DM_FX_SLICER_H
   #define DM_FX_SLICER_H
   
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
   
     fx_audio_node * input;
   
     fx_audio_node * output_1;
   
     fx_audio_node * output_2;
   
     fx_audio_node * output_3;
     
     fx_audio_node * output_4;
     
     fx_audio_node * output_5;
     
     fx_audio_node * output_6;
     
     fx_audio_node * output_7;
     
     fx_audio_node * output_8;
   
   
     fx_control_node * period;
   
   
     fx_control_node * start;
   
     
   
   
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
   
     
     void enable() {
       CHECK_LAST_ENABLED();
       param_enabled = true; 
       parent_canvas->spi_transmit_param(FX_SLICER, instance_id, T_BOOL, FX_SLICER_PARAM_ID_ENABLED, (void *) &param_enabled);
     }
   
     void bypass() {
       CHECK_LAST_DISABLED();
       param_enabled = false; 
       parent_canvas->spi_transmit_param(FX_SLICER, instance_id, T_BOOL, FX_SLICER_PARAM_ID_ENABLED, (void *) &param_enabled);
     }  
   
   
     void set_period_ms(float period) { 
   
       CHECK_LAST(period, param_period);
             
       if (node_ctrl_period.connected) {
         return; 
       }
   
       param_period = period; 
       parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_FLOAT, FX_SLICER_PARAM_ID_PERIOD, &param_period);
     }
   
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
   };
   
   
   #endif  // DM_FX_SLICER_H
