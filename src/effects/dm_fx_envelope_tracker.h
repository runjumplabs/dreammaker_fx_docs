// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_ENVELOPE_TRACKER_H
#define DM_FX_ENVELOPE_TRACKER_H

/**
 * @brief      Effect: Envelope tracker
 * 
 * Here's a nice tutorial on one effect that can be created with an envelope tracker
 * 
 * <iframe width="560" height="315" src="https://www.youtube.com/embed/gFltSCZVqx0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
 * 
 */
class fx_envelope_tracker:public fx_effect {

  private:

    // Parameters
    float param_decay_ms;
    float param_attack_ms;
    bool  param_triggered;
    ENV_TRACKER_TYPE param_type;
    float param_scale;
    float param_offset;

    // Control nodes
    fx_control_node node_ctrl_attack_ms;
    fx_control_node node_ctrl_decay_ms;
    fx_control_node node_ctrl_envelope;
    fx_control_node node_ctrl_scale;
    fx_control_node node_ctrl_offset;


    void init(void) {
      // Set class
      type = FX_ENVELOPE_TRACKER;

      // Set name
      strcpy(effect_name, "envelope tracker");


      // Initialize parameter stack
      int indx = 1;
      param_stack[indx] = &param_attack_ms;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_decay_ms;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_scale;
      param_stack_types[indx++] = T_FLOAT;
      param_stack[indx] = &param_offset;
      param_stack_types[indx++] = T_FLOAT;      
      param_stack[indx] = &param_type;
      param_stack_types[indx++] = T_INT16;
      param_stack[indx] = &param_triggered;
      param_stack_types[indx++] = T_BOOL;
      total_params = indx;

      // Initialize node stacks
      control_node_stack[total_control_nodes++] = &node_ctrl_attack_ms;
      control_node_stack[total_control_nodes++] = &node_ctrl_decay_ms;
      control_node_stack[total_control_nodes++] = &node_ctrl_scale;
      control_node_stack[total_control_nodes++] = &node_ctrl_offset;
      control_node_stack[total_control_nodes++] = &node_ctrl_envelope;

      // Assign node variables to locations
      input           = &node_input;
      attack_speed_ms = &node_ctrl_attack_ms;
      decay_speed_ms  = &node_ctrl_decay_ms;
      envelope        = &node_ctrl_envelope;
      scale           = &node_ctrl_scale;
      offset          = &node_ctrl_offset;

    }

  public:

    // Audio node names that users will be using
    fx_audio_node * input;

    // Control node names that users will be using
    
    /**
     * Control routing node: decay speed of envelope (milliseconds)
     */    
    fx_control_node * decay_speed_ms;

    /**
     * Control routing node: attack speeed of envelope (milliseconds)
     */
    fx_control_node * attack_speed_ms;

    /**
     * Control routing node: envelope signal
     */
    fx_control_node * envelope;

    /**
     * Control routing node: scale of envelope signal
     */
    fx_control_node * scale;

    /**
     * Control routing node: offset of envelope signal
     */
    fx_control_node * offset;

    /**
     * @brief      Constructs a new envelope tracker instance.
     *
     * @param[in]  attack_speed_ms  The attack speed milliseconds
     * @param[in]  decay_speed_ms   The decay speed milliseconds
     * @param[in]  triggered        Indicates if triggered (should envelope value drop down to zero when new note event is detected)
     */
    fx_envelope_tracker(float attack_speed_ms, float decay_speed_ms, bool triggered) : 
      node_ctrl_attack_ms(NODE_IN, NODE_FLOAT, "node_ctrl_attack_speed", this, FX_ENV_TRACKER_PARAM_ID_ATTACK_MS),
      node_ctrl_decay_ms(NODE_IN, NODE_FLOAT, "node_ctrl_decay_speed", this, FX_ENV_TRACKER_PARAM_ID_DECAY_MS),
      node_ctrl_scale(NODE_IN, NODE_FLOAT, "node_ctrl_scale", this, FX_ENV_TRACKER_PARAM_ID_SCALE),
      node_ctrl_offset(NODE_IN, NODE_FLOAT, "node_ctrl_offset", this, FX_ENV_TRACKER_PARAM_ID_OFFSET),
      node_ctrl_envelope(NODE_OUT, NODE_FLOAT, "node_ctrl_envelope", this, FX_ENV_TRACKER_PARAM_ID_VALUE) {
    
      param_attack_ms = attack_speed_ms;
      param_decay_ms = decay_speed_ms;
      param_triggered = triggered;
      param_scale = 1.0;
      param_offset = 0.0;

      init();
        
    }

    fx_envelope_tracker(float attack_speed_ms, float decay_speed_ms, bool triggered, float ctrl_scale, float ctrl_offset) : 
      node_ctrl_attack_ms(NODE_IN, NODE_FLOAT, "node_ctrl_attack_speed", this, FX_ENV_TRACKER_PARAM_ID_ATTACK_MS),
      node_ctrl_decay_ms(NODE_IN, NODE_FLOAT, "node_ctrl_decay_speed", this, FX_ENV_TRACKER_PARAM_ID_DECAY_MS),
      node_ctrl_scale(NODE_IN, NODE_FLOAT, "node_ctrl_scale", this, FX_ENV_TRACKER_PARAM_ID_SCALE),
      node_ctrl_offset(NODE_IN, NODE_FLOAT, "node_ctrl_offset", this, FX_ENV_TRACKER_PARAM_ID_OFFSET),
      node_ctrl_envelope(NODE_OUT, NODE_FLOAT, "node_ctrl_envelope", this, FX_ENV_TRACKER_PARAM_ID_VALUE) {
    
      param_attack_ms = attack_speed_ms;
      param_decay_ms = decay_speed_ms;
      param_triggered = triggered;
      param_scale = ctrl_scale;
      param_offset = ctrl_offset;

      init();
        
    }


    void set_attack_speed_ms(float attack_speed_ms) { 

      CHECK_LAST(attack_speed_ms, param_attack_ms);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_attack_ms.connected) {
        return; 
      }

      param_attack_ms = attack_speed_ms; 
      parent_canvas->spi_transmit_param(FX_ENVELOPE_TRACKER, instance_id, T_FLOAT, FX_ENV_TRACKER_PARAM_ID_ATTACK_MS, &param_attack_ms);
    }

    void set_decay_speed_ms(float decay_speed_ms) { 

      CHECK_LAST(decay_speed_ms, param_decay_ms);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_decay_ms.connected) {
        return; 
      }

      param_decay_ms = decay_speed_ms; 
      parent_canvas->spi_transmit_param(FX_ENVELOPE_TRACKER, instance_id, T_FLOAT, FX_ENV_TRACKER_PARAM_ID_DECAY_MS, &param_decay_ms);
    }

    /**
     * @brief      Sets the envelope scale
     *
     * @param[in]  scale  The scale value / multiplier
     */
    void set_env_scale(float scale) { 

      CHECK_LAST(scale, param_scale);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_scale.connected) {
        return; 
      }

      param_scale = scale; 
      parent_canvas->spi_transmit_param(FX_ENVELOPE_TRACKER, instance_id, T_FLOAT, FX_ENV_TRACKER_PARAM_ID_SCALE, &param_scale);
    }

    void set_env_offset(float offset) { 

      CHECK_LAST(offset, param_offset);

      // If this node is being controlled by a controller, don't allow a direct write to it
      if (node_ctrl_offset.connected) {
        return; 
      }

      param_offset = offset; 
      parent_canvas->spi_transmit_param(FX_ENVELOPE_TRACKER, instance_id, T_FLOAT, FX_ENV_TRACKER_PARAM_ID_OFFSET, &param_offset);
    }


    void  print_params(void) {

      // void print_parameter( void * val, char * name, PARAM_TYPES type)
      Serial.println("Parameters:");
      print_parameter( &param_attack_ms, "Attack speed (ms) ", T_FLOAT );
      print_parameter( &param_decay_ms, "Decay speed (ms) ", T_FLOAT );
      print_parameter( &param_scale, "Envelope scale ", T_FLOAT );
      print_parameter( &param_offset, "Envelope offset ", T_FLOAT );
      print_parameter( &param_triggered, "Triggered ", T_BOOL );

      Serial.println("Control Routing:");     
      print_ctrl_node_status(&node_ctrl_attack_ms);
      print_ctrl_node_status(&node_ctrl_decay_ms);
      print_ctrl_node_status(&node_ctrl_scale);
      print_ctrl_node_status(&node_ctrl_offset);
      print_ctrl_node_status(&node_ctrl_envelope);

      Serial.println("Audio Routing:");      
      print_audio_node_status(&node_input);

      Serial.println();
    }    


};

#endif // DM_FX_ENVELOPE_TRACKER_H