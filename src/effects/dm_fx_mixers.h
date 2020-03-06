// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_MIXERS_H
#define DM_FX_MIXERS_H

/**
 * @brief      Utility: 2 channel mixer 
 */
class fx_mixer_2: public fx_effect {

  private:
    // Additional audio nodes
    fx_audio_node node_input_2;

	public:

    /**
     * Audio routing node [input]: audio input (mixer channel 1)
     */
    fx_audio_node * input_1;

    /**
     * Audio routing node [input]: audio input (mixer channel 2)
     */
    fx_audio_node * input_2;

    /**
     * Audio routing node [output]: mixer output
     */
    fx_audio_node * output;

    /**
     * @brief      Simple constructor takes no arguments
     */
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
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /**
     * @brief      Print the parameters for this effect
     */    
		void  print_params(void) {
     	Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);
      print_audio_node_status(&node_input_2);
      print_audio_node_status(&node_output);
		}		
#endif    
};

/**
 * @brief      Utility: 3 channel mixer 
 */
class fx_mixer_3: public fx_effect {

  private:
    // Additional audio nodes
    fx_audio_node node_input_2;
    fx_audio_node node_input_3;
    fx_audio_node node_dummy_output;

	public:   

  	/**
      * Audio routing node [input]: audio input (mixer channel 1)
      */
    fx_audio_node * input_1;

    /**
     * Audio routing node [input]: audio input (mixer channel 2)
     */    
    fx_audio_node * input_2;

    /**
     * Audio routing node [input]: audio input (mixer channel 3)
     */    
    fx_audio_node * input_3;

    /**
     * Audio routing node [output]: mixer output
     */    
    fx_audio_node * output;

    /**
     * @brief      Simple constructor takes no arguments
     */
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
      audio_node_stack[total_audio_nodes++] = &node_dummy_output;		// dummy output node since inputs and outputs go in pairs
      audio_node_stack[total_audio_nodes++] = &node_input_3;
		}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /**
     * @brief      Print the parameters for this effect
     */
		void  print_params(void) {
     	Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);
      print_audio_node_status(&node_input_2);
      print_audio_node_status(&node_input_3);
      print_audio_node_status(&node_output);
		}
#endif    
};

/**
 * @brief      Utility: 4 channel mixer 
 */
class fx_mixer_4: public fx_effect {

  private:

    // Additional audio nodes
    fx_audio_node node_input_2;
    fx_audio_node node_input_3;
    fx_audio_node node_input_4;
    fx_audio_node node_dummy_output;

	public:


    /**
      * Audio routing node [input]: audio input (mixer channel 1)
      */
    fx_audio_node * input_1;

    /**
      * Audio routing node [input]: audio input (mixer channel 2)
      */
    fx_audio_node * input_2;

    /**
      * Audio routing node [input]: audio input (mixer channel 3)
      */
    fx_audio_node * input_3;

    /**
      * Audio routing node [input]: audio input (mixer channel 4)
      */
    fx_audio_node * input_4;

    /**
     * Audio routing node [output]: mixer output
     */        
    fx_audio_node * output;

    /**
     * @brief      Simple constructor takes no arguments
     */
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
      audio_node_stack[total_audio_nodes++] = &node_dummy_output;		// dummy output node since inputs and outputs go in pairs      
      audio_node_stack[total_audio_nodes++] = &node_input_3;
      audio_node_stack[total_audio_nodes++] = &node_dummy_output;		// dummy output node since inputs and outputs go in pairs      
      audio_node_stack[total_audio_nodes++] = &node_input_4;
	 }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
   /**
     * @brief      Print the parameters for this effect
     */  
		void  print_params(void) {
     	Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);
      print_audio_node_status(&node_input_2);
      print_audio_node_status(&node_input_3);
      print_audio_node_status(&node_input_4);
      print_audio_node_status(&node_output);
		}	
#endif    
};


#endif 	// DM_FX_MIXERS_H