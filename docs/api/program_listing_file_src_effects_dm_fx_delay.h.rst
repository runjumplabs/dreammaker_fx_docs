
.. _program_listing_file_src_effects_dm_fx_delay.h:

Program Listing for File dm_fx_delay.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_delay.h>` (``src/effects/dm_fx_delay.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DM_FX_DELAY_H
   #define DM_FX_DELAY_H
   
   
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
   
   
       fx_audio_node * input;
   
       fx_audio_node * output;
   
       fx_audio_node * fx_send;
   
       fx_audio_node * fx_receive;
   
   
       fx_control_node * length_ms;
   
       fx_control_node * feedback;
   
       fx_control_node * dry_mix;
   
       fx_control_node * wet_mix;
   
   
   
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
   
   
       void enable() {
         CHECK_LAST_ENABLED();
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_BOOL, FX_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_BOOL, FX_DELAY_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void set_length_ms(float len_ms) { 
   
         CHECK_LAST(len_ms, param_len_ms);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_len_ms.connected) {
           return;
         }
   
         param_len_ms = len_ms; 
         parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_LEN_MS, (void *) &param_len_ms);
       }
   
       void set_feedback(float feedback) { 
   
         CHECK_LAST(feedback, param_feedback);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_feedback.connected) {
           return; 
         }
   
         param_feedback = feedback; 
         parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_FEEDBACK, &param_feedback);
       }
   
       void set_dry_mix(float dry_mix) { 
         
         CHECK_LAST(dry_mix, param_dry_mix);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_dry_mix.connected) {
           return; 
         }
   
         param_dry_mix = dry_mix; 
         parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_DRY_MIX, &param_dry_mix);
       }
   
   
       void set_wet_mix(float wet_mix) { 
         
         CHECK_LAST(wet_mix, param_wet_mix);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_wet_mix.connected) {
           return; 
         }
   
         param_wet_mix = wet_mix; 
         parent_canvas->spi_transmit_param(FX_DELAY, instance_id, T_FLOAT, FX_DELAY_PARAM_ID_WET_MIX, &param_wet_mix);
       }
   
     
   
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
   
   };
   
   #endif // DM_FX_DELAY_H
