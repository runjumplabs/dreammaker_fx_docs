
.. _program_listing_file_src_effects_dm_fx_adsr_envelope.h:

Program Listing for File dm_fx_adsr_envelope.h
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_adsr_envelope.h>` (``src/effects/dm_fx_adsr_envelope.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DM_FX_ADSR_ENVELOPE_H
   #define DM_FX_ADSR_ENVELOPE_H
   
   class fx_adsr_envelope: public fx_effect {
   
   private:
   
       // Parameters
       uint16_t param_start_stop;
   
       float param_attack_ms;
       float param_decay_ms;
       float param_sustain_ms;
       float param_release_ms;
       float param_peak_ratio;
       float param_sustain_ratio;
       float param_out_vol;
   
       // Control nodes
       fx_control_node node_ctrl_attack_ms;
       fx_control_node node_ctrl_decay_ms;
       fx_control_node node_ctrl_sustain_ms;
       fx_control_node node_ctrl_release_ms;
       fx_control_node node_ctrl_peak_ratio;
       fx_control_node node_ctrl_sustain_ratio;
       fx_control_node node_ctrl_out_vol;
       fx_control_node node_ctrl_start;
       fx_control_node node_ctrl_value;
   
       void init(void) {
         // Set class
         type = FX_ADSR_ENVELOPE;
   
         // Set name
         strcpy(effect_name, "adsr envelope");
   
         // Assign programmable node names
         input = &node_input;
         output = &node_output;
   
         // Initialize parameter stack
         int indx = 1;
         param_stack[indx] = &param_attack_ms;
         param_stack_types[indx++] = T_FLOAT;
         param_stack[indx] = &param_decay_ms;
         param_stack_types[indx++] = T_FLOAT;
         param_stack[indx] = &param_sustain_ms;
         param_stack_types[indx++] = T_FLOAT;
         param_stack[indx] = &param_release_ms;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_peak_ratio;
         param_stack_types[indx++] = T_FLOAT;
         param_stack[indx] = &param_sustain_ratio;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_out_vol;
         param_stack_types[indx++] = T_FLOAT;
   
         total_params = indx;
   
         // Add addiitonal notes to the control stack
         control_node_stack[total_control_nodes++] = &node_ctrl_attack_ms;
         control_node_stack[total_control_nodes++] = &node_ctrl_decay_ms;
         control_node_stack[total_control_nodes++] = &node_ctrl_sustain_ms;
         control_node_stack[total_control_nodes++] = &node_ctrl_release_ms;
         control_node_stack[total_control_nodes++] = &node_ctrl_peak_ratio;
         control_node_stack[total_control_nodes++] = &node_ctrl_sustain_ratio;
         control_node_stack[total_control_nodes++] = &node_ctrl_out_vol;
         control_node_stack[total_control_nodes++] = &node_ctrl_start;
         control_node_stack[total_control_nodes++] = &node_ctrl_value;
   
         start = &node_ctrl_start;
         attack_ms = &node_ctrl_attack_ms;
         decay_ms = &node_ctrl_decay_ms;
         sustain_ms = &node_ctrl_sustain_ms;
         release_ms = &node_ctrl_release_ms;
         peak_ratio = &node_ctrl_peak_ratio;
         sustain_ratio = &node_ctrl_sustain_ratio;
         gain_out = &node_ctrl_out_vol;
           value = &node_ctrl_value;
   
       }    
   
    public:
   
       fx_audio_node * input;
   
       fx_audio_node * output;
   
       fx_control_node * attack_ms;    
   
       fx_control_node * decay_ms;    
   
       fx_control_node * sustain_ms;    
   
       fx_control_node * release_ms;    
   
       fx_control_node * peak_ratio;    
   
       fx_control_node * sustain_ratio;    
   
       fx_control_node * gain_out;    
   
       fx_control_node * start;    
   
       fx_control_node * value;       
   
       fx_adsr_envelope(float attack_ms, float decay_ms, float sustain_ms, float release_ms, float sustain_ratio, float gain_out) : 
         node_ctrl_attack_ms(NODE_IN, NODE_FLOAT, "node_ctrl_attack_ms", this, FX_ADSR_PARAM_ID_ATK_MS),
         node_ctrl_decay_ms(NODE_IN, NODE_FLOAT, "node_ctrl_decay_ms", this, FX_ADSR_PARAM_ID_DEC_MS),
         node_ctrl_sustain_ms(NODE_IN, NODE_FLOAT, "node_ctrl_sustain_ms", this, FX_ADSR_PARAM_ID_SUS_MS),
         node_ctrl_release_ms(NODE_IN, NODE_FLOAT, "node_ctrl_release_ms", this, FX_ADSR_PARAM_ID_RLS_MS),
         node_ctrl_peak_ratio(NODE_IN, NODE_FLOAT, "node_ctrl_peak_ratio", this, FX_ADSR_PARAM_ID_PEAK_RATIO),
         node_ctrl_sustain_ratio(NODE_IN, NODE_FLOAT, "node_ctrl_sustain_ratio", this, FX_ADSR_PARAM_ID_SUSTAIN_RATIO),
         node_ctrl_out_vol(NODE_IN, NODE_FLOAT, "node_ctrl_out_vol", this, FX_ADSR_PARAM_ID_OUT_VOL),
         node_ctrl_start(NODE_IN, NODE_FLOAT, "node_ctrl_start", this, FX_ADSR_PARAM_ID_START),
         node_ctrl_value(NODE_OUT, NODE_FLOAT, "node_ctrl_value", this, FX_ADSR_PARAM_ID_VALUE)
   
         {
   
         // Set parameters
         param_attack_ms = attack_ms;
         param_decay_ms = decay_ms;
         param_sustain_ms = sustain_ms;
         param_release_ms = release_ms;
   
         param_peak_ratio = 1.0;
         param_sustain_ratio = sustain_ratio;
   
         param_out_vol = gain_out;
   
         param_start_stop = 0;
   
         init();
       }
   
       void enable() {
         CHECK_LAST_ENABLED();
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_ADSR_ENVELOPE, instance_id, T_BOOL, FX_ADSR_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_ADSR_ENVELOPE, instance_id, T_BOOL, FX_ADSR_PARAM_ID_ENABLED, (void *) &param_enabled);
       }      
   
   
   
   
       void set_attack_ms(float attack) {
         CHECK_LAST(attack, param_attack_ms);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_attack_ms.connected) {
           return;
         }
   
         param_attack_ms = attack; 
         parent_canvas->spi_transmit_param(FX_ADSR_ENVELOPE, instance_id, T_FLOAT, FX_ADSR_PARAM_ID_ATK_MS, &param_attack_ms);
   
       }
   
       void set_decay_ms(float decay) {
         CHECK_LAST(decay, param_decay_ms);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_decay_ms.connected) {
           return;
         }
   
         param_decay_ms = decay; 
         parent_canvas->spi_transmit_param(FX_ADSR_ENVELOPE, instance_id, T_FLOAT, FX_ADSR_PARAM_ID_DEC_MS, &param_decay_ms);
   
       }
   
       void set_sustain_ms(float sustain) {
         CHECK_LAST(sustain, param_sustain_ms);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_sustain_ms.connected) {
           return;
         }
   
         param_sustain_ms = sustain; 
         parent_canvas->spi_transmit_param(FX_ADSR_ENVELOPE, instance_id, T_FLOAT, FX_ADSR_PARAM_ID_SUS_MS, &param_sustain_ms);
       }
   
       void set_release_ms(float release) {
         CHECK_LAST(release, param_release_ms);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_release_ms.connected) {
           return;
         }
   
         param_release_ms = release; 
         parent_canvas->spi_transmit_param(FX_ADSR_ENVELOPE, instance_id, T_FLOAT, FX_ADSR_PARAM_ID_RLS_MS, &param_release_ms);
       }
   
   
       void set_output_gain(float gain) {
   
         CHECK_LAST(gain, param_out_vol);
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_out_vol.connected) {
           return;
         }
   
         param_out_vol = gain; 
         parent_canvas->spi_transmit_param(FX_ADSR_ENVELOPE, instance_id, T_FLOAT, FX_ADSR_PARAM_ID_OUT_VOL, &param_out_vol);      
       }
   
   
       void  print_params(void) {
   
         // void print_parameter( void * val, char * name, PARAM_TYPES type)
         Serial.println("Parameters:");
         print_parameter( &param_enabled, "Enabled", T_BOOL );
   
         print_parameter( &param_attack_ms, "Attack (ms)", T_FLOAT );
         print_parameter( &param_decay_ms, "Decay (ms)", T_FLOAT );
         print_parameter( &param_sustain_ms, "Sustain (ms)", T_FLOAT );
         print_parameter( &param_release_ms, "Release (ms)", T_FLOAT );
   
         print_parameter( &param_peak_ratio, "Peak ratio", T_FLOAT );
         print_parameter( &param_sustain_ratio, "Sustain ratio", T_FLOAT );
         print_parameter( &param_out_vol, "Output volume", T_FLOAT );
   
   
         Serial.println("Control Routing:");      
         print_ctrl_node_status(&node_ctrl_attack_ms);
         print_ctrl_node_status(&node_ctrl_decay_ms);
         print_ctrl_node_status(&node_ctrl_sustain_ms);
         print_ctrl_node_status(&node_ctrl_release_ms);
   
         print_ctrl_node_status(&node_ctrl_peak_ratio);
         print_ctrl_node_status(&node_ctrl_sustain_ratio);
         print_ctrl_node_status(&node_ctrl_out_vol);
   
         print_ctrl_node_status(&node_ctrl_start);
         print_ctrl_node_status(&node_ctrl_value);
   
   
         Serial.println("Audio Routing:");      
         print_audio_node_status(&node_input);
         print_audio_node_status(&node_output);
   
         Serial.println();
       }
   
   
   };
   
   
   #endif  // DM_FX_ADSR_ENVELOPE_H
