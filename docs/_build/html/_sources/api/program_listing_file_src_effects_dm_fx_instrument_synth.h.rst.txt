
.. _program_listing_file_src_effects_dm_fx_instrument_synth.h:

Program Listing for File dm_fx_instrument_synth.h
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_instrument_synth.h>` (``src/effects/dm_fx_instrument_synth.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

    // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DM_FX_INSTRUMENT_SYNTH_H
   #define DM_FX_INSTRUMENT_SYNTH_H
   
   class fx_instrument_synth: public fx_effect {
   
     private:
   
       // Parameters
       OSC_TYPES param_osc_type;
       float param_freq_ratio;
       OSC_TYPES param_fm_osc_type;
       float param_fm_mod_freq_ratio;
       float param_fm_mod_depth;
       float param_attack_ms;
       float param_filt_resonance;
       float param_filt_response;
   
       OSC_TYPES last_type;
       OSC_TYPES last_type_fm_mod;
   
       // Control nodes
       fx_control_node node_ctrl_attack_ms;
       fx_control_node node_ctrl_freq_ratio;
       fx_control_node node_ctrl_dm_mod_freq_ratio;
       fx_control_node node_ctrl_dm_mod_depth;
       fx_control_node node_ctrl_filt_resonance;
       fx_control_node node_ctrl_filt_response;
   
           void init(void) {
             // Set class
             type = FX_INSTRUMENT_SYNTH;
   
             // Set name
             strcpy(effect_name, "instrument synth");
   
             // Assign programmable node names
             output = &node_output;
   
             // Initialize parameter stack
             int indx = 1;
             param_stack[indx] = &param_osc_type;
             param_stack_types[indx++] = T_INT16;
             param_stack[indx] = &param_fm_osc_type;
             param_stack_types[indx++] = T_INT16;    
             param_stack[indx] = &param_freq_ratio;
             param_stack_types[indx++] = T_FLOAT;
             param_stack[indx] = &param_fm_mod_freq_ratio;
             param_stack_types[indx++] = T_FLOAT;
             param_stack[indx] = &param_fm_mod_depth;
             param_stack_types[indx++] = T_FLOAT;
             param_stack[indx] = &param_attack_ms;
             param_stack_types[indx++] = T_FLOAT;
             param_stack[indx] = &param_filt_resonance;
             param_stack_types[indx++] = T_FLOAT;
             param_stack[indx] = &param_filt_response;
             param_stack_types[indx++] = T_FLOAT;
   
             total_params = indx;
   
             // Add addiitonal notes to the control stack
             control_node_stack[total_control_nodes++] = &node_ctrl_freq_ratio;
             control_node_stack[total_control_nodes++] = &node_ctrl_attack_ms;
             control_node_stack[total_control_nodes++] = &node_ctrl_dm_mod_freq_ratio;
             control_node_stack[total_control_nodes++] = &node_ctrl_dm_mod_depth;
             control_node_stack[total_control_nodes++] = &node_ctrl_filt_resonance;
             control_node_stack[total_control_nodes++] = &node_ctrl_filt_response;
   
             freq_ratio = &node_ctrl_freq_ratio;
             attack_ms = &node_ctrl_attack_ms;
             fm_mod_freq_ratio = &node_ctrl_dm_mod_freq_ratio;
             fm_mod_depth = &node_ctrl_dm_mod_depth;
             resonance = &node_ctrl_filt_resonance;
             response = &node_ctrl_filt_response;
   
           }    
   
       public:
   
       fx_audio_node * output;
   
       fx_control_node * attack_ms;    
   
       fx_control_node * freq_ratio;    
   
       fx_control_node * fm_mod_freq_ratio;    
           
       fx_control_node * fm_mod_depth;        
   
       fx_control_node * resonance; 
   
       fx_control_node * response; 
   
       fx_instrument_synth( OSC_TYPES osc_type, float attack_ms, float filter_resonance, float filter_response) :
           node_ctrl_freq_ratio(NODE_IN, NODE_FLOAT, "node_ctrl_freq_ratio", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FREQ_RATIO),
               node_ctrl_dm_mod_freq_ratio(NODE_IN, NODE_FLOAT, "node_ctrl_dm_mod_freq_ratio", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_RATIO),
               node_ctrl_dm_mod_depth(NODE_IN, NODE_FLOAT, "node_ctrl_dm_mod_depth", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_DEPTH),
               node_ctrl_attack_ms(NODE_IN, NODE_FLOAT, "node_ctrl_attack_ms", this, FX_INSTRUMENT_SYNTH_PARAM_ID_ATTACK_MS),
               node_ctrl_filt_resonance(NODE_IN, NODE_FLOAT, "node_ctrl_filt_resonance", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESONANCE),
               node_ctrl_filt_response(NODE_IN, NODE_FLOAT, "param_filt_response", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESPONSE) {
   
               param_osc_type = osc_type;
               param_attack_ms = attack_ms; 
               param_filt_resonance = filter_resonance;
               param_filt_response = filter_response;
   
               param_fm_osc_type = OSC_NONE;
               param_fm_mod_depth = 0.0;
   
               param_fm_mod_freq_ratio = 1.0;
               param_freq_ratio = 1.0;
   
               init();
   
       }
   
       fx_instrument_synth( OSC_TYPES osc_type, OSC_TYPES fm_mod_osc_type, float fm_mod_depth, float freq_ratio, float freq_ratio_fm_mod, float attack_ms, float filter_resonance, float filter_response) :
           node_ctrl_freq_ratio(NODE_IN, NODE_FLOAT, "node_ctrl_freq_ratio", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FREQ_RATIO),
               node_ctrl_dm_mod_freq_ratio(NODE_IN, NODE_FLOAT, "node_ctrl_dm_mod_freq_ratio", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_RATIO),
               node_ctrl_dm_mod_depth(NODE_IN, NODE_FLOAT, "node_ctrl_dm_mod_depth", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_DEPTH),
               node_ctrl_attack_ms(NODE_IN, NODE_FLOAT, "node_ctrl_attack_ms", this, FX_INSTRUMENT_SYNTH_PARAM_ID_ATTACK_MS),
               node_ctrl_filt_resonance(NODE_IN, NODE_FLOAT, "node_ctrl_filt_resonance", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESONANCE),
               node_ctrl_filt_response(NODE_IN, NODE_FLOAT, "node_ctrl_filt_response", this, FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESPONSE) {
   
               param_osc_type = osc_type;
               param_attack_ms = attack_ms; 
               param_filt_resonance = filter_resonance;
               param_filt_response = filter_response;
   
               param_fm_osc_type = fm_mod_osc_type;
               param_fm_mod_depth = fm_mod_depth;
   
               param_fm_mod_freq_ratio = freq_ratio_fm_mod;
               param_freq_ratio = freq_ratio;
   
               init();
           }
   
       void enable() {
         CHECK_LAST_ENABLED();
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_BOOL, FX_INSTRUMENT_SYNTH_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_BOOL, FX_INSTRUMENT_SYNTH_PARAM_ID_ENABLED, (void *) &param_enabled);
       }           
   
     
   
       void set_freq_ratio(float ratio) { 
   
           CHECK_LAST(ratio, param_freq_ratio);
   
           if (node_ctrl_freq_ratio.connected) {
             return; 
           }
   
           param_freq_ratio = ratio;
           parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_FLOAT, FX_INSTRUMENT_SYNTH_PARAM_ID_FREQ_RATIO, &param_freq_ratio);
       }  
   
         void set_fm_mod_ratio(float fm_mod_ratio) {
   
           CHECK_LAST(fm_mod_ratio, param_fm_mod_freq_ratio);
   
           if (node_ctrl_dm_mod_freq_ratio.connected) {
             return; 
           }
   
           param_fm_mod_freq_ratio = fm_mod_ratio;
           parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_FLOAT, FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_RATIO, &param_fm_mod_freq_ratio);
   
         }
   
       void set_fm_mod_depth(float depth) {
   
           CHECK_LAST(depth, param_fm_mod_depth);
   
   
           if (node_ctrl_dm_mod_depth.connected) {
               return; 
           }
   
             param_fm_mod_depth = depth;
             parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_FLOAT, FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_DEPTH, &param_fm_mod_depth);     
         }
   
       void set_attack_ms(float attack_ms) { 
   
           CHECK_LAST(attack_ms, param_attack_ms);
   
           if (node_ctrl_attack_ms.connected) {
             return; 
           }
   
           param_attack_ms = attack_ms;
           parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_FLOAT, FX_INSTRUMENT_SYNTH_PARAM_ID_ATTACK_MS, &param_attack_ms);
         }
   
       void set_filter_resonance(float resonance) {
   
           CHECK_LAST(resonance, param_filt_resonance);
   
           if (node_ctrl_filt_resonance.connected) {
             return; 
           }
   
           param_filt_resonance = resonance;
           parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_FLOAT, FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESONANCE, &param_filt_resonance);
   
       }
   
       void set_filter_response(float response) {
   
           CHECK_LAST(response, param_filt_response);
   
           if (node_ctrl_filt_response.connected) {
             return; 
           }
   
           param_filt_response = response;
           parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_FLOAT, FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESPONSE, &param_filt_response);
           
       }
   
       void set_oscillator_type(OSC_TYPES new_type) {
   
           CHECK_LAST(new_type, param_osc_type);
   
           param_osc_type = new_type; 
           parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_INT16, FX_INSTRUMENT_SYNTH_PARAM_ID_OSC_TYPE, &param_osc_type);
           
       }
   
       void set_oscillator_type_fm_mod(OSC_TYPES new_type) {
   
           CHECK_LAST(new_type, param_fm_osc_type);
   
           param_fm_osc_type = new_type; 
           parent_canvas->spi_transmit_param(FX_INSTRUMENT_SYNTH, instance_id, T_INT16, FX_INSTRUMENT_SYNTH_PARAM_ID_OSC_FM_TYPE, &param_fm_osc_type);
       
       }
   
   
   
   
       void  print_params(void) {
         // void print_parameter( void * val, char * name, PARAM_TYPES type)
         Serial.println("Parameters:");
         print_parameter( &param_enabled, "Enabled", T_BOOL );
         print_parameter( &param_osc_type, "OSC type (enumeration index)", T_INT16 );
         print_parameter( &param_fm_osc_type, "FM mod OSC type (enumeration index)", T_INT16 );
         print_parameter( &param_freq_ratio, "Synth frequency ratio", T_FLOAT);
         print_parameter( &param_fm_mod_freq_ratio, "FM mod frequency ratio", T_FLOAT );
         print_parameter( &param_fm_mod_depth, "FM mod depth", T_FLOAT );
         print_parameter( &param_attack_ms, "Attack (ms)", T_FLOAT );
         print_parameter( &param_filt_resonance, "Filter resonance", T_FLOAT );
         print_parameter( &param_filt_response, "Filter response", T_FLOAT );
   
         Serial.println("Control Routing:");      
         print_ctrl_node_status(&node_ctrl_attack_ms);
         print_ctrl_node_status(&node_ctrl_freq_ratio);
         print_ctrl_node_status(&node_ctrl_dm_mod_freq_ratio);
         print_ctrl_node_status(&node_ctrl_dm_mod_depth);
         print_ctrl_node_status(&node_ctrl_filt_resonance);
         print_ctrl_node_status(&node_ctrl_filt_response);
   
         Serial.println("Audio Routing:");      
         print_audio_node_status(&node_output);
   
         Serial.println();
       }    
   
   
   };
   
   
   #endif // DM_FX_INSTRUMENT_SYNTH_H
