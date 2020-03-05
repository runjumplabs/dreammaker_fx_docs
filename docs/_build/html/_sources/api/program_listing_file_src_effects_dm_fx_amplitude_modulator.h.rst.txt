
.. _program_listing_file_src_effects_dm_fx_amplitude_modulator.h:

Program Listing for File dm_fx_amplitude_modulator.h
====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_amplitude_modulator.h>` (``src/effects/dm_fx_amplitude_modulator.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DM_FX_AMP_MOD_H
   #define DM_FX_AMP_MOD_H
   
   class fx_amplitude_mod: public fx_effect {
   
     private:
   
       // Parameters
       float     param_depth;
       float     param_rate_hz;
       OSC_TYPES param_type;
       float     param_phase_deg;
       bool      param_ext_modulator;
   
       OSC_TYPES last_type;
   
       void init() {
           // Set class
           type = FX_AMPLITUDE_MODULATOR;
   
           // Set name
           strcpy(effect_name, "amplitude modulator");      
   
               // Assign programmable node names
           input = &node_input;
           output = &node_output;
           ext_mod_in = &node_loop_ext_mod;      
   
               // Initialize parameter stack
           int indx = 1;
           param_stack[indx] = &param_rate_hz;
           param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_phase_deg;
         param_stack_types[indx++] = T_FLOAT;
   
           param_stack[indx] = &param_depth;
           param_stack_types[indx++] = T_FLOAT;
   
           param_stack[indx] = &param_type;
           param_stack_types[indx++] = T_INT16;
   
           param_stack[indx] = &param_ext_modulator;
           param_stack_types[indx++] = T_BOOL;
           total_params = indx;         
   
           // Add additional nodes to the audio stack
           audio_node_stack[total_audio_nodes++] = &node_loop_ext_mod;
   
           // Assign controls
           depth = &node_ctrl_depth;
           rate_hz = &node_ctrl_rate_hz;
           
         } 
   
           // Additional audio nodes
         fx_audio_node node_loop_ext_mod;
   
         // Control nodes
         fx_control_node node_ctrl_depth;
         fx_control_node node_ctrl_rate_hz;
   
    public:
   
   
   
     fx_audio_node * input;
     fx_audio_node * output;
     fx_audio_node * ext_mod_in;
   
   
     fx_control_node * depth;
     fx_control_node * rate_hz;
   
       fx_amplitude_mod(float rate_hz, float depth) : 
       node_loop_ext_mod(NODE_IN, "external modulator", this),
       node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_AMP_MOD_PARAM_ID_MOD_DEPTH),
       node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_AMP_MOD_PARAM_ID_MOD_FREQ) {
               
               // Set parameters
           param_depth = depth;
           param_rate_hz = rate_hz;
           last_type = param_type = OSC_SINE;
           param_ext_modulator = false;
         param_phase_deg = 0;
   
           init();
     }
   
     fx_amplitude_mod(float rate_hz, float depth, float initial_phase_deg, OSC_TYPES modulation_type, bool use_ext_modulator) :
       node_loop_ext_mod(NODE_IN, "external modulator", this),
       node_ctrl_depth(NODE_IN, NODE_FLOAT, "node_ctrl_depth", this, FX_AMP_MOD_PARAM_ID_MOD_DEPTH),
       node_ctrl_rate_hz(NODE_IN, NODE_FLOAT, "node_ctrl_rate_hz", this, FX_AMP_MOD_PARAM_ID_MOD_FREQ) {
   
               // Set parameters
           param_depth = depth;
           param_rate_hz = rate_hz;
         param_phase_deg = initial_phase_deg;      
           last_type = param_type = modulation_type;
           param_ext_modulator = use_ext_modulator;
   
           init();
       }
     
     void enable() {    
       CHECK_LAST_ENABLED();
       param_enabled = true; 
       parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_BOOL, FX_AMP_MOD_PARAM_ID_ENABLED, (void *) &param_enabled);
     }
   
     void bypass() {
       CHECK_LAST_DISABLED();
       param_enabled = false; 
       parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_BOOL, FX_AMP_MOD_PARAM_ID_ENABLED, (void *) &param_enabled);
     }  
   
   
     void set_depth(float depth) { 
       CHECK_LAST(depth, param_depth);
       
       if (node_ctrl_depth.connected) {
         return; 
       }
   
       param_depth = depth; 
       parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_FLOAT, FX_AMP_MOD_PARAM_ID_MOD_DEPTH, &param_depth);
     }
   
   
     void set_rate_hz(float rate_hz) { 
       CHECK_LAST(rate_hz, param_rate_hz);
   
       if (node_ctrl_rate_hz.connected) {
         return; 
       }
   
       param_rate_hz = rate_hz; 
       parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_FLOAT, FX_AMP_MOD_PARAM_ID_MOD_FREQ, &param_rate_hz);
     }    
   
   
     void set_lfo_type(OSC_TYPES new_type) {
       CHECK_LAST(new_type, param_type);
   
       param_type = new_type; 
       
       if (new_type != last_type) {
         parent_canvas->spi_transmit_param(FX_AMPLITUDE_MODULATOR, instance_id, T_INT16, FX_AMP_MOD_PARAM_ID_MOD_TYPE, &param_type);
       }
       last_type = new_type;
   
     }
   
     void  print_params(void) {
   
           // void print_parameter( void * val, char * name, PARAM_TYPES type)
           Serial.println("Parameters:");
       print_parameter( &param_enabled, "Enabled", T_BOOL );
       print_parameter( &param_depth, "Depth", T_FLOAT );
       print_parameter( &param_rate_hz, "Rate (Hz)", T_FLOAT );
       print_parameter( &param_phase_deg, "Initial phase (degrees)", T_FLOAT );
       print_parameter( &param_type, "Oscillator type", T_INT16 );
       print_parameter( &param_ext_modulator, "External modulator", T_BOOL );
   
       Serial.println("Control Routing:");      
       print_ctrl_node_status(&node_ctrl_depth);
       print_ctrl_node_status(&node_ctrl_rate_hz);
   
       Serial.println("Audio Routing:");      
       print_audio_node_status(&node_input);
       print_audio_node_status(&node_output);
       print_audio_node_status(&node_loop_ext_mod);
   
       Serial.println();
     }
   
   };
   
   #endif  // DM_FX_AMP_MOD_H
