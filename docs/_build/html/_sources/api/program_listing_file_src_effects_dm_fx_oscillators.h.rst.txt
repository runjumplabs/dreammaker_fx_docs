
.. _program_listing_file_src_effects_dm_fx_oscillators.h:

Program Listing for File dm_fx_oscillators.h
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_oscillators.h>` (``src/effects/dm_fx_oscillators.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   
   #ifndef DM_FX_OSCILLATOR_H
   #define DM_FX_OSCILLATOR_H
   
   class fx_oscillator: public fx_effect {
   
     private:
       OSC_TYPES   param_type;
       float       param_freq;
       float       param_amp;
       float       param_offset;
       float       param_osc_param1;
       float       param_osc_param2;
       float     param_osc_initial_phase;
   
       // Control nodes
       fx_control_node node_ctrl_freq;
       fx_control_node node_ctrl_amp;
       fx_control_node node_ctrl_offset;
       fx_control_node node_ctrl_value;
   
       void init(void) {
          // Set class
         type = FX_OSCILLATOR;
   
         // Set name
         strcpy(effect_name, "oscillator");
   
         // Assign programmable node names
         output = &node_output;      
   
         // Initialize parameter stack
         int indx = 1;
         param_stack[indx] = &param_freq;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_amp;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_offset;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_type;
         param_stack_types[indx++] = T_INT16;
   
         param_stack[indx] = &param_osc_param1;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_osc_param2;
         param_stack_types[indx++] = T_FLOAT;
   
         param_stack[indx] = &param_osc_initial_phase;
         param_stack_types[indx++] = T_FLOAT;
   
         total_params = indx;      
   
   
         // Add addiitonal notes to the control stack
         control_node_stack[total_control_nodes++] = &node_ctrl_freq;
         control_node_stack[total_control_nodes++] = &node_ctrl_amp;
         control_node_stack[total_control_nodes++] = &node_ctrl_offset;
         control_node_stack[total_control_nodes++] = &node_ctrl_value;
   
         freq = &node_ctrl_freq;
         amplitude = &node_ctrl_amp;
         offset = &node_ctrl_offset;
         value = &node_ctrl_value;      
       }
   
   
    public:
   
     fx_audio_node * output;
   
       fx_control_node * freq;
   
       fx_control_node * amplitude;
   
       fx_control_node * offset;
   
       fx_control_node * value;
   
     fx_oscillator(OSC_TYPES osc_type, float freq, float amplitude ) : 
       node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_OSCILLATOR_PARAM_ID_FREQ),
       node_ctrl_amp(NODE_IN, NODE_FLOAT, "node_ctrl_amp", this, FX_OSCILLATOR_PARAM_ID_AMP),
       node_ctrl_offset(NODE_IN, NODE_FLOAT, "node_ctrl_offset", this, FX_OSCILLATOR_PARAM_ID_OFFSET),
       node_ctrl_value(NODE_OUT, NODE_FLOAT, "node_ctrl_value", this, FX_OSCILLATOR_PARAM_ID_OSC_CTRL_VAL)  {
   
         // Set parameters
         param_type = osc_type;
         param_freq = freq;
         param_amp = amplitude;
         param_offset = 0.0;
   
         param_osc_param1 = 0.0;
         param_osc_param2 = 0.0;
   
         init();
   
       }
   
   
     fx_oscillator(OSC_TYPES   osc_type, float freq, float amplitude, float initial_phase ) : 
       node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_OSCILLATOR_PARAM_ID_FREQ),
       node_ctrl_amp(NODE_IN, NODE_FLOAT, "node_ctrl_amp", this, FX_OSCILLATOR_PARAM_ID_AMP),
       node_ctrl_offset(NODE_IN, NODE_FLOAT, "node_ctrl_offset", this, FX_OSCILLATOR_PARAM_ID_OFFSET),
       node_ctrl_value(NODE_OUT, NODE_FLOAT, "node_ctrl_value", this, FX_OSCILLATOR_PARAM_ID_OSC_CTRL_VAL)  {
   
         // Set parameters
         param_type = osc_type;
         param_freq = freq;
         param_amp = amplitude;
         param_offset = 0.0;
         param_osc_initial_phase = initial_phase;
   
         param_osc_param1 = 0.0;
         param_osc_param2 = 0.0;
   
         init();
   
       }
   
       void enable() {
         CHECK_LAST_ENABLED();      
         param_enabled = true; 
         parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_BOOL, FX_OSCILLATOR_PARAM_ID_ENABLED, (void *) &param_enabled);
       }
   
       void bypass() {
         CHECK_LAST_DISABLED();
         param_enabled = false; 
         parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_BOOL, FX_OSCILLATOR_PARAM_ID_ENABLED, (void *) &param_enabled);
       }      
       
   
       void set_frequency(float freq) { 
   
         CHECK_LAST(freq, param_freq)
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_freq.connected) {
           return; 
         }
   
         param_freq = freq; 
         parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_FLOAT, FX_OSCILLATOR_PARAM_ID_FREQ, &param_freq);
       }   
   
   
       void set_amplitude(float amplitude) { 
   
         CHECK_LAST(amplitude, param_amp)
   
         // If this node is being controlled by a controller, don't allow a direct write to it
         if (node_ctrl_amp.connected) {
           return; 
         }
   
         param_amp = amplitude; 
         parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_FLOAT, FX_OSCILLATOR_PARAM_ID_AMP, &param_amp);
       }   
   
       void set_oscillator_type(OSC_TYPES new_type) {
   
         CHECK_LAST(new_type, param_type)
   
         param_type = new_type; 
         parent_canvas->spi_transmit_param(FX_OSCILLATOR, instance_id, T_INT16, FX_OSCILLATOR_PARAM_ID_TYPE, &param_type);
   
       }
   
   
       void  print_params(void) {
   
         // void print_parameter( void * val, char * name, PARAM_TYPES type)
         Serial.println("Parameters:");
         print_parameter( &param_enabled, "Enabled", T_BOOL );
         print_parameter( &param_type, "Oscillator type", T_INT16 );
         print_parameter( &param_freq, "Frequency (Hz)", T_FLOAT );
         print_parameter( &param_amp, "Amplitude", T_FLOAT );
         print_parameter( &param_offset, "Offset", T_FLOAT );
   
         Serial.println("Control Routing:");      
         print_ctrl_node_status(&node_ctrl_freq);
         print_ctrl_node_status(&node_ctrl_amp);
         print_ctrl_node_status(&node_ctrl_offset);
         print_ctrl_node_status(&node_ctrl_value);
   
         Serial.println("Audio Routing:");      
         print_audio_node_status(&node_input);
         print_audio_node_status(&node_output);
   
         Serial.println();
       }    
   
   };
   
   
   #endif  // DM_FX_OSCILLATOR_H
