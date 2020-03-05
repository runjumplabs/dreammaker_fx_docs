
.. _program_listing_file_src_effects_dm_fx_mixers.h:

Program Listing for File dm_fx_mixers.h
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_mixers.h>` (``src/effects/dm_fx_mixers.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   
   #ifndef DM_FX_MIXERS_H
   #define DM_FX_MIXERS_H
   
   class fx_mixer_2: public fx_effect {
   
     private:
       // Additional audio nodes
       fx_audio_node node_input_2;
   
       public:
   
       fx_audio_node * input_1;
   
       fx_audio_node * input_2;
   
       fx_audio_node * output;
   
           fx_mixer_2(void) : node_input_2(NODE_IN, "input_2", this) {
         // Set class
         type = FX_MIXER_2;
   
         // Set name
         strcpy(effect_name, "mixer_2");      
   
         input_1 = &node_input;
         input_2 = &node_input_2;
         output = &node_output;
   
         // Add additional nodes to the audio stack
         audio_node_stack[total_audio_nodes++] = &node_input_2;
           }
   
           void  print_params(void) {
           Serial.println("Audio Routing:");      
         print_audio_node_status(&node_input);
         print_audio_node_status(&node_input_2);
         print_audio_node_status(&node_output);
           }       
   };
   
   class fx_mixer_3: public fx_effect {
   
     private:
       // Additional audio nodes
       fx_audio_node node_input_2;
       fx_audio_node node_input_3;
       fx_audio_node node_dummy_output;
   
       public:   
   
       fx_audio_node * input_1;
   
       fx_audio_node * input_2;
   
       fx_audio_node * input_3;
   
       fx_audio_node * output;
   
           fx_mixer_3(void) : node_input_2(NODE_IN, "input_2", this), node_input_3(NODE_IN, "input_3", this), node_dummy_output(NODE_OUT, "dummy", this)  {
         // Set class
         type = FX_MIXER_3;
   
         // Set name
         strcpy(effect_name, "mixer_3");      
   
         input_1 = &node_input;
         input_2 = &node_input_2;
         input_3 = &node_input_3;
         output = &node_output;
   
         // Add additional nodes to the audio stack
         audio_node_stack[total_audio_nodes++] = &node_input_2;
         audio_node_stack[total_audio_nodes++] = &node_dummy_output;       // dummy output node since inputs and outputs go in pairs
         audio_node_stack[total_audio_nodes++] = &node_input_3;
           }
   
           void  print_params(void) {
           Serial.println("Audio Routing:");      
         print_audio_node_status(&node_input);
         print_audio_node_status(&node_input_2);
         print_audio_node_status(&node_input_3);
         print_audio_node_status(&node_output);
           }
   };
   
   class fx_mixer_4: public fx_effect {
   
     private:
   
       // Additional audio nodes
       fx_audio_node node_input_2;
       fx_audio_node node_input_3;
       fx_audio_node node_input_4;
       fx_audio_node node_dummy_output;
   
       public:
   
   
       fx_audio_node * input_1;
   
       fx_audio_node * input_2;
   
       fx_audio_node * input_3;
   
       fx_audio_node * input_4;
   
       fx_audio_node * output;
   
           fx_mixer_4(void) : node_input_2(NODE_IN, "input_2", this), node_input_3(NODE_IN, "input_3", this), node_input_4(NODE_IN, "input_3", this), node_dummy_output(NODE_OUT, "dummy", this){
         // Set class
         type = FX_MIXER_4;
   
         // Set name
         strcpy(effect_name, "mixer_4");      
   
         input_1 = &node_input;
         input_2 = &node_input_2;
         input_3 = &node_input_3;
         input_4 = &node_input_4;
         output = &node_output;
   
         // Add additional nodes to the audio stack
         audio_node_stack[total_audio_nodes++] = &node_input_2;
         audio_node_stack[total_audio_nodes++] = &node_dummy_output;       // dummy output node since inputs and outputs go in pairs      
         audio_node_stack[total_audio_nodes++] = &node_input_3;
         audio_node_stack[total_audio_nodes++] = &node_dummy_output;       // dummy output node since inputs and outputs go in pairs      
         audio_node_stack[total_audio_nodes++] = &node_input_4;
        }
   
           void  print_params(void) {
           Serial.println("Audio Routing:");      
         print_audio_node_status(&node_input);
         print_audio_node_status(&node_input_2);
         print_audio_node_status(&node_input_3);
         print_audio_node_status(&node_input_4);
         print_audio_node_status(&node_output);
           }   
   };
   
   
   #endif  // DM_FX_MIXERS_H
