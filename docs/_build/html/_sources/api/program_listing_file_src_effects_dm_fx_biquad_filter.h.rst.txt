
.. _program_listing_file_src_effects_dm_fx_biquad_filter.h:

Program Listing for File dm_fx_biquad_filter.h
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_biquad_filter.h>` (``src/effects/dm_fx_biquad_filter.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DM_FX_BIQUAD_FILTER_H
   #define DM_FX_BIQUAD_FILTER_H
   
   class fx_biquad_filter: public fx_effect {
   
     private:
   
       // Parameters
       // important safety tip - don't put 8-bit enum types together as they need to exist on 16 or 32-bit memory boundaries
       BIQUAD_FILTER_TYPE          param_type;
       float                   param_freq;
       BIQUAD_FILTER_ORDER     param_order;
       float                   param_q;
       float                   param_gain;
       EFFECT_TRANSITION_SPEED param_speed;
   
         
         fx_control_node node_ctrl_freq;
         fx_control_node node_ctrl_q;
         fx_control_node node_ctrl_gain;
   
       void init(void) {
   
           // Set class
           type = FX_BIQUAD_FILTER;
   
           // Set name
           strcpy(effect_name, "biquad filter");
   
   
           // Assign programmable node names
           input = &node_input;
           output = &node_output;
           
               // Initialize parameter stack
           int indx = 1;
           param_stack[indx] = &param_type;
           param_stack_types[indx++] = T_INT16;
           param_stack[indx] = &param_speed;
           param_stack_types[indx++] = T_INT16;
           param_stack[indx] = &param_freq;
           param_stack_types[indx++] = T_FLOAT;
           param_stack[indx] = &param_q;
           param_stack_types[indx++] = T_FLOAT;
           param_stack[indx] = &param_gain;
           param_stack_types[indx++] = T_FLOAT;
         param_stack[indx] = &param_order;
         param_stack_types[indx++] = T_INT16;
           total_params = indx;    
   
           // Add addititonal nodes to the control stack
           control_node_stack[total_control_nodes++] = &node_ctrl_freq;
           control_node_stack[total_control_nodes++] = &node_ctrl_q;
           control_node_stack[total_control_nodes++] = &node_ctrl_gain;
   
           // Assign controls
           freq = &node_ctrl_freq;
           q = &node_ctrl_q;
           gain = &node_ctrl_gain;
       }
   
   
   
    public:
   
     fx_audio_node * input;
   
     fx_audio_node * output;
   
     fx_control_node * freq;
   
   
     fx_control_node * q;
   
   
     fx_control_node * gain;    
   
   
     fx_biquad_filter(float filt_freq, float filt_resonance, BIQUAD_FILTER_TYPE filt_type) :
       node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
       node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
       node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {
   
       // Set parameters
       param_freq = filt_freq;
       if (filt_resonance <= 0.1) {
         filt_resonance = 0.1;
       } else if (filt_resonance > 20.0) {
         filt_resonance = 20.0;
       }
       param_q = filt_resonance * 0.7071;
       param_gain = 0.0;    
       param_type = filt_type; 
       param_speed = TRANS_MED;
       param_order = BIQUAD_ORDER_2;
   
       init();
   
     } 
   
     fx_biquad_filter(float filt_freq, float filt_resonance, BIQUAD_FILTER_TYPE filt_type, BIQUAD_FILTER_ORDER order) :
       node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
       node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
       node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {
   
       // Set parameters
       param_freq = filt_freq;
       if (filt_resonance <= 0.1) {
         filt_resonance = 0.1;
       } else if (filt_resonance > 20.0) {
         filt_resonance = 20.0;
       }
       param_q = filt_resonance * 0.7071;
       param_gain = 0.0;    
       param_type = filt_type; 
       param_speed = TRANS_MED;
       param_order = order;
   
       init();
   
     }   
   
   
     fx_biquad_filter(float filt_freq, float filt_resonance, float filter_gain, BIQUAD_FILTER_TYPE filt_type, EFFECT_TRANSITION_SPEED trans_speed) :
       node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
       node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
       node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {
   
       // Set parameters
       param_freq = filt_freq;
       if (filt_resonance <= 0.1) {
         filt_resonance = 0.1;
       } else if (filt_resonance > 20.0) {
         filt_resonance = 20.0;
       }
       param_q = filt_resonance * 0.7071;
       param_gain = filter_gain;    
       param_type = filt_type; 
       param_speed = trans_speed;
       param_order = BIQUAD_ORDER_2;
   
       init();
   
     } 
   
     fx_biquad_filter(float filt_freq, float filt_resonance, float filter_gain_db, BIQUAD_FILTER_TYPE filt_type, EFFECT_TRANSITION_SPEED trans_speed, BIQUAD_FILTER_ORDER order) :
       node_ctrl_freq(NODE_IN, NODE_FLOAT, "node_ctrl_freq", this, FX_BIQUAD_PARAM_ID_FREQ),
       node_ctrl_q(NODE_IN, NODE_FLOAT, "node_ctrl_q", this, FX_BIQUAD_PARAM_ID_Q),
       node_ctrl_gain(NODE_IN, NODE_FLOAT, "node_ctrl_gain", this, FX_BIQUAD_PARAM_ID_GAIN) {
   
       // Set parameters
       param_freq = filt_freq;
       if (filt_resonance <= 0.1) {
         filt_resonance = 0.1;
       } else if (filt_resonance > 20.0) {
         filt_resonance = 20.0;
       }
       param_q = filt_resonance * 0.7071;
       param_gain = filter_gain_db;    
       param_type = filt_type; 
       param_speed = trans_speed;
       param_order = order;
   
       init();
   
     } 
   
     void enable() {
       CHECK_LAST_ENABLED();
       param_enabled = true; 
       parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_BOOL, FX_BIQUAD_PARAM_ID_ENABLED, (void *) &param_enabled);
     }
   
     void bypass() {
       CHECK_LAST_DISABLED();
       param_enabled = false; 
       parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_BOOL, FX_BIQUAD_PARAM_ID_ENABLED, (void *) &param_enabled);
     }  
   
   
     void set_freq(float freq) { 
   
       CHECK_LAST(freq, param_freq);
   
       // If this node is being controlled by a controller, don't allow a direct write to it
       if (node_ctrl_freq.connected) {
         return; 
       }
   
       param_freq = freq; 
       parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_FREQ, &param_freq);
     }
   
     void set_q(float q) { 
   
       CHECK_LAST(q, param_q)
   
       if (node_ctrl_q.connected) {
         return; 
       }
   
       param_q = q; 
       parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_Q, &param_q);
     }    
   
     void set_resonance(float filt_resonance) { 
   
       if (node_ctrl_q.connected) {
         return; 
       }
   
       if (filt_resonance <= 0.1) {
         filt_resonance = 0.1;
       } else if (filt_resonance > 20.0) {
         filt_resonance = 20.0;
       }
   
       filt_resonance = filt_resonance * 0.7071;
   
       CHECK_LAST(filt_resonance, param_q);
   
       param_q = filt_resonance * 0.7071;
   
       parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_Q, &param_q);
     }   
   
     void set_gain(float gain) { 
   
       CHECK_LAST(gain, param_gain);
       if (node_ctrl_gain.connected) {
         return; 
       }
   
       param_gain = gain; 
       parent_canvas->spi_transmit_param(FX_BIQUAD_FILTER, instance_id, T_FLOAT, FX_BIQUAD_PARAM_ID_GAIN, &param_gain);
     }   
   
     void  print_params(void) {
   
           // void print_parameter( void * val, char * name, PARAM_TYPES type)
           Serial.println("Parameters:");
       print_parameter( &param_enabled, "Enabled", T_BOOL );
       print_parameter( &param_freq, "Frequency (Hz)", T_FLOAT );
       print_parameter( &param_q, "Width/Q", T_FLOAT );
           print_parameter( &param_gain, "Gain (db)", T_FLOAT );
       print_parameter( &param_order, "Order (enumerated))", T_INT16 );
   
       Serial.println("Control Routing:");      
       print_ctrl_node_status(&node_ctrl_freq);
       print_ctrl_node_status(&node_ctrl_q);
       print_ctrl_node_status(&node_ctrl_gain);
   
       Serial.println("Audio Routing:");      
       print_audio_node_status(&node_input);
       print_audio_node_status(&node_output);
   
       Serial.println();
     }    
   };
   #endif  // DM_FX_BIQUAD_FILTER_H
