// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)
#ifndef DM_FX_H
#define DM_FX_H

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <math.h>

#include "dm_fx_dsp.h"
#include "dm_fx_spi_proto.h"
#include "dm_fx_codec.h"
#include "dm_fx_ui.h"
#include "dm_fx_debug.h"
#include "dm_fx_platform_constants.h"

#include "effects/dm_fx_effects_defines.h"


#define API_VERSION         10504


class fx_effect;
class fx_pedal;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

typedef enum {
  FOOTSWITCH_NONE,
  FOOTSWITCH_RIGHT,
  FOOTSWITCH_LEFT,
  FOOTSWITCH_BOTH,
} FOOTSWITCH;

typedef enum {
  SPI_RX_WAITING,
  SPI_RX_HEADER_1_RX,
  SPI_RX_RECEIVING,
  SPI_RX_FRAME_READY
} SPI_RX_STATE;

typedef enum {
  NODE_IN,
  NODE_OUT
} NODE_DIRECTION;

typedef struct {
  uint8_t id;
  EFFECT_TYPE type;
  void * address;
} FX_INSTANCE;

typedef struct {
  uint8_t src_id;
  uint8_t src_node_indx;
  uint8_t dest_id;
  uint8_t dest_node_indx;
} AUDIO_ROUTE;

typedef struct {
  uint8_t src_id;
  uint8_t src_node_indx;
  uint8_t src_param_id;
  uint8_t dest_id;
  uint8_t dest_node_indx;
  uint8_t dest_param_id;
  float scale;
  float offset;
  CTRL_NODE_TYPE type;
} CTRL_ROUTE;

#endif




/************************************************************************
 * NODES 
 ***********************************************************************/
#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * @brief      Class for effects audio node.
 * 
 * Audio nodes are audio inputs and outputs.  Audio nodes can exist on an 
 * effect (i.e. effect inputs and outputs).  Audio nodes can also exist on the
 * canvas (i.e. ADC and DAC channels that are part of the hardware).
 */
class fx_audio_node {
  
  friend class fx_pedal;

  protected:
    fx_effect      * parent_effect;
    fx_pedal      * parent_canvas;

  public:

    NODE_DIRECTION node_direction;    
    bool           connected;
    char           node_name[MAX_NODE_NAME];

    // Audio nodes that are part of the effect
    fx_audio_node(NODE_DIRECTION dir, const char * name, fx_effect * p) {
      node_direction = dir;
      strcpy(node_name, name);
      parent_effect = p;
      parent_canvas = NULL;
      connected = false;
    }

    // Audio nodes that are part of the canvas (i.e. ADCs, DACs)
    fx_audio_node(NODE_DIRECTION dir, const char * name, fx_pedal * p) {
      node_direction = dir;
      strcpy(node_name, name);
      parent_canvas = p;
      parent_effect = NULL;
      connected = false;
    }    
  
};

/**
 * @brief      Class for effects control node.
 * 
 * Control nodes are controller inputs and outputs.  Control nodes can exist on 
 * effects like an amplitude measurement outupt or delay lenght input.  They can
 * also exist in the canvas like a MIDI input or a POT connected to an ADC.
 * 
 * Control nodes are updated at the block level
 */
class fx_control_node {

  friend class fx_pedal;

  protected:
    fx_effect      * parent_effect;
    fx_pedal      * parent_canvas;    

  public:
    uint8_t        param_id;
    NODE_DIRECTION node_direction;
    CTRL_NODE_TYPE node_type;
    char           node_name[MAX_NODE_NAME];

    bool           connected;

    // Control nodes that are part of effects
    fx_control_node(NODE_DIRECTION dir, CTRL_NODE_TYPE type, const char * name, fx_effect * p, uint8_t ctrl_param_id) {
      param_id = ctrl_param_id;
      node_direction = dir;
      node_type = type;
      strcpy(node_name, name);
      parent_effect = p;
      parent_canvas = NULL;
      connected = false;  
    }

    // Control nodes that are part of the canvas (i.e. POTs, MIDI, etc.)
    fx_control_node(NODE_DIRECTION dir, CTRL_NODE_TYPE type, const char * name, fx_pedal * p, uint8_t ctrl_param_id) {
      param_id = ctrl_param_id;
      node_direction = dir;
      node_type = type;
      strcpy(node_name, name);
      parent_canvas = p;
      parent_effect = NULL;
      connected = false;
    }    
};

#endif  // DOXYGEN_SHOULD_SKIP_THIS

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  typedef enum {
    LED_RIGHT = 0,
    LED_CENTER = 1,
    LED_LEFT = 2,
  } LED_POS;
  #define LED_UPDATE_RATE_MS   25
#endif 



/**
 * LED colors for pedals with RGB LEDs
 */
typedef enum {
  RED     = 0x800000,
  GREEN   = 0x008000,
  BLUE    = 0x000080,
  YELLOW  = 0x808000,
  PURPLE  = 0x800080
} LED_COLOR;


/**
 * @brief      These functions are used to control the LEDs on the pedal.
 * 
 * The LEDs are part of the `pedal` object.  The LEDs available on the first
 * version of hardware are `pedal.led_left` and `pedal.led_right`.  On the second
 * generation of hardware, there is also a `pedal.led_center`.  Add the routines
 * described below to these like so:
 * 
 * To turn on the left LED:
 * ``` CPP
 *   pedal.led_left.turn_on();    // Turn on left LED
 * ```
 * 
 * To turn off the right LED;
 * ``` CPP
 *   pedal.led_right.turn_off();  // Turn off right LED
 * ```
 * 
 * To set the center LED to a purplish color:
 * ``` CPP
 *   pedal.led_center.turn_on(40, 0, 50); // Red = 40, Blue = 50
 * ```
 * 
 *   
 * Fade the right LED from red to blue over 1 second
 * ``` CPP
 *   pedal.led_right.set_rgb(RED);
 *   pedal.led_right.fade_to_rgb(BLUE, 1000.0);
 * ```
 */
class fx_led {

  private:
    LED_POS led_pos;
    float  cur_r, cur_g, cur_b;
    float  target_r, target_g, target_b;
    float  inc_r, inc_g, inc_b;
    uint32_t steps;
    uint32_t last_scan;

    void  update_rgb_led(void);

  public:

    #ifndef DOXYGEN_SHOULD_SKIP_THIS
      fx_led(LED_POS pos);
    #endif 
    void  turn_on();
    void  turn_on(uint8_t red, uint8_t green, uint8_t blue);
    void  turn_on(LED_COLOR rgb);
    void  turn_off();

    void  set_rgb(uint8_t red, uint8_t green, uint8_t blue);
    void  set_rgb(LED_COLOR rgb);
    void  fade_to_rgb(uint8_t red, uint8_t green, uint8_t blue, uint32_t milliseconds);
    void  fade_to_rgb(LED_COLOR rgb, uint32_t milliseconds);
    void  service();

};




/**
 * Toggle switch positions
 */
typedef enum {
  SWITCH_POS_UP,      /**< Toggle switch is in up position (towards back of pedal) */
  SWITCH_POS_MIDDLE,  /**< Toggle switch is in center position (in the middle) */
  SWITCH_POS_DOWN,    /**< Toggle switch is in down position (towards front of pedal) */
} SWITCH_POS;

/**
 * @brief      These functions are used to control the toggle switches on the pedal.
 * 
 * The switches, which are available on the second generation hardware, are part of the `pedal`
 * object.   The available switches are `pedal.toggle_left` and `pedal.toggle_right`.
 * 
 * ``` CPP
 * 
 * void loop() {
 *   // When the user changes the left toggle switch, change the color of the LED
 *   if (pedal.toggle_left.has_changed()) {
 *     if (pedal.toggle_left.position == SWITCH_POS_UP) {
 *       pedal.led_left.turn_on(RED);
 *     } 
 *    
 *     else if (pedal.toggle_left.position == SWITCH_POS_MIDDLE) {
*        pedal.led_left.turn_on(GREEN);
 *     }
 *
 *     else if (pedal.toggle_left.position == SWITCH_POS_DOWN) {
 *       pedal.led_left.turn_on(BLUE);
 *     }
 *   }
 *   
 *   // Other code in loop()...
 *   
 * }
 * 
 * ```
 * 
 * 
 */
class fx_switch {

  private:
    int up_pin, down_pin;
    SWITCH_POS switch_pos_last;

  public:
    SWITCH_POS  position;   /**< Current switch position (`SWITCH_POS_UP`, `SWITCH_POS_MIDDLE`, `SWITCH_POS_DOWN`) */

    /**
     * @brief      Determines if the swtich has changed
     * 
     * ``` CPP
     * if (pedal.toggle_right.has_changed()) {
     *    // Add code to read toggle position and respond
     * }
     *
     * @return     True if changed, False otherwise.
     */
    bool has_changed(void) {      
      bool res = (position==switch_pos_last?false:true);
      switch_pos_last = position;
      return res;
    }   


  #ifndef DOXYGEN_SHOULD_SKIP_THIS
    void read_switch(void) {

      switch_pos_last = position;

      bool down_pos = digitalRead(down_pin);
      bool up_pos = digitalRead(up_pin);

      if (!up_pos) {
        position = SWITCH_POS_UP;
      } else if (!down_pos) {
        position = SWITCH_POS_DOWN;
      } else {
        position = SWITCH_POS_MIDDLE;
      }

    }   

    fx_switch(int up_pin_id, int down_pin_id) {
      up_pin = up_pin_id;
      down_pin = down_pin_id;

      // Set reset pin to output
      pinMode(up_pin, INPUT);  
      pinMode(down_pin, INPUT);  

    }
  #endif 

};

/**
 * @brief      These functions are used to read the pots (aka the knobs) of the pedal.
 * 
 * Each knob has a value ranging from 0.0 (full counter-clockwise) to 1.0 (full clock-wise).
 * 
 * The first generation hardware has three pots (`pedal.pot_left`, `pedal.pot_center`, and 
 * `pedal.pot_right`). 
 * 
 * The second generation hardware has five pots (`pedal.pot_top_left`, 'pedal.pot_top_right', 
 * 'pedal.pot_bot_left`, `pedal.pot_bot_center` and `pedal.pot_bot_right`).  To preserve
 * backwards compatibility with sketches developed on the first generation hardware, the
 * `pedal.pot_left` will map to 'pedal.pot_bot_left` (and same for center and right pots).
 * 
 * Use the `.has_changed()` function to determine when a pot has been adjusted by the user.
 * 
 * ```CPP
 * if (pedal.pot_left.has_changed()) {
 *   delay_effect.set_feedback(pedal.pot_left.val);   // Set feedback of delay using left pot
 * }
 * ```
 * 
 */
class fx_pot {

  #define POT_LONG_HIST_LEN  (10)
  #define POT_SHORT_HIST_LEN (3)

  private:
    bool  first_read;
    bool  changed;
    float avg_val;
    float val_last;
    int   avg_ptr;
    int   pin_number;
    float coeff;
    float pot_history_long[POT_LONG_HIST_LEN];
    float pot_history_short[POT_SHORT_HIST_LEN];
    int   pot_long_hist_indx, pot_short_hist_indx;
    int   last_poll;

    #pragma GCC optimize ("-O3")
    #pragma GCC push_options

    float pot_mean(float * a, float pts) {
      float mean = 0;
      float r = 1.0 / pts;
      for (int i=0;i<pts;i++) {
        mean += a[i];
      }
      mean *= r;
      return mean;
    }

    float pot_variance(float * a, float pts) {
      float mean = 0;

      float r = 1.0 / pts;
      for (int i=0;i<pts;i++) {
        mean += a[i];
      }
      mean *= r;

      float v = 0.0;
      for (int i=0;i<pts;i++) {
        float x = a[i] - mean;
        v += x*x;
      }
      v *= r;

      return v;
    }    
    
    #pragma GCC pop_options

  public:

    float val;            /**< Current value of pot (0.0 to 1.0) */
    float val_inv;        /**< Current value of pot (1.0 to 0.0) */
    float val_log;        /**< Current value of pot with log curve applied (still 0.0 to 1.0) */
    float val_log_inv;    /**< Current value of pot with inverse log curve applied (still 0.0 to 1.0)  */

    /**
     * @brief      Returns true if this pot has been changed by the user
     * ```CPP
     * if (pedal.pot_left.has_changed()) {
     *   delay_effect.set_feedback(pedal.pot_left.val);   // Set feedback of delay using left pot
     * }
     * ```    
     * @return     True if changed, False otherwise.
     */
    bool has_changed(void) {
      bool return_val = changed;
      changed = false;
      return return_val;
    }

    #ifndef DOXYGEN_SHOULD_SKIP_THIS

    #pragma GCC optimize ("-O3")
    #pragma GCC push_options
    void read_pot() {
      
      changed = false;

      if (millis() < last_poll + 50) {
        return;
      }
      last_poll = millis();

      int val_int;
      if (pin_number == 0) val_int = analogRead(A0);
      else if (pin_number == 1) val_int = analogRead(A1);
      else if (pin_number == 2) val_int = analogRead(A2);
      else if (pin_number == 3) val_int = analogRead(A3);
      else if (pin_number == 4) val_int = analogRead(A4);
      else if (pin_number == 5) val_int = analogRead(A5);
      else {
        val_int = 0;
      }
      float valf = (1.0/1023.0) * (float) val_int;

      if (first_read) {
        for (int i=0;i<POT_LONG_HIST_LEN;i++) {
          pot_history_long[i] = valf;
        }
        for (int i=0;i<POT_SHORT_HIST_LEN;i++) {
          pot_history_short[i] = valf;
        }        
        changed = true;
        first_read = false;
      } else {

        pot_history_long[pot_long_hist_indx++] = valf;
        pot_history_short[pot_short_hist_indx++] = valf;

        if (pot_long_hist_indx >= POT_LONG_HIST_LEN) {
          pot_long_hist_indx = 0;
        }
        if (pot_short_hist_indx >= POT_SHORT_HIST_LEN) {
          pot_short_hist_indx = 0;
        }

        float v_long = pot_variance(pot_history_long, POT_LONG_HIST_LEN);
        
        if (v_long > 0.00005) {
          changed = true;
          float v_short= pot_variance(pot_history_short, POT_SHORT_HIST_LEN);
          if (v_short < 0.00005) {
            changed = false;
          }
        }    
      }
      val = pot_mean(pot_history_short, POT_SHORT_HIST_LEN);

      #if defined (DM_FX_TWO)
        val = 1.0 - val;
      #endif 
      val_inv = -val;
      val_log = log10(1.0 + (val*9.0));
      val_log_inv = 1.0 - log10(1.0 + ((1.0-val)*9.0));
    }
    #pragma GCC pop_options


    fx_pot(int pin) {

      pin_number = pin;
      first_read = true;
      val = 0;
      val_inv = 0;

      for (int i=0;i<POT_LONG_HIST_LEN;i++) {
        pot_history_long[i] = 0;
      }
      for (int i=0;i<POT_SHORT_HIST_LEN;i++) {
        pot_history_short[i] = 0;
      }
      pot_short_hist_indx = pot_long_hist_indx = 0;

      last_poll = millis();

    }

    #endif // DOXYGEN_SHOULD_SKIP_THIS



};


/**********************************************************************
 * CANVAS
 *********************************************************************/

/**
 * @brief      This class describes an effects canvas.
 */
class fx_pedal {

  private:

    // Has the system been initialized
    bool        initialized;
    bool        valid_audio_routes;
    bool        valid_control_routes;
    bool        debug_mode, debug_dsp_telemetry, debug_no_reset;
    
    uint32_t    last_service_ts;

    uint32_t    tap_history[16];
    uint16_t    tap_indx = 0;
    float       tap_interval_ms;
    bool        tap_locked;
    bool        tap_led_flash;
    uint32_t    tap_led_flash_cntr;
    uint32_t    tap_last_tap;
    bool        tap_new_val;

    bool        footswitch_left_pressed, footswitch_right_pressed;
    bool        footswitch_left_released, footswitch_right_released;
    bool        footswitch_left_last_state, footswitch_right_last_state;

    DSP_STATUS * status;

    char * get_effect_type(EFFECT_TYPE t);

    // All effect instances in canvas
    FX_INSTANCE instance_stack[MAX_INSTANCES];
    int         total_instances;

    // All audio routes between effects in cancas
    AUDIO_ROUTE audio_routing_stack[MAX_ROUTES];
    int         total_audio_routes;

    // All control routes between effects in cancas
    CTRL_ROUTE  control_routing_stack[MAX_ROUTES];
    int         total_control_routes;

    // Does this canvas have a valid topology
    bool        valid_canvas;   

    // Canvas audio nodes
    fx_audio_node sys_input_instr_l;
    fx_audio_node sys_input_instr_r;
    fx_audio_node sys_output_amp_l;
    fx_audio_node sys_output_amp_r;
    fx_audio_node sys_input_mic_l;
    fx_audio_node sys_input_mic_r;

    // Canvas control nodes
    fx_control_node sys_current_frequency;
    fx_control_node sys_note_duration_ms;
    fx_control_node sys_new_note;

    // Adds a new route
    bool    add_audio_route_to_stack(uint8_t src_id, uint8_t src_node_indx, uint8_t dest_id, uint8_t dest_node_indx);
    bool    add_control_route_to_stack(uint8_t src_id, 
                                            uint8_t src_node_indx, 
                                            uint8_t src_param_id, 
                                            uint8_t dest_id,                                             
                                            uint8_t dest_node_indx,
                                            uint8_t dest_param_id, 
                                            float scale,
                                            float offset,
                                            CTRL_NODE_TYPE type);
    
    // SPI protocoll support functions
    void    spi_get_status(void);
    void    spi_service(void);
    void    spi_transmit_bypass(uint16_t bypass_state);
    void    spi_transmit_all_params(void);
    void    spi_transmit_params(uint16_t node_index);
    void    spi_transmit_audio_routing_stack(void);
    void    spi_transmit_control_routing_stack(void);
    void    spi_transmit_instance_stack(void);

    // Returns the index in the node index for this effect 
    bool    get_audio_node_index(fx_audio_node * node, uint8_t * node_index);
    bool    get_control_node_index(fx_control_node * node, uint8_t * node_index);

    const char * get_name() {
        return "canvas";
    }

    
  protected:

    fx_audio_node   * audio_node_stack[4];
    fx_control_node * control_node_stack[4];

    // Services any SPI transactions that need to happen




  public:

    #ifndef DOXYGEN_SHOULD_SKIP_THIS
    friend class fx_effect;

    bool        bypass_control_enabled;
    bool        bypassed;
    FOOTSWITCH  bypass_footswitch;

    bool        tap_control_enabled;
    bool        tap_blink_only_enabled;
    FOOTSWITCH  tap_footswitch;
    #endif // DOXYGEN_SHOULD_SKIP_THIS


    #if defined (DM_FX)
      fx_pot  pot_right;
      fx_pot  pot_center;
      fx_pot  pot_left;

      fx_led  left_led;
      fx_led  right_led;

    #elif defined (DM_FX_TWO)
      fx_pot  pot_top_left;
      fx_pot  pot_top_right;
      fx_pot  pot_bot_left;
      fx_pot  pot_bot_center;
      fx_pot  pot_bot_right;
      fx_pot  exp_pedal;

      // Create aliases for backwards compatibility
      #define pot_left    pot_bot_left
      #define pot_center  pot_bot_center
      #define pot_right   pot_bot_right

      fx_switch toggle_left;
      fx_switch toggle_right;

      fx_led  led_left;
      fx_led  led_center;
      fx_led  led_right;

    #endif 

    fx_audio_node * instr_in;   // Alias
    fx_audio_node * instr_in_l;
    fx_audio_node * instr_in_r;
    fx_audio_node * amp_out;    // Alias
    fx_audio_node * amp_out_l;
    fx_audio_node * amp_out_r;
    fx_audio_node * mic_in_l;
    fx_audio_node * mic_in_r;

    fx_control_node * note_frequency;
    fx_control_node * note_duration;
    fx_control_node * new_note;

    #ifndef DOXYGEN_SHOULD_SKIP_THIS
    fx_pedal():

      #if defined (DM_FX)
        pot_right(0),
        pot_center(1),
        pot_left(2),
        led_left(LED_LEFT),
        led_right(LED_RIGHT),

      #elif defined (DM_FX_TWO)
        pot_bot_left(2),
        pot_bot_center(3),
        pot_bot_right(4),
        pot_top_left(0),
        pot_top_right(1),
        exp_pedal(5),
        toggle_left(8, 9),
        toggle_right(10, 11),
        led_left(LED_LEFT),
        led_center(LED_CENTER),
        led_right(LED_RIGHT),
      #endif 

      sys_input_instr_l(NODE_OUT, "instr_in_l", this), 
      sys_input_instr_r(NODE_OUT, "instr_in_r", this), 
      sys_output_amp_l(NODE_IN, "amp_out_l", this),
      sys_output_amp_r(NODE_IN, "amp_out_r", this),
      sys_input_mic_l(NODE_IN, "mic_in_l", this),
      sys_input_mic_r(NODE_IN, "mic_in_r", this),
      sys_current_frequency(NODE_OUT, NODE_FLOAT, "current note frequency", this, FX_CANVAS_PARAM_ID_NOTE_FREQ),
      sys_note_duration_ms(NODE_OUT, NODE_FLOAT, "current note duration (ms)", this, FX_CANVAS_PARAM_ID_NOTE_DURATION),
      sys_new_note(NODE_OUT, NODE_FLOAT, "New note playing event", this, FX_CANVAS_PARAM_ID_NOTE_NEW_NOTE) {

        // Audio routing nodes
        instr_in = &sys_input_instr_l;    // Alias
        instr_in_l = &sys_input_instr_l;
        instr_in_r = &sys_input_instr_r;
        amp_out = &sys_output_amp_l;      // Alias
        amp_out_l = &sys_output_amp_l;
        amp_out_r = &sys_output_amp_r;
        mic_in_l = &sys_input_mic_l;
        mic_in_r = &sys_input_mic_r;

        // Control node routing
        note_frequency = &sys_current_frequency;
        note_duration = &sys_note_duration_ms;
        new_note = &sys_new_note;

        // Add to node stack
        audio_node_stack[0] = instr_in_l;
        audio_node_stack[1] = amp_out_l;
        audio_node_stack[2] = instr_in_r;
        audio_node_stack[3] = amp_out_r;

        // Todo - populate 0 with canvas enable function
        control_node_stack[1] = &sys_current_frequency;
        control_node_stack[2] = &sys_note_duration_ms;
        control_node_stack[3] = &sys_new_note;        
        /*
        audio_node_stack[3] = amp_out_r;
        audio_node_stack[4] = mic_in_l;
        audio_node_stack[5] = mic_in_r;
        */

        // Add canvas as instance 0
        total_instances = 1;
        instance_stack[0].id = 0;
        instance_stack[0].type = FX_CANVAS;

        // Init the remaining slots in the instance stack
        for (int i=1;i<MAX_INSTANCES;i++) {
          instance_stack[i].id = UNDEFINED;
          instance_stack[i].address = NULL;       
          instance_stack[i].type = FX_UNDEFINED; 
        }

        // Init the slots in the audio and control routing stacks
        total_audio_routes = 0;
        total_control_routes = 0;
        for (int i=0;i<MAX_ROUTES;i++) {
          audio_routing_stack[i].src_id = UNDEFINED;
          audio_routing_stack[i].src_node_indx = UNDEFINED;       
          audio_routing_stack[i].dest_id = UNDEFINED; 
          audio_routing_stack[i].src_node_indx = UNDEFINED;       

          control_routing_stack[i].src_id = UNDEFINED;
          control_routing_stack[i].src_node_indx = UNDEFINED;       
          control_routing_stack[i].dest_id = UNDEFINED; 
          control_routing_stack[i].src_node_indx = UNDEFINED;            
        }          

        // Reset DSP by default
        debug_no_reset = false;

        // Set valid canvas to false
        valid_canvas = false;

        // Set initialized to false      
        initialized = false;

        // Set bypass control set to false
        bypass_control_enabled = false;
        bypassed = false;

        // Tap control
        tap_control_enabled = false;
        tap_blink_only_enabled = false;

        // Button press detection
        footswitch_left_last_state = true;
        footswitch_right_last_state = true;

        footswitch_left_pressed = false;
        footswitch_right_pressed = false;
        footswitch_left_released = false;
        footswitch_right_released = false;

        // Set routes valid to false 
        valid_audio_routes = false;
        valid_control_routes = false;

        status = &dsp_status;

        // Set last service timetamp
        last_service_ts = millis();

    }
    #endif    // DOXYGEN_SHOULD_SKIP_THIS

    // Pedal init functions
    void    init(void); 
    void    init(DEBUG_MSG_LEVEL debug_level);
    void    init(DEBUG_MSG_LEVEL debug_level, bool dsp_no_reset);

    // Depricated
    void    init(bool debug_enable);
    void    init(bool debug_enable, bool dsp_telem);

    // Canvas main control functions
    bool    run(void);
    void    service(void);

    // Canvas configuration

    // Route audio and control links
    bool    route_audio(fx_audio_node * out, fx_audio_node * in);
    bool    route_control(fx_control_node * src, fx_control_node * dest);
    bool    route_control(fx_control_node * src, fx_control_node * dest, float scale, float offset);

    // Attach a bypass button / LED to the effect
    void    add_bypass_button(FOOTSWITCH footswitch);
    void    add_tap_interval_button(FOOTSWITCH footswitch, bool enable_led_flash);

    // Canvas control
    void    bypass_fx(void);
    void    enable_fx(void);


    // Canvas control

    // Tap LED controls
    bool    new_tap_interval(void);
    float   get_tap_interval_ms(void);
    float   get_tap_freq_hz(void);
    void    set_tap_blink_rate_hz(float rate_hz);
    void    set_tap_blink_rate_hz(float rate_hz, FOOTSWITCH led);
    void    set_tap_blink_rate_ms(float ms);
    void    set_tap_blink_rate_ms(float ms, FOOTSWITCH led);

    // Events for when a button is pressed and released
    bool    button_pressed(FOOTSWITCH footswitch, bool enable_led);
    bool    button_released(FOOTSWITCH footswitch, bool enable_led);
  
    // Register a tap for tap reading
    #ifndef DOXYGEN_SHOULD_SKIP_THIS
      void    register_tap(void);
      void    button_press_check(void);
      void    service_button_events(void);
    #endif  // DOXYGEN_SHOULD_SKIP_THIS

    // Utility functions to print the instance and routing stack
    void    print_instance_stack(void);
    void    print_routing_table(void);
    void    print_param_tables(void);
    void    print_processor_load(int seconds);

        // Supporting functions control
    #ifndef DOXYGEN_SHOULD_SKIP_THIS
      void    spi_transmit_param(EFFECT_TYPE instance_type, uint32_t instance_id, PARAM_TYPES param_type, uint8_t param_id, void * value);


      // Parameter serice function
      void    parameter_service(void);
    #endif  // DOXYGEN_SHOULD_SKIP_THIS

};


extern fx_pedal pedal;



/**********************************************************************
 * EFFECTS
 *********************************************************************/
class fx_effect {
  
  protected:

    friend fx_pedal;


    EFFECT_TYPE     type;

    // input / output node stack (audio and control)
    fx_audio_node * audio_node_stack[MAX_NODES_PER_FX];
    int             total_audio_nodes;
    fx_control_node * control_node_stack[MAX_NODES_PER_FX];
    int             total_control_nodes;

    // Parameter stack
    void            * param_stack[MAX_PARMS_PER_FX];
    PARAM_TYPES     param_stack_types[MAX_PARMS_PER_FX];

    // total number of parameters that this effect has
    int             total_params;

    // Universal effect parameters
    bool            param_enabled;

    fx_audio_node   node_input;
    fx_audio_node   node_output;
    fx_control_node node_enabled;

    int             node_index;
    fx_pedal       * parent_canvas;
    uint8_t         instance_id;

    // Set when there are new parameters to send down to DSP
    bool            updated_parameters;


    bool get_audio_node_index(fx_audio_node * node, uint8_t * local_node_index);
    bool get_control_node_index(fx_control_node * node, uint8_t * local_node_index);

    uint16_t * serialize_params(uint16_t * serialized_params, uint16_t * size);
    bool  float_param_updated( float * param, float * param_last, float threshold );
    bool  bool_param_updated( bool * param, bool * param_last );


  public:

    friend class fx_pedal;

      // String name of current effect    
      char  effect_name[32];
      
      // Constructor
      fx_effect() : 
        node_input(NODE_IN, "input", this), 
        node_output(NODE_OUT, "output", this),
        node_enabled(NODE_IN, NODE_BOOL, "enabled", this, FX_PARAM_ID_ENABLED) {

          // Set up default audio nodes
          total_audio_nodes = 0;
          audio_node_stack[total_audio_nodes++] = &node_input;
          audio_node_stack[total_audio_nodes++] = &node_output;

          // Set up default control nodes
          total_control_nodes = 0;
          control_node_stack[total_control_nodes++] = &node_enabled;
          
          // Set up initial parameter stack
          param_enabled = true;
          param_stack[0] = &param_enabled;
          param_stack_types[0] = T_BOOL;
          total_params = 1;

          // Node index has not been assigned
          node_index = 0;

          // No parameters to update an init
          updated_parameters = false;

          // Set instance ID to 0xFF (meaning it hasn't been routed/placed yet)
          instance_id = 0xFF;

      }

      bool  service(void);
      void  print_params(void);

      const char * get_name() {
          return effect_name;
      }

      EFFECT_TYPE get_type() {
        return type;
      }

    
    /**
     * @brief      Enables this effect
     */
    void  enable(void) { param_enabled = true;  }

    /**
     * @brief      Bypasses this effect (just passes audio through)
     */
    void  bypass(void) { param_enabled = false; }

    #ifndef DOXYGEN_SHOULD_SKIP_THIS
    void print_ctrl_node_status(fx_control_node * t) {
      char buf[64];

      sprintf(buf," + [%s] %s: ", (t->node_direction==NODE_IN?"ctrl-in":"ctrl-out"), t->node_name);  Serial.print(buf);
      if (t->connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }
    }

    void print_audio_node_status(fx_audio_node * t) {
      char buf[64];
      sprintf(buf," * [%s] %s: ", (t->node_direction==NODE_IN?"audio-in":"audio-out"), t->node_name);  Serial.print(buf);
      if (t->connected) {
        Serial.println("routed");
      } else {
        Serial.println("not routed");
      }
    }

    void print_parameter( void * val, char * name, PARAM_TYPES type) {
      char buf[64];
      if (type == T_FLOAT) {
        sprintf(buf," %s: %.2f", name, *(float*) val);  Serial.println(buf);
      } else if (type == T_BOOL) {
        sprintf(buf," %s: %s", name, *(bool*) val?"true":"false");  Serial.println(buf);
      } else if (type == T_INT16) {
        sprintf(buf," %s: %u", name, *(uint16_t*) val);  Serial.println(buf);
      }
    }
    #endif 
};

#include "effects/dm_fx_effect_macros.h"

#include "effects/dm_fx_adsr_envelope.h"
#include "effects/dm_fx_allpass_filter.h"
#include "effects/dm_fx_amplitude_modulator.h"
#include "effects/dm_fx_biquad_filter.h"
#include "effects/dm_fx_destructor.h"
#include "effects/dm_fx_compressor.h"
#include "effects/dm_fx_delay.h"
#include "effects/dm_fx_delay_multitap.h"
#include "effects/dm_fx_envelope_tracker.h"
#include "effects/dm_fx_gain.h"
#include "effects/dm_fx_instrument_synth.h"
#include "effects/dm_fx_looper.h"
#include "effects/dm_fx_mixers.h"
#include "effects/dm_fx_oscillators.h"
#include "effects/dm_fx_phase_shifter.h"
#include "effects/dm_fx_pitch_shift.h"
#include "effects/dm_fx_ring_modulator.h"
#include "effects/dm_fx_slicer.h"
#include "effects/dm_fx_variable_delay.h"


#endif // DM_FX_H