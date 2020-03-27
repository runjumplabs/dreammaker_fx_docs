// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_IMPULSE_RESPONSE_H
#define DM_FX_IMPULSE_RESPONSE_H

/**
 * @brief      Effect: Impulse Resonse - applies an impulse response to the signal
 * 
 * Applies an impulse preset to the audio.  
 *     `IR_SPRING_LONG` - modeled spring with broader response
 *     `IR_SPRING_MED` - modeled medium spring with narrower response
 *     `IR_HF_SPRING` - captured impulse from spring 
 *     `IR_PLATE_1` - broad dispersion from a plate impulse
 *   
 * 
 */
class fx_impulse_response: public fx_effect {
  private:
    IMPULSE param_impulse;
    // Parameters
  
    void init(void) {
      // Set class
      type = FX_IMPULSE_RESPONSE;

      // Set name
      strcpy(effect_name, "impulse response");

      // Assign programmable node names
      input = &node_input;
      output = &node_output;

      // Initialize parameter stack
      PARAM_STACK(param_impulse,    T_INT16);

    }    

  public:

   /**
     * Audio routing node: primary audio input
     */
    fx_audio_node * input;

    /**
     * Audio routing node: primary audio output
     */
    fx_audio_node * output;

    /**
     * @brief      Constructor for impulse response
     *
     * @param[in]  type  The type of impulse (see above)
     */
    fx_impulse_response(IMPULSE type) {
      param_impulse = type;
      init();
    }

    /**
     * @brief  Prints the parameters for the delay effect
     */
    void  print_params(void) {

      // void print_parameter( void * val, char * name, PARAM_TYPES type)
      Serial.println();
    }
};

#endif	// DM_FX_IMPULSE_RESPONSE_H