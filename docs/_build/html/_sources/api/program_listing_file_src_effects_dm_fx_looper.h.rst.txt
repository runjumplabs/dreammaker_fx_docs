
.. _program_listing_file_src_effects_dm_fx_looper.h:

Program Listing for File dm_fx_looper.h
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_looper.h>` (``src/effects/dm_fx_looper.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   
   #ifndef DM_FX_LOOPER_H
   #define DM_FX_LOOPER_H
   
   class fx_looper: public fx_effect {
     
     private:
   
       // Parameters
       float param_playback_rate;
       float param_dry_mix;
       float param_loop_mix;
       float param_max_length_seconds;
       bool  param_start;
       bool  param_stop;
       bool  param_ext_pre_processing_en;
   
       // Additional audio nodes
       fx_audio_node node_loop_pp_send;
       fx_audio_node node_loop_pp_receive;
   
   
       // Control nodes
       fx_control_node node_ctrl_playback_rate;
       fx_control_node node_ctrl_dry_mix;
       fx_control_node node_ctrl_loop_mix;
       fx_control_node node_ctrl_start;
       fx_control_node node_ctrl_stop;
       fx_control_node node_ctrl_loop_length;
       fx_control_node node_ctrl_loop_length_set;
   
     public:
   
   
       fx_audio_node * input;
   
       fx_audio_node * output;
   
       fx_audio_node * preproc_send;
   
       fx_audio_node * preproc_receive;
   
   
   
       fx_control_node * start;
   
       fx_control_node * stop;
   
       fx_control_node * playback_rate;
   
       fx_control_node * dry_mix;
   
       fx_control_node * loop_mix;
   
       fx_control_node * loop_length_seconds;
   
       fx_control_node * loop_length_seconds_set;
   
       fx_looper(float looper_dry_mix, float looper_loop_mix, float looper_max_length_seconds, bool looper_enable_loop_preprocessing ) : 
         node_loop_pp_send(NODE_OUT, "looper_pp_send", this), 
         node_loop_pp_receive(NODE_IN, "looper_pp_receive", this),
   
         node_ctrl_playback_rate(NODE_IN, NODE_FLOAT, "node_ctrl_playback_rate", this, FX_LOOPER_PARAM_ID_RATE),
         node_ctrl_dry_mix(NODE_IN, NODE_FLOAT, "node_ctrl_dry_mix", this, FX_LOOPER_PARAM_ID_DRY_MIX),
         node_ctrl_loop_mix(NODE_IN, NODE_FLOAT, "node_ctrl_loop_mix", this, FX_LOOPER_PARAM_ID_LOOP_MIX),
         node_ctrl_start(NODE_IN, NODE_BOOL, "node_ctrl_start", this, FX_LOOPER_PARAM_ID_START),
         node_ctrl_stop(NODE_IN, NODE_BOOL, "node_ctrl_stop", this, FX_LOOPER_PARAM_ID_STOP),
         node_ctrl_loop_length(NODE_OUT, NODE_FLOAT, "node_loop_length_s_read", this, FX_LOOPER_PARAM_ID_LOOP_LEN_S),
         node_ctrl_loop_length_set(NODE_IN, NODE_FLOAT, "node_loop_length_s_set", this, FX_LOOPER_PARAM_ID_LOOP_LEN_S_SET) {
   
         // Set class
         type = FX_LOOPER;
   
         // Set name
         strcpy(effect_name, "looper");
   
         // Set parameters
         param_dry_mix = looper_dry_mix;
         param_loop_mix = looper_loop_mix;
         param_max_length_seconds = looper_max_length_seconds;
         param_ext_pre_processing_en = looper_enable_loop_preprocessing;
   
         // initialize other variables
         param_playback_rate = 1.0;
         param_start = false;
         param_stop = false;
         param_enabled = true;
   
         // Assign programmable node names
         input = &node_input;
         output = &node_output;
         preproc_send = &node_loop_pp_send;
         preproc_receive = &node_loop_pp_receive;
   
   
               // Initialize parameter stack
         int indx = 1;
         param_stack[indx] = &param_max_length_seconds;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_dry_mix;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_loop_mix;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_playback_rate;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_ext_pre_processing_en;
         param_stack_types[indx++] = T_BOOL;
   
         param_stack[indx] = &param_start;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_stop;
         param_stack_types[indx++] = T_BOOL;
         total_params = indx;
   
         // Add additional nodes to the audio stack
         audio_node_stack[total_audio_nodes++] = &node_loop_pp_receive;
         audio_node_stack[total_audio_nodes++] = &node_loop_pp_send;    
   
         // Add addiitonal notes to the control stack
         control_node_stack[total_control_nodes++] = &node_ctrl_playback_rate;
         control_node_stack[total_control_nodes++] = &node_ctrl_dry_mix;
         control_node_stack[total_control_nodes++] = &node_ctrl_loop_mix;
         control_node_stack[total_control_nodes++] = &node_ctrl_start;
         control_node_stack[total_control_nodes++] = &node_ctrl_stop;
         control_node_stack[total_control_nodes++] = &node_ctrl_loop_length;
         control_node_stack[total_control_nodes++] = &node_ctrl_loop_length_set;
   
         start = &node_ctrl_start;
         stop = &node_ctrl_stop;
         playback_rate = &node_ctrl_playback_rate;
         dry_mix = &node_ctrl_dry_mix;
         loop_mix = &node_ctrl_loop_mix;
         loop_length_seconds = &node_ctrl_loop_length;
         loop_length_seconds_set = &node_ctrl_loop_length_set;
   
       }
   
       void enable() {
         CHECK_LAST_ENABLED();
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_BOOL, FX_LOOPER_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_BOOL, FX_LOOPER_PARAM_ID_ENABLED, (void *) &param_enabled);
       }    
   
       void start_loop_recording() {
           param_start = true;
           param_stop = false;
           parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_BOOL, FX_LOOPER_PARAM_ID_START, (void *) &param_start);
       }
   
       void stop_loop_recording() {
           param_start = false;
           param_stop= true;
           parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_BOOL, FX_LOOPER_PARAM_ID_STOP, (void *) &param_stop);
       }
   
       void stop_loop_playback() {
         param_start = false;
         param_stop= true;
         parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_BOOL, FX_LOOPER_PARAM_ID_STOP, (void *) &param_stop);
       }
   
       void set_playback_rate(float playback_rate) { 
   
         CHECK_LAST(playback_rate, param_playback_rate);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_playback_rate.connected) {
           return; 
         }
   
         param_playback_rate = playback_rate; 
         parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_FLOAT, FX_LOOPER_PARAM_ID_RATE, &param_playback_rate);
       }
   
       void set_loop_mix(float new_loop_mix) { 
   
         CHECK_LAST(new_loop_mix, param_loop_mix);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_loop_mix.connected) {
           return; 
         }
   
         param_loop_mix = new_loop_mix; 
         parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_FLOAT, FX_LOOPER_PARAM_ID_LOOP_MIX, &param_loop_mix);
       }
   
       void set_dry_mix(float new_dry_mix) { 
   
         CHECK_LAST(new_dry_mix, param_dry_mix);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_dry_mix.connected) {
           return; 
         }
   
         param_dry_mix = new_dry_mix; 
         parent_canvas->spi_transmit_param(FX_LOOPER, instance_id, T_FLOAT, FX_LOOPER_PARAM_ID_DRY_MIX, &param_dry_mix);
       }    
   
       void  print_params(void) {
   
               // void print_parameter( void * val, char * name, PARAM_TYPES type)
               Serial.println("Parameters:");
         print_parameter( &param_enabled, "Enabled", T_BOOL );
         print_parameter( &param_playback_rate, "Playback rate", T_FLOAT );
         print_parameter( &param_dry_mix, "Dry mix", T_FLOAT );
         print_parameter( &param_loop_mix, "Loop mix", T_FLOAT );
         print_parameter( &param_ext_pre_processing_en, "Pre-process insert before loop", T_BOOL );
   
         Serial.println("Control Routing:");      
         print_ctrl_node_status(&node_ctrl_playback_rate);
         print_ctrl_node_status(&node_ctrl_dry_mix);
         print_ctrl_node_status(&node_ctrl_loop_mix);
         print_ctrl_node_status(&node_ctrl_start);
         print_ctrl_node_status(&node_ctrl_stop);
         print_ctrl_node_status(&node_ctrl_loop_length);
         print_ctrl_node_status(&node_ctrl_loop_length_set);
   
         Serial.println("Audio Routing:");      
         print_audio_node_status(&node_input);
         print_audio_node_status(&node_output);
         print_audio_node_status(&node_loop_pp_receive);
         print_audio_node_status(&node_loop_pp_send);
   
         Serial.println();
       }
   
   };
   #endif // DM_FX_LOOPER_H
