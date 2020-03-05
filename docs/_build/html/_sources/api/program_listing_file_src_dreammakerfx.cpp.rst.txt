
.. _program_listing_file_src_dreammakerfx.cpp:

Program Listing for File dreammakerfx.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_dreammakerfx.cpp>` (``src/dreammakerfx.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #include <stdlib.h>
   
   #include "Arduino.h"
   #include "dreammakerfx.h"
   
   
   fx_pedal pedal;
   
   /************************************************************************
    *  Initialization
    ***********************************************************************/
   
   void fx_pedal::init(void) {
     init(MSG_WARN, false);
   }
   
   
   void fx_pedal::init(DEBUG_MSG_LEVEL debug_level) {
     init(debug_level, false);
   }
   
   
   [[deprecated("Replaced by init(DEBUG_MSG_LEVEL debug_level), which has an improved interface")]]
   void fx_pedal::init(bool debug_enable) {
     if (debug_enable) {
       init(MSG_INFO);    
     } else {
       init();
     }
   }
   
   [[deprecated("Replaced by init(DEBUG_MSG_LEVEL debug_level), which has an improved interface")]]
   void fx_pedal::init(bool debug_enable, bool dsp_telem) {
     if (dsp_telem) {
       init(MSG_DEBUG);    
     } else {
   
     }
   }
   
   void fx_pedal::init(DEBUG_MSG_LEVEL debug_level, bool dsp_no_reset) {
   
     dmfx_debug_level = debug_level;
     dmfx_debug_no_reset = debug_no_reset = dsp_no_reset;
   
     #if defined (DM_FX)
       // Setup LEDs
       pinMode(PIN_FOOTSW_LED_1, OUTPUT);
       pinMode(PIN_FOOTSW_LED_2, OUTPUT);
       pinMode(PIN_ARD_LED, OUTPUT);
       pinMode(PIN_USR_PB, INPUT);
       attachInterrupt(digitalPinToInterrupt(PIN_USR_PB), user_pb_pressed, FALLING);
   
     #elif defined (DM_FX_TWO)
       pinMode(PIN_ARD_LED_G, OUTPUT);
       pinMode(PIN_ARD_LED_Y, OUTPUT);
       digitalWrite(PIN_ARD_LED_G, HIGH);
       digitalWrite(PIN_ARD_LED_Y, LOW);
     #endif 
   
     // Initialize hardware pins
     pinMode(PIN_FOOTSW_1, INPUT);
     pinMode(PIN_FOOTSW_2, INPUT);
   
     bool left = digitalRead(PIN_FOOTSW_LEFT);
     bool right = digitalRead(PIN_FOOTSW_RIGHT);
   
     // if both buttons held, reboot into bootloader
     if (!left && ! right) {
       for (int i=0;i<10;i++) {
         turn_on_left_footsw_led();
         turn_off_right_footsw_led();;
         delay(100);
         turn_off_left_footsw_led();;
         turn_on_right_footsw_led();;
         delay(100);
       }
       reset_into_bootloader();
     }
   
     // Initialize the RGB LEDs if present
     rgb_leds_init();
     turn_on_left_footsw_led_rgb(0, 0, 200);
     turn_on_center_footsw_led_rgb(0, 0, 200);
     turn_on_right_footsw_led_rgb(0, 0, 200);
   
     int now = millis();
     bool timeout = false;
     while ( !Serial && !timeout) {
       if (millis() > now + 2000) {
         timeout = true;
       }
     }
   
     #if defined (DM_FX)
       // Initialize the WM8731
       wm8731_initialize();
     #elif defined (DM_FX_TWO)
       // Initialize SigmaDSP
       adau1761_initialize();
     #endif 
   
     turn_on_left_footsw_led_rgb(100, 0, 100);
     turn_on_center_footsw_led_rgb(100, 0, 100);
     turn_on_right_footsw_led_rgb(100, 0, 100);
   
     // Set up telemetry link to SHARC
     Serial1.begin(115200);
     // Set up Serial USB link
     Serial.begin(115200);
   
     attachInterrupt(digitalPinToInterrupt(PIN_FOOTSW_1), footswitch_right_pressed_isr, FALLING);
     attachInterrupt(digitalPinToInterrupt(PIN_FOOTSW_2), footswitch_left_pressed_isr, FALLING);
   
   
     if (!debug_no_reset) {
       // Reset the DSP
       dsp_reset();
     } else {
       // If we're not resetting DSP, at least make sure it's not still using the SPI port to boot
       wait_for_dsp_spi_flash_access_to_cease();
     }
     
     turn_on_left_footsw_led_rgb(100, 100, 0);
     turn_on_center_footsw_led_rgb(100, 100, 0);
     turn_on_right_footsw_led_rgb(100, 100, 0);
   
     
     Serial.print("DreamMaker FX By Run Jump Labs");
   
     Serial.print(" (version: ");
     String package_str = String(API_VERSION);
     package_str.replace("0",".");
     Serial.print(package_str);    
     Serial.println(")");
   
     // Wait for the DSP to boot and report firmware
     wait_for_dsp_to_boot();
   
     turn_on_left_footsw_led_rgb(100, 100, 100);
     turn_on_center_footsw_led_rgb(100, 100, 100);
     turn_on_right_footsw_led_rgb(100, 100, 100);
   
     bool firmware_match = true;
     if (dsp_status.firmware_ver != API_VERSION) {
       firmware_match = false;
     }
     if (!firmware_match) {
       Serial.println(" The Arduino package version does not match the DSP firmware version, updating firmware...");
       dsp_update_firmware_image();
       dsp_reset();
       wait_for_dsp_spi_flash_access_to_cease();
       wait_for_dsp_to_boot();
     }
   
     // Wait for the DSP to be ready
     wait_for_dsp_to_be_ready();
   
     turn_off_left_footsw_led();
     turn_off_center_footsw_led();
     turn_off_right_footsw_led();
   
     initialized = true;
   
   }
   
   
   
   
   /******************************************************************************
    *  DSP SPI Protocol
    *****************************************************************************/
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   void  fx_pedal::spi_transmit_control_routing_stack(void) {
   
     DEBUG_MSG("Starting", MSG_DEBUG);
   
     uint16_t * routing_block = (uint16_t *) malloc(total_control_routes*8*sizeof(uint16_t));
     if (routing_block == NULL) {
         DEBUG_MSG("Could not allocate space for routing block", MSG_ERROR);
         display_error_status(ERROR_INTERNAL);    
     }
   
     // serialize routing data
     int indx = 0;
     routing_block[indx++] = HEADER_CONTROL_ROUTING_BLOCK;
     for (int i=0;i<total_control_routes;i++) {
       routing_block[indx++] = (control_routing_stack[i].src_id << 8) | control_routing_stack[i].src_node_indx;
       routing_block[indx++] = (control_routing_stack[i].dest_id << 8) | control_routing_stack[i].dest_node_indx;
       routing_block[indx++] = control_routing_stack[i].src_param_id; 
       routing_block[indx++] = control_routing_stack[i].dest_param_id; 
       float scale = control_routing_stack[i].scale;
       float offset = control_routing_stack[i].offset;
       uint32_t part_32 = * (uint32_t *) &scale;
       routing_block[indx++] = (uint16_t) (part_32 >> 16);
       routing_block[indx++] = (uint16_t) (part_32 & 0xFFFF);
       part_32 = * (uint32_t *) &offset;
       routing_block[indx++] = (uint16_t) (part_32 >> 16);
       routing_block[indx++] = (uint16_t) (part_32 & 0xFFFF);
       routing_block[indx++] = (uint16_t) control_routing_stack[i].type;
   
     }
   
     // Copy to SPI transmit fifo
     spi_fifo_insert_block(routing_block, indx);
   
     free (routing_block);
   
     DEBUG_MSG("Complete", MSG_DEBUG);
   
   }
   
   void  fx_pedal::spi_transmit_audio_routing_stack(void) {
   
     DEBUG_MSG("Starting", MSG_DEBUG);
   
     uint16_t * routing_block = (uint16_t *) malloc(total_audio_routes*2*sizeof(uint16_t));
     if (routing_block == NULL) {
         DEBUG_MSG("Could not allocate space for routing block", MSG_ERROR);
         display_error_status(ERROR_INTERNAL);    
     }
    
     // serialize routing data
     int indx = 0;
     routing_block[indx++] = HEADER_AUDIO_ROUTING_BLOCK;
     for (int i=0;i<total_audio_routes;i++) {
       routing_block[indx++] = (audio_routing_stack[i].src_id << 8) | audio_routing_stack[i].src_node_indx;
       routing_block[indx++] = (audio_routing_stack[i].dest_id << 8) | audio_routing_stack[i].dest_node_indx;
     }
   
     // Copy to SPI transmit fifo
     spi_fifo_insert_block(routing_block, indx);
   
     DEBUG_MSG("Complete", MSG_DEBUG);
   
   
   }
   
   
   void fx_pedal::spi_transmit_instance_stack(void) {
   
     DEBUG_MSG("Starting", MSG_DEBUG);
   
     uint16_t * spi_instance_block = (uint16_t *) malloc((total_instances + 1)*sizeof(uint16_t));
     if (spi_instance_block == NULL) {
         DEBUG_MSG("Could not allocate space for routing block", MSG_ERROR);
         display_error_status(ERROR_INTERNAL);    
     }
   
     // serialize instance data
     int indx = 0;
     spi_instance_block[indx++] = HEADER_INSTANCE_BLOCK;
     for (int i=0;i<total_instances;i++) {
       spi_instance_block[indx++] = (instance_stack[i].type << 8) | instance_stack[i].id; 
     }
   
     // Copy to SPI transmit fifo
     spi_fifo_insert_block(spi_instance_block, indx);
   
     free(spi_instance_block);
   
     DEBUG_MSG("Complete", MSG_DEBUG);
   
   }
   
   void fx_pedal::spi_transmit_param(EFFECT_TYPE instance_type, uint32_t instance_id, PARAM_TYPES param_type, uint8_t param_id, void * value) {
     
     DEBUG_MSG("Starting", MSG_DEBUG);  
   
     static uint16_t param_block[7] = {HEADER_SINGLE_PARAMETER};
   
     uint16_t part_16;
     uint32_t part_32;
   
     param_block[1] = (uint16_t) instance_type;
     param_block[2] = (uint16_t) instance_id;
     param_block[3] = (uint16_t) param_type;
     param_block[4] = param_id;
   
     if (param_type == T_BOOL) {
       part_16 = * (uint8_t *) value;
       param_block[5] = part_16;
     }
     else if (param_type == T_INT16) {
       param_block[5] = (uint16_t) (* (uint16_t *) value);
     }
     else if (param_type == T_INT32) {
       param_block[5] = (uint16_t)((* (uint32_t *) value) >> 16);
       param_block[6] = (uint16_t)((* (uint32_t *) value) & 0xFFFF);
     }
     else if (param_type == T_FLOAT) {
       part_32 = * (uint32_t *) value;
       param_block[5] = (uint16_t) (part_32 >> 16);
       param_block[6] = (uint16_t) (part_32 & 0xFFFF);
     }     
   
     // Add to transmit FIFO
     spi_fifo_insert_block(param_block, sizeof(param_block)/sizeof(uint16_t));
   
     DEBUG_MSG("Complete", MSG_DEBUG);  
   
   }
   
   void fx_pedal::spi_transmit_bypass(uint16_t bypass_state) {
   
     DEBUG_MSG("Starting", MSG_DEBUG);  
     uint16_t param_block[2];
   
     param_block[0] = HEADER_SET_BYPASS;
     param_block[1] = bypass_state; 
    
     // Copy to SPI transmit fifo
     spi_fifo_insert_block(param_block, 2);
   
     DEBUG_MSG("Complete", MSG_DEBUG);  
   
   }
   
   void fx_pedal::spi_get_status(void) {
   
     uint16_t param_block[1];
     param_block[0] = HEADER_GET_STATUS;
   
     spi_fifo_insert_block(param_block, SPI_DSP_STAT_FRAME_SIZE);
     
   }
   
   void fx_pedal::spi_transmit_params(uint16_t node_index) {
   
     DEBUG_MSG("Starting", MSG_DEBUG);  
     
     uint16_t param_block[64];
     uint16_t size;
   
     if (!node_index) {
       DEBUG_MSG("This instance is not part of a canvas", MSG_ERROR);  
       display_error_status(ERROR_CODE_ILLEGAL_ROUTING);   
     }
   
     fx_effect * effect = (fx_effect *) instance_stack[node_index].address;
     
     if (effect == NULL) {
       DEBUG_MSG("NULL effect encountered", MSG_ERROR);  
       display_error_status(ERROR_CODE_ILLEGAL_ROUTING);   
     }
   
     #if 0
       Serial.println("Generating parameter transmit block");
     #endif 
   
     size = 0;
     param_block[0] = HEADER_PARAMETER_BLOCK;
     param_block[1] = (uint16_t) instance_stack[node_index].type;  // instance type
     param_block[2] = (uint16_t) instance_stack[node_index].id;    // instance id
     effect->serialize_params(&param_block[3], &size);
     size += 3;
   
     // Copy to SPI transmit fifo
     spi_fifo_insert_block(param_block, size);
   
     DEBUG_MSG("Complete", MSG_DEBUG);  
   
   }
   
   void fx_pedal::spi_transmit_all_params(void) {
   
     DEBUG_MSG("Starting", MSG_DEBUG);  
   
     uint16_t param_block[64];
     uint16_t size;
   
     param_block[0] = HEADER_PARAMETER_BLOCK;
   
     for (int i=1;i<total_instances;i++) {
       size = 0;
   
       fx_effect * effect = (fx_effect *) instance_stack[i].address;
       
       if (effect == NULL) {
         DEBUG_MSG("NULL effect encountered", MSG_ERROR);  
         display_error_status(ERROR_CODE_ILLEGAL_ROUTING);  
       } else {
         param_block[1] = (uint16_t) instance_stack[i].type;  // instance type
         param_block[2] = (uint16_t) instance_stack[i].id;    // instance id
         effect->serialize_params(&param_block[3], &size);
         size += 3;
   
         // Copy to SPI transmit block
         spi_fifo_insert_block(param_block, size);
   
       }
     }
   
     DEBUG_MSG("Complete", MSG_DEBUG);  
   }
   
   
   
   
   void  fx_pedal::spi_service(void) {
     spi_transmit_buffered_frames(false);
   }
   
   #endif // DOXYGEN_SHOULD_SKIP_THIS
   
   
   
   
   
   
   
   
   /******************************************************************************
    *  Audio and control routing
    *****************************************************************************/
   
   
   bool  fx_pedal::add_audio_route_to_stack(uint8_t src_id, 
                                             uint8_t src_node_indx, 
                                             uint8_t dest_id, 
                                             uint8_t dest_node_indx) {
     audio_routing_stack[total_audio_routes].src_id = src_id;
     audio_routing_stack[total_audio_routes].src_node_indx = src_node_indx;
     audio_routing_stack[total_audio_routes].dest_id = dest_id;
     audio_routing_stack[total_audio_routes].dest_node_indx = dest_node_indx;
     total_audio_routes++;
   
     return true;
   }
   
   bool  fx_pedal::add_control_route_to_stack(uint8_t src_id, 
                                               uint8_t src_node_indx, 
                                               uint8_t src_param_id, 
                                               uint8_t dest_id,                                             
                                               uint8_t dest_node_indx,
                                               uint8_t dest_param_id, 
                                               float scale,
                                               float offset,
                                               CTRL_NODE_TYPE type) {
   
     control_routing_stack[total_control_routes].src_id = src_id;
     control_routing_stack[total_control_routes].src_node_indx = src_node_indx;
     control_routing_stack[total_control_routes].src_param_id = src_param_id;
     control_routing_stack[total_control_routes].dest_id = dest_id;
     control_routing_stack[total_control_routes].dest_node_indx = dest_node_indx;
     control_routing_stack[total_control_routes].dest_param_id = dest_param_id;
     control_routing_stack[total_control_routes].scale = scale;
     control_routing_stack[total_control_routes].offset = offset;
     control_routing_stack[total_control_routes].type = type;
     
     total_control_routes++;
   
     return true;  
   }
   
   
   bool fx_pedal::route_audio(fx_audio_node * src, fx_audio_node * dest) {
   
     bool local_debug = false;
     // Ensure inputs and outputs are valid
     if (src->node_direction != NODE_OUT || dest->node_direction != NODE_IN) {
       DEBUG_MSG("Source node is not an output, or destination node is not an input", MSG_ERROR);
       return false;
     }
   
     uint8_t src_id, dest_id;
   
     // Set to false in case we bail mid-way through due to error
     valid_audio_routes = false;
   
     // Check to see if inputs and outputs are in our stack, and add if not
     if (src->parent_effect != NULL) {
       bool found = false;
       for (int i=0;i<total_instances;i++) {
         if ((void *) src->parent_effect == instance_stack[i].address) {
           found = true;
           src_id = i;
         }
       }
       if (!found) {
         instance_stack[total_instances].address = src->parent_effect; 
         instance_stack[total_instances].type = src->parent_effect->get_type(); 
         instance_stack[total_instances].id = total_instances; 
         src_id = total_instances;      
   
         #if 0
           Serial.print("route_audio(): Adding new (source) instance type: ");
           Serial.print(instance_stack[total_instances].type);
           Serial.print(", address: ");
           Serial.println((uint32_t) instance_stack[total_instances].address, HEX);
         #endif
   
   
         // Set instance ID in this effect instance too
         src->parent_effect->instance_id = total_instances;
   
         total_instances++;
   
       }
     } else if (src->parent_canvas != NULL) {
       src_id = 0;
     }
   
     if (dest->parent_effect != NULL) {
       bool found = false;
       for (int i=0;i<total_instances;i++) {
         if ((void *) dest->parent_effect == instance_stack[i].address) {
           found = true;
           dest_id = i;
         }
       }
       if (!found) {
         instance_stack[total_instances].address = dest->parent_effect; 
         instance_stack[total_instances].type = dest->parent_effect->get_type(); 
         instance_stack[total_instances].id = total_instances; 
         dest_id = total_instances;
   
         #if 0
           Serial.print("route_audio(): Adding new (dest) instance type: ");
           Serial.print(instance_stack[total_instances].type);
           Serial.print(", address: ");
           Serial.println((uint32_t) instance_stack[total_instances].address, HEX);
         #endif
   
         // Set instance ID in this effect instance too
         dest->parent_effect->instance_id = total_instances;
   
         total_instances++;
       }
     } else if (dest->parent_canvas != NULL) {
       dest_id = 0;
     }
   
     // Add routes to our routing table
     uint8_t src_node_indx, dest_node_indx;
     bool res;
   
     // Look up source node
     if (src->parent_effect != NULL) {
       res = src->parent_effect->get_audio_node_index(src, &src_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find this source node in the effect!", MSG_ERROR);
         return false;
       }
     }
     else if (src->parent_canvas != NULL) {
       res = src->parent_canvas->get_audio_node_index(src, &src_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find this source node in the canvas!", MSG_ERROR);
         return false;
       }
     }
   
     // Look up destination node
     if (dest->parent_effect != NULL) {
       res = dest->parent_effect->get_audio_node_index(dest, &dest_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find this destination node in the effect!", MSG_ERROR);
         return false;
       }
     }
     else if (dest->parent_canvas != NULL) {
       res = dest->parent_canvas->get_audio_node_index(dest, &dest_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find this destination node in the canvas!", MSG_ERROR);
         return false;
       }
     }
   
     // Set both nodes to connected
     src->connected = true;
     dest->connected = true;
   
     // Add route to routing table
     add_audio_route_to_stack(src_id, src_node_indx, dest_id, dest_node_indx);
   
     // Check routing table for any outputs that are being written to twice
     for (int i=0;i<total_audio_routes;i++) {
       for (int j=0;j<total_audio_routes;j++) {
         if (i != j) {
           if ((audio_routing_stack[i].dest_id == audio_routing_stack[j].dest_id) && 
               (audio_routing_stack[i].dest_node_indx == audio_routing_stack[j].dest_node_indx)) {
             DEBUG_MSG("Two different effects writing to same audio node", MSG_ERROR);
             return false;
           }
         }
       }
     }
   
     // Happy days, we made it
     valid_audio_routes = true;
     return true;
   }
   
   
   bool fx_pedal::route_control(fx_control_node * src, fx_control_node * dest) {
     route_control(src, dest, 1.0, 0.0);
   }
   
   bool fx_pedal::route_control(fx_control_node * src, fx_control_node * dest, float scale, float offset) {
   
     // Set to false in case we bail mid-way through due to error
     valid_control_routes = false;
   
   // Ensure inputs and outputs are valid
     if (src->node_direction != NODE_OUT || dest->node_direction != NODE_IN) {
       DEBUG_MSG("Source must be output, dest must be input", MSG_ERROR);
       return false;
     }
   
     uint8_t src_id, dest_id;
   
     // Check to see if inputs and outputs are in our stack, and add if not
     if (src->parent_effect != NULL) {
       bool found = false;
       for (int i=0;i<total_instances;i++) {
         if ((void *) src->parent_effect == instance_stack[i].address) {
           found = true;
           src_id = i;
         }
       }
       if (!found) {
         instance_stack[total_instances].address = src->parent_effect; 
         instance_stack[total_instances].type = src->parent_effect->get_type(); 
         instance_stack[total_instances].id = total_instances; 
         src_id = total_instances;
         total_instances++;
   
         #if 0
           Serial.println(src->parent_effect->get_name());
         #endif
       }
     } else if (src->parent_canvas != NULL) {
       src_id = 0;
     }
   
     if (dest->parent_effect != NULL) {
       bool found = false;
       for (int i=0;i<total_instances;i++) {
         if ((void *) dest->parent_effect == instance_stack[i].address) {
           found = true;
           dest_id = i;
         }
       }
       if (!found) {
         instance_stack[total_instances].address = dest->parent_effect; 
         instance_stack[total_instances].type = dest->parent_effect->get_type(); 
         instance_stack[total_instances].id = total_instances; 
         dest_id = total_instances;
         total_instances++;
       }
     } else if (src->parent_canvas != NULL) {
       dest_id = 0;
     }
   
     // Make sure controllers are compatible
     if (dest->node_type != src->node_type) {
       DEBUG_MSG("Trying to connect incompatible controls", MSG_ERROR);
       return false;
     }
   
     // Add routes to our routing table
     uint8_t src_node_indx, dest_node_indx;
     bool res;
   
     // Look up source node
     if (src->parent_effect != NULL) {
       res = src->parent_effect->get_control_node_index(src, &src_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find the source node in an effect", MSG_ERROR);
         return false;
       }
     }
     else if (src->parent_canvas != NULL) {
       if (src->parent_canvas)
       res = src->parent_canvas->get_control_node_index(src, &src_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find the source node in the canvas", MSG_ERROR);
         return false;
       }
     }
   
     // Look up destination node
     if (dest->parent_effect != NULL) {
       res = dest->parent_effect->get_control_node_index(dest, &dest_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find the dest node in an effect", MSG_ERROR);
         return false;
       }
     }
     else if (dest->parent_canvas != NULL) {
       if (dest->parent_canvas)
       res = dest->parent_canvas->get_control_node_index(dest, &dest_node_indx);
       if (!res) {
         DEBUG_MSG("Couldn't find the dest node in the canvas", MSG_ERROR);
         return false;
       }
     }
   
     // Set both nodes to connected
     src->connected = true;
     dest->connected = true;
   
     // Add route to routing table
     add_control_route_to_stack(src_id, src_node_indx, src->param_id, dest_id, dest_node_indx, dest->param_id, scale, offset, dest->node_type);
   
     // Check routing table for any outputs that are being written to twice
   
     for (int i=0;i<total_control_routes;i++) {
       for (int j=0;j<total_control_routes;j++) {
         if (i != j) {
           if ((control_routing_stack[i].dest_id == control_routing_stack[j].dest_id) && 
               (control_routing_stack[i].dest_node_indx == control_routing_stack[j].dest_node_indx)) {
             DEBUG_MSG("Two different effects writing to same control node", MSG_ERROR);
             return false;
           }
         }
       }
     }
   
     // Happy days, we made it
     valid_control_routes = true;
     return true;
   
   }
   
   
   void fx_pedal::add_bypass_button(FOOTSWITCH footswitch) {
     bypass_control_enabled = true;
     pedal.bypassed = true;
     bypass_footswitch = footswitch;
   }
   
   void fx_pedal::add_tap_interval_button(FOOTSWITCH footswitch, bool enable_led_flash) {
     tap_control_enabled = true;
     if (bypass_footswitch == footswitch) {
       DEBUG_MSG("Attempting to add tap interval to footswitch already used for bypass", MSG_ERROR);
       return;
     }
   
     tap_led_flash = enable_led_flash;
     tap_footswitch = footswitch;
     tap_indx = 0;
     tap_last_tap = millis();
   
   }
   
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   bool  fx_pedal::get_audio_node_index(fx_audio_node * node, uint8_t * node_index) {
   
     for (int i=0;i<MAX_NODES_PER_FX;i++) {
   
       if (node == audio_node_stack[i]) {
         *node_index = i;
         return true;
       }
     }
     return false;
   }
   
   
   bool  fx_pedal::get_control_node_index(fx_control_node * node, uint8_t * node_index) {
   
     for (int i=0;i<MAX_NODES_PER_FX;i++) {
   
       if (node == control_node_stack[i]) {
         *node_index = i;
         return true;
       }
     }
     return false;
   }
   
   #endif  // DOXYGEN_SHOULD_SKIP_THIS
   
   
   
   
   
   
   
   /******************************************************************************
    *  Canvas control functions 
    *****************************************************************************/
   
   
   void fx_pedal::service(void) {
   
     if (tap_control_enabled) {
       if (tap_footswitch == FOOTSWITCH_LEFT) {
         if (millis() < tap_led_flash_cntr + 50) {
           turn_on_left_footsw_led();
         } else {
           turn_off_left_footsw_led();
         }
       }
       else if (tap_footswitch == FOOTSWITCH_RIGHT) {
         if (millis() < tap_led_flash_cntr + 50) {
           turn_on_right_footsw_led();
         } else {
           turn_off_right_footsw_led();
         }
       }
     }
   
     if ((tap_blink_only_enabled || tap_control_enabled) && tap_locked) {
       if (millis() > tap_led_flash_cntr + tap_interval_ms) {
         tap_led_flash_cntr = millis();
         if (tap_footswitch == FOOTSWITCH_LEFT) {
           turn_on_left_footsw_led();
         } else if (tap_footswitch == FOOTSWITCH_RIGHT) {
           turn_on_right_footsw_led();
         }
       } else if (millis() > tap_led_flash_cntr + 200) {
         if (tap_footswitch == FOOTSWITCH_LEFT) {
           turn_off_left_footsw_led();
         } else if (tap_footswitch == FOOTSWITCH_RIGHT) {
           turn_off_right_footsw_led();
         }
       }
     }
   
     // Service LEDs
     led_left.service();
     #if defined (DM_FX_TWO)
       led_center.service();
     #endif
     led_right.service();
   
     button_press_check();
     service_button_events();
   
     // Read in telemetry data from the DSP
     display_data_from_sharc();
   
     // Run the remainder of service loop ~30 times / second
     if (millis() < last_service_ts + 33) return;
     last_service_ts = millis();
   
     // Read the pots and switches
     #if defined (DM_FX)
       pot_right.read_pot();
       pot_center.read_pot();
       pot_left.read_pot();
   
     #elif defined (DM_FX_TWO)
       pot_top_left.read_pot();
       pot_top_right.read_pot();
       pot_bot_left.read_pot();
       pot_bot_center.read_pot();
       pot_bot_right.read_pot();
       exp_pedal.read_pot();
       toggle_left.read_switch();
       toggle_right.read_switch();
   
     #endif 
   
     // Get status data from the DSP
     spi_get_status();
   
     // Service any parameter updates
     spi_service();
   
   }
   
   void    fx_pedal::bypass_fx(void) {
     DEBUG_MSG("Bypass", MSG_DEBUG);
     spi_transmit_bypass((uint16_t) 1);
   }
   
   
   void    fx_pedal::enable_fx(void) {
     DEBUG_MSG("Enable", MSG_DEBUG);
     spi_transmit_bypass((uint16_t) 0);
   }
   
   
   bool fx_pedal::run(void) {
   
     
     bool ready = true;
   
     // Check to see if our routing is valid
     if (total_audio_routes == 0) {
       DEBUG_MSG("No routes defined", MSG_ERROR);
       display_error_status(ERROR_CODE_ILLEGAL_ROUTING);
     } else if (!valid_audio_routes) {
       DEBUG_MSG("Errors in the audio routing.  Fix errors in your route_audio() calls.", MSG_ERROR);
       display_error_status(ERROR_CODE_ILLEGAL_ROUTING);
     } else if (total_control_routes > 0 && !valid_control_routes) {
       DEBUG_MSG("Errors in the control routing.  Fix errors in your route_control() calls.", MSG_ERROR);
       display_error_status(ERROR_CODE_ILLEGAL_ROUTING);
     }  
   
     if (ready) {
       display_data_from_sharc();
   
       // Send routing stack to DSP
       spi_transmit_audio_routing_stack();
       display_data_from_sharc();
   
       spi_transmit_control_routing_stack();
       display_data_from_sharc();
   
       // Send instance stack to DSP
       spi_transmit_instance_stack();
       display_data_from_sharc();
   
       // Send parameters to DSP
       spi_transmit_all_params();
       display_data_from_sharc();
   
       // Wait for DSP to send message that canvas is running
       wait_for_canvas_to_start();
       int now = millis();
       while (millis() < now + 50) {
         display_data_from_sharc();
       }
   
       // If we've added bypass controls, start effect bypassed
       if (bypass_control_enabled) {
         pedal.bypassed = true;
         bypass_fx();
       } else {
         pedal.bypassed = false;
         enable_fx();
       }
   
       if (dsp_status.state_canvas_running) {
         DEBUG_MSG("Canvas is running", MSG_INFO);
       } else {
         report_canvas_errors();
         ready = false;
       } 
     }
   
     // Display error code on LEDs
     if (!ready) {
       display_error_status(ERROR_CODE_ILLEGAL_ROUTING);
     }
   
     return ready;
   
   }
   
   
   
   
   /******************************************************************************
    *  Parameter control functions 
    *****************************************************************************/
   
   
   
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   void fx_pedal::register_tap(void) {
   
     uint32_t interval = millis() - tap_last_tap;
     tap_last_tap = millis();
     tap_led_flash_cntr = millis();
   
     #if 0
       Serial.print("Debug: interval:");
       Serial.println(interval);
     #endif 
   
     if (tap_indx == 0 || interval > 2000) {
       
       tap_indx = 1;      
       tap_locked = false;
       tap_new_val = false;
   
       for (int i=0;i<15;i++) {
         tap_history[i] = 0;
       }
   
       return;
     } else if (tap_indx < 2) {
       tap_history[tap_indx++] = interval;
       tap_locked = false;
     } else {
       tap_history[tap_indx++] = interval;
       if (tap_indx >= 16) {
         tap_indx = 15;
       }
       float sum = 0.0;
       for (int i=1; i<tap_indx; i++) {
         sum += (float) tap_history[i];
       }
       sum *= 1.0/((float) (tap_indx-1));
       tap_locked = true;
       tap_interval_ms = sum;
       tap_new_val = true;
       #if 0
         Serial.print("Debug: tap ms:");
         Serial.println(tap_interval_ms);
       #endif 
     }
   }
   
   void fx_pedal::button_press_check(void) {
     static int time_since_last_press = 0;
     if (millis() < time_since_last_press + 75) {
       return;
     }
     time_since_last_press = millis();
   
     bool left = !digitalRead(PIN_FOOTSW_LEFT);
     bool right = !digitalRead(PIN_FOOTSW_RIGHT);
   
     if (left && left != footswitch_left_last_state) {
       footswitch_left_pressed = true;  
     } else if (!left && left != footswitch_left_last_state) {
       footswitch_left_released = true;  
     }
   
     if (right && right != footswitch_right_last_state) {
       footswitch_right_pressed = true;  
     } else if (!right && right != footswitch_right_last_state) {
       footswitch_right_released = true; 
     }
   
     footswitch_left_last_state = left;
     footswitch_right_last_state = right;
   }
   
   void  fx_pedal::service_button_events(void) {
   
     if (footswitch_right_pressed_event) {
       footswitch_right_pressed_event = false;
   
       if (bypass_control_enabled && bypass_footswitch == FOOTSWITCH_RIGHT) {
   
           DEBUG_MSG("Toggle bypass", MSG_DEBUG);
   
           if (bypassed) {
             led_right.turn_on();
             enable_fx();
           } else {
             led_right.turn_off();
             bypass_fx();
           }
           bypassed = !bypassed;
         }
     }
   
     if (footswitch_left_pressed_event) {
       footswitch_left_pressed_event = false;
       if (bypass_control_enabled && bypass_footswitch == FOOTSWITCH_LEFT) {
   
         DEBUG_MSG("Toggle bypass", MSG_DEBUG);
   
         if (bypassed) {
           //Serial.println("Turning on left LED");
           led_left.turn_on();
           //turn_on_left_footsw_led();
           enable_fx();
         } else {
           //Serial.println("Turning off left LED");
           led_left.turn_off();
           //turn_off_left_footsw_led();
           bypass_fx();
         }
         bypassed = !bypassed;
       }
     }
   }
   
   #endif // DOXYGEN_SHOULD_SKIP_THIS
   
   
   
   bool fx_pedal::new_tap_interval(void) {
     if (tap_new_val) {
       tap_new_val = false;
       return true;
     } else {
       return false;
     }
   }
   
   
   float fx_pedal::get_tap_interval_ms(void) {
     if (tap_locked) {
       return tap_interval_ms;
     } else {
       return 1000.0;
     }
   }
   
   float fx_pedal::get_tap_freq_hz(void) {
     if (tap_locked) {
       return 1.0/(0.001 * tap_interval_ms);
     } else {
       return 1.0;
     }
   }
   
   
   void fx_pedal::set_tap_blink_rate_hz(float rate_hz) {
     if (rate_hz < 0) {
       rate_hz = 0;
     } else if (rate_hz > 100) {
       rate_hz = 100.0;
     }
     tap_interval_ms = (1000.0/rate_hz);
     tap_locked = true;
     tap_control_enabled = true;
   }
   
   void fx_pedal::set_tap_blink_rate_hz(float rate_hz, FOOTSWITCH led) {
     if (rate_hz < 0) {
       rate_hz = 0;
     } else if (rate_hz > 100) {
       rate_hz = 100.0;
     }
     tap_interval_ms = (1000.0/rate_hz);
     tap_locked = true;
     tap_blink_only_enabled = true;
     tap_footswitch = led;
   
   } 
   
   
   void fx_pedal::set_tap_blink_rate_ms(float ms) {
     if (ms < 0) {
       ms = 10;
     } else if (ms > 10000) {
       ms = 10000.0;
     }
     tap_interval_ms = ms;
     tap_locked = true;
     tap_control_enabled = true;
   }
   
   void fx_pedal::set_tap_blink_rate_ms(float ms, FOOTSWITCH led) {
     if (ms < 0) {
       ms = 10;
     } else if (ms > 10000) {
       ms = 10000.0;
     }
     tap_interval_ms = ms;
     tap_locked = true;
     tap_blink_only_enabled = true;
     tap_footswitch = led;
   }    
   
   
   
   bool fx_pedal::button_pressed(FOOTSWITCH footswitch, bool enable_led) {
     bool result = false;
   
     if (footswitch == FOOTSWITCH_BOTH && footswitch_left_pressed && footswitch_right_pressed) {
       footswitch_left_pressed = false;
       footswitch_left_released = false;
       footswitch_right_pressed = false;
       footswitch_right_released = false;
       result = true;
     } else if (footswitch == FOOTSWITCH_LEFT && footswitch_left_pressed) {
       footswitch_left_pressed = false;
       footswitch_left_released = false;
       if (enable_led) {
         led_left.turn_on();
       }
       result = true;
     } else if (footswitch == FOOTSWITCH_RIGHT && footswitch_right_pressed) {
       footswitch_right_pressed = false;
       footswitch_right_released = false;
       if (enable_led) {
         led_right.turn_on();
       }
       result = true;
     }
     return result;
   }
   
   bool fx_pedal::button_released(FOOTSWITCH footswitch, bool enable_led) {
     bool result = false;
     if (footswitch == FOOTSWITCH_LEFT && footswitch_left_released) {
       footswitch_left_pressed = false;
       footswitch_left_released = false;
       if (enable_led) {
         led_left.turn_off();
         //turn_off_left_footsw_led();
       }
       result = true;
     } else if (footswitch == FOOTSWITCH_RIGHT && footswitch_right_released) {
       footswitch_right_pressed = false;
       footswitch_right_released = false;
       if (enable_led) {
         led_right.turn_off();
         //turn_off_right_footsw_led();
       }
       result = true;
     }
     return result;
   }
   
   
   
   
   
   
   
   
   
   
   void print_processor_load(int seconds) {
     if (seconds < 1) {
       seconds = 1;
     }
     static int now = 0;
     if (millis() > now + seconds*1000) {
       Serial.print("Processor load: ");
       Serial.print(dsp_status.loading_percentage);
       Serial.println("%");
       now = millis();
     }
   }
   
   
   
   
   
   void fx_pedal::print_instance_stack() {
   
     char buf[64];
   
     Serial.println();
     Serial.println("Instance stack:");
   
     sprintf(buf," Total instances: %d", (int) total_instances); Serial.println(buf);
     for (int i=0;i<total_instances;i++) {
       if (instance_stack[i].type != FX_UNDEFINED) {
   
         sprintf(buf," ID: %#04x", (int) instance_stack[i].id); Serial.println(buf);
   
         Serial.print("  Type: ");
         Serial.print(get_effect_type(instance_stack[i].type));
         Serial.print(" (");
         Serial.print(instance_stack[i].type);
         Serial.println(")");
         
         sprintf(buf,"  Address: %#04x", (int) instance_stack[i].address); Serial.println(buf);
   
       } else {
         Serial.println("Undefined instance found");
       }
     }
     Serial.println();
   }
   
   void fx_pedal::print_routing_table() {
     
     char buf[64];
   
     Serial.println();
     Serial.println("Audio routing table:");
   
     if (total_audio_routes > 0) {
       for (int i=0;i<total_audio_routes;i++) {
         if (audio_routing_stack[i].src_id != UNDEFINED) {
           Serial.print(" Src ID: ");
           Serial.println((int) audio_routing_stack[i].src_id, HEX);
           Serial.print("  Src Node Indx: ");
           Serial.println((int) audio_routing_stack[i].src_node_indx, HEX);
           Serial.print(" Dest ID: ");
           Serial.println((int) audio_routing_stack[i].dest_id, HEX);
           Serial.print("  Dest Node Indx: ");
           Serial.println((int) audio_routing_stack[i].dest_node_indx, HEX);
           Serial.println();
         }
       }
     } else {
       Serial.println(" No audio routes in canvas");
     }
   
     Serial.println("Control routing table:");
   
     if (total_control_routes > 0) {
       for (int i=0;i<total_control_routes;i++) {
         if (audio_routing_stack[i].src_id != UNDEFINED) {
           Serial.print(" Src ID: ");
           Serial.println((int) control_routing_stack[i].src_id, HEX);
           Serial.print("  Src Node Indx: ");
           Serial.println((int) control_routing_stack[i].src_node_indx, HEX);
           Serial.print(" Dest ID: ");
           Serial.println((int) control_routing_stack[i].dest_id, HEX);
           Serial.print("  Dest Node Indx: ");
           Serial.println((int) control_routing_stack[i].dest_node_indx, HEX);
           Serial.println();
         }
       }
     } else {
       Serial.println(" No control routes in canvas");
     }
   
   
   }
   
   void fx_pedal::print_param_tables() {
     char buf[64];
   
     Serial.println();
     Serial.println("Parameter tables:");
   
     if (total_instances <= 1) {
       Serial.println(" There are no effect instances in this canvas so there are no parameters");
       return;
     }
   
     for (int i=1;i<total_instances;i++) {
       if (instance_stack[i].address == NULL) {
         sprintf(buf, "Null pointer encoundered for instance %d", i); Serial.println(buf);
       } else {
   
         if (instance_stack[i].type == FX_ALLPASS_FILTER) {
           sprintf(buf, "Allpass filter (instance %d)", i); Serial.println(buf);
           fx_allpass_filter * effect = (fx_allpass_filter *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_ADSR_ENVELOPE) {
           sprintf(buf, "ADSR Envelope (instance %d)", i); Serial.println(buf);
           fx_adsr_envelope * effect = (fx_adsr_envelope *) instance_stack[i].address; 
           effect->print_params();
         }
   
   
         if (instance_stack[i].type == FX_AMPLITUDE_MODULATOR) {
           sprintf(buf, "AMP MODULATOR (instance %d)", i); Serial.println(buf);
           fx_amplitude_mod * effect = (fx_amplitude_mod *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_BIQUAD_FILTER) {
           sprintf(buf, "BIQUAD (instance %d)", i); Serial.println(buf);
           fx_biquad_filter * effect = (fx_biquad_filter *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_DESTRUCTOR) {
           sprintf(buf, "Destructor (instance %d)", i); Serial.println(buf);
           fx_destructor * effect = (fx_destructor *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_COMPRESSOR) {
           sprintf(buf, "Compressor (instance %d)", i); Serial.println(buf);
           fx_compressor * effect = (fx_compressor *) instance_stack[i].address; 
           effect->print_params();
         }
   
   
         if (instance_stack[i].type == FX_DELAY) {
           sprintf(buf, "DELAY (instance %d)", i); Serial.println(buf);
           fx_delay * effect = (fx_delay *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_DELAY_MULTITAP) {
           sprintf(buf, "MULTITAP DELAY (instance %d)", i); Serial.println(buf);
           fx_multitap_delay * effect = (fx_multitap_delay *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_ENVELOPE_TRACKER) {
           sprintf(buf, "ENVELOPE TRACKER (instance %d)", i); Serial.println(buf);
           fx_envelope_tracker * effect = (fx_envelope_tracker *) instance_stack[i].address; 
           effect->print_params();
         }
   
   
         if (instance_stack[i].type == FX_GAIN) {
           sprintf(buf, "GAIN (instance %d)", i); Serial.println(buf);
           fx_gain * effect = (fx_gain *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_MIXER_2) {
           sprintf(buf, "MIXER x 2 (instance %d)", i); Serial.println(buf);
           fx_mixer_2 * effect = (fx_mixer_2 *) instance_stack[i].address; 
           effect->print_params();
         }      
     
         if (instance_stack[i].type == FX_MIXER_3) {
           sprintf(buf, "MIXER x 3 (instance %d)", i); Serial.println(buf);
           fx_mixer_3 * effect = (fx_mixer_3 *) instance_stack[i].address; 
           effect->print_params();
         }      
   
         if (instance_stack[i].type == FX_MIXER_4) {
           sprintf(buf, "MIXER x 4 (instance %d)", i); Serial.println(buf);
           fx_mixer_4 * effect = (fx_mixer_4 *) instance_stack[i].address; 
           effect->print_params();
         }      
   
         if (instance_stack[i].type == FX_PHASE_SHIFTER) {
           sprintf(buf, "PHASE SHIFTER (instance %d)", i); Serial.println(buf);
           fx_phase_shifter * effect = (fx_phase_shifter *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_PITCH_SHIFT) {
           sprintf(buf, "PITCH SHIFT (instance %d)", i); Serial.println(buf);
           fx_pitch_shift * effect = (fx_pitch_shift *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_RING_MOD) {
           sprintf(buf, "RING MODULATOR (instance %d)", i); Serial.println(buf);
           fx_ring_mod * effect = (fx_ring_mod *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_SLICER) {
           sprintf(buf, "SLICER (instance %d)", i); Serial.println(buf);
           fx_slicer * effect = (fx_slicer *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_LOOPER) {
           sprintf(buf, "LOOPER (instance %d)", i); Serial.println(buf);
           fx_looper * effect = (fx_looper *) instance_stack[i].address; 
           effect->print_params();
         }
   
         if (instance_stack[i].type == FX_INSTRUMENT_SYNTH) {
           sprintf(buf, "INSTRUMENT SYNTH (instance %d)", i); Serial.println(buf);
           fx_instrument_synth * effect = (fx_instrument_synth *) instance_stack[i].address; 
           effect->print_params();
         }      
   
         if (instance_stack[i].type == FX_OSCILLATOR) {
           sprintf(buf, "OSCILLATOR (instance %d)", i); Serial.println(buf);
           fx_oscillator * effect = (fx_oscillator *) instance_stack[i].address; 
           effect->print_params();
         }
   
   
         if (instance_stack[i].type == FX_VARIABLE_DELAY) {
           sprintf(buf, "VARIABLE DELAY (instance %d)", i); Serial.println(buf);
           fx_variable_delay * effect = (fx_variable_delay *) instance_stack[i].address; 
           effect->print_params();
         }
   
         else {
           fx_effect * effect = (fx_effect *) instance_stack[i].address;
           effect->print_params();
         }
       }
     }
   }
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   char * fx_pedal::get_effect_type(EFFECT_TYPE t) {
     if (t == FX_NONE) return "none";
     else if (t == FX_AMPLITUDE_MODULATOR) return "amplitude modulator";
     else if (t == FX_BIQUAD_FILTER) return "biquad filter";
     else if (t == FX_DESTRUCTOR) return "destructor";
     else if (t == FX_COMPRESSOR) return "compressor";
     else if (t == FX_DELAY) return "delay";
     else if (t == FX_ENVELOPE_TRACKER) return "envelope tracker";
     else if (t == FX_GAIN) return "gain";  
     else if (t == FX_INSTRUMENT_SYNTH) return "instrument synth";
     else if (t == FX_MIXER_2) return "mixer x 2";  
     else if (t == FX_MIXER_3) return "mixer x 3";  
     else if (t == FX_MIXER_4) return "mixer x 4";  
     else if (t == FX_RING_MOD) return "ring modulator";
     else if (t == FX_LOOPER) return "looper";
     else if (t == FX_OSCILLATOR) return "oscillator";
     else if (t == FX_PHASE_SHIFTER) return "phase shifter";
     else if (t == FX_PITCH_SHIFT) return "pitch shift";
     else if (t == FX_SLICER) return "slicer";
     else if (t == FX_VARIABLE_DELAY) return "variable delay";
     else if (t == FX_UNDEFINED) return "undefined";
     else if (t == FX_CANVAS) return "canvas";
   }
   
   #endif 
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   /************************************************************************
    *
    *                        EFFECTS FUNCTIONS
    *
    ***********************************************************************/
   
   
   bool fx_effect::get_audio_node_index(fx_audio_node * node, uint8_t * local_node_index) {
   
     for (int i=0;i<MAX_NODES_PER_FX;i++) {
   
       if (node == audio_node_stack[i]) {
         *local_node_index = i;
         
         // save in instance
         node_index = i;
         return true;
       }
     }
     return false;
   }
   
   bool fx_effect::get_control_node_index(fx_control_node * node, uint8_t * local_node_index) {
     
   
     for (int i=0;i<MAX_NODES_PER_FX;i++) {
     
       if (node == control_node_stack[i]) {
         *local_node_index = i;
         
         // save in instance
         node_index = i;
         return true;
       }
     }
     return false;
   }
   
   
   uint16_t * fx_effect::serialize_params(uint16_t * serialized_params, uint16_t * size) {
   
     char buf[64];
   
   #if 0
     sprintf(buf,"Serializing parameters for : %s", effect_name); Serial.println(buf);
   #endif 
     // serialize instance data
     int indx = 0;
   
     uint32_t part_32;
     uint16_t part_16;
     uint8_t part_8;
     
     for (int i=0;i<total_params;i++) {
      
       if (param_stack_types[i] == T_BOOL) {
        // Serial.println(" : bool");
         part_16 = * (uint8_t *) param_stack[i];
         serialized_params[indx++] = part_16;
       }
       else if (param_stack_types[i] == T_INT16) {
         //Serial.println(" : int16");
         //Serial.println((int)param_stack[i],HEX);
         serialized_params[indx++] = (uint16_t) (* (uint16_t *) param_stack[i]);
       }
       else if (param_stack_types[i] == T_INT32) {
         //Serial.println(" : int32");
         part_32 = * (uint32_t *) param_stack[i];
         serialized_params[indx++] = (uint16_t)((* (uint32_t *) param_stack[i]) >> 16);
         serialized_params[indx++] = (uint16_t)((* (uint32_t *) param_stack[i]) & 0xFFFF);
       }
       else if (param_stack_types[i] == T_FLOAT) {
         //Serial.println(" : float");
         part_32 = * (uint32_t *) param_stack[i];
         serialized_params[indx++] = (uint16_t) (part_32 >> 16);
         serialized_params[indx++] = (uint16_t) (part_32 & 0xFFFF);
       }   
   
       //sprintf(buf,"  %#08x - %d", param_stack[i], (int) param_stack_types[i]); Serial.println(buf);
   
     }
     *size = indx;
     #if 0
       Serial.println("  ------");
   
       for (int i=0;i<indx;i++) {
         //sprintf(buf,"  %#04x, [%#08x] -> %#04x - %d", serialized_params[i], param_stack[i], * (uint16_t *) param_stack[i], (int) param_stack_types[i]); Serial.println(buf);
         sprintf(buf,"  %#04x", serialized_params[i]); Serial.println(buf);
       }
       Serial.println("Complete");
       //sprintf(buf, "Bool: %d" , (int) sizeof(bool)); Serial.println(buf);
     #endif 
   }
   
   bool  fx_effect::float_param_updated( float * param, float * param_last, float threshold ) {
     
     bool different;
     if (abs(*param - *param_last) > threshold) {
       different = true;
     } else {
       different = false;
     }
   
     *param_last = *param;
   
     return different;
   
   }
   
   bool  fx_effect::bool_param_updated( bool * param, bool * param_last ) {
     bool different = false;
     if (*param != *param_last) {
       different = true;
     }
     *param_last = *param;
     return different;
   }    
   
   
   void  fx_effect::print_params(void) {
     Serial.println(" No print function declared for this effect");
   }
   
   bool  fx_effect::service(void) {
     Serial.println(" No service function declared for this effect");  
     return false;
   }
   
   
   
   
   
   
   /************************************************************************
    *
    *                        LED FUNCTIONS
    *
    ***********************************************************************/
   
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   fx_led::fx_led(LED_POS pos) {
     led_pos = pos;
     last_scan = millis();
   }
   
   
   void  fx_led::update_rgb_led(void) {
     rgb_write((int) led_pos, (uint8_t) cur_r, (uint8_t) cur_g, (uint8_t) cur_b);
   }
   
   void  fx_led::service() {
     if (millis() >= last_scan + LED_UPDATE_RATE_MS) {
       last_scan = millis();
   
       if (steps) {
   
         cur_r += inc_r;
         cur_g += inc_g;
         cur_b += inc_b;
   
         update_rgb_led();
   
         steps--;
       }
     }
   }   
   #endif // DOXYGEN_SHOULD_SKIP_THIS
   
   
   
   void  fx_led::turn_on() {
   
     switch(led_pos) {
       case LED_RIGHT: 
         turn_on_right_footsw_led(); break;
       case LED_CENTER: 
         turn_on_center_footsw_led(); break;
       case LED_LEFT: 
         turn_on_left_footsw_led(); break;
       default: break;
     }
   }
   
   void  fx_led::turn_on(uint8_t red, uint8_t green, uint8_t blue) {
     set_rgb(red, green, blue);
   }
   
   void  fx_led::turn_on(LED_COLOR rgb) {
     set_rgb((uint32_t) rgb);
   }
   
   
   void  fx_led::turn_off() {
     switch(led_pos) {
       case LED_RIGHT: turn_off_right_footsw_led(); break;
       case LED_CENTER: turn_off_center_footsw_led(); break;
       case LED_LEFT: turn_off_left_footsw_led(); break;
       default: break;
     }
   }
   
   void  fx_led::set_rgb(uint8_t red, uint8_t green, uint8_t blue) {
     #if defined (DM_FX)
       turn_on();
     #elif defined (DM_FX_TWO)
       cur_r = red;
       cur_g = green;
       cur_b = blue;
       steps = 0;
       update_rgb_led();      
     #endif       
   }
   
   void  fx_led::set_rgb(LED_COLOR rgb) {
   
     #if defined (DM_FX_TWO)
       float red = ((uint32_t) rgb >> 16) & 0xFF;
       float green = ((uint32_t) rgb >> 8) & 0xFF;
       float blue = (uint32_t) rgb & 0xFF;
       set_rgb(red, green, blue);      
     #endif 
   }
   
   void  fx_led::fade_to_rgb(uint8_t red, uint8_t green, uint8_t blue, uint32_t milliseconds) {
     #if defined (DM_FX_TWO)
       target_r = red;
       target_g = green;
       target_b = blue;
   
       steps = (uint32_t) ((float) milliseconds * (1.0/LED_UPDATE_RATE_MS));
       float inc = 1.0/(float) steps;
   
       inc_r = (target_r - cur_r) * inc;
       inc_g = (target_g - cur_g) * inc;
       inc_b = (target_b - cur_b) * inc;
   
       update_rgb_led();
     #endif 
   
   }
   
   void  fx_led::fade_to_rgb(LED_COLOR rgb, uint32_t milliseconds) {
     float red = (rgb >> 16) & 0xFF;
     float green = (rgb >> 8) & 0xFF;
     float blue = rgb & 0xFF;
     fade_to_rgb(red, green, blue, milliseconds);
   
   }
   
   
   
