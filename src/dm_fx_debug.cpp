// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#include "dreammakerfx.h"
#include "dm_fx_debug.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// Support for Serial debug messages
DEBUG_MSG_LEVEL dmfx_debug_level;
char debug_levels[4][6] = {"DEBUG", "INFO", "WARN", "ERROR"};

// Global debug flags accessible outside of class structure
bool dmfx_debug_mode = false;
bool dmfx_dsp_telemetry_mode = false;
bool dmfx_debug_no_reset = false;

/**
 * @brief      Displays the error status on the LEDs
 *
 * @param[in]  error_number  The error code 
 */
void  display_error_status(uint8_t error_number) {

  digitalWrite(PIN_DSP_RESET, LOW);

  for (int i=0;i<20;i++) {
    #if defined (DM_FX)
      turn_on_left_footsw_led();
      turn_off_right_footsw_led();
      delay(50);
      turn_off_left_footsw_led();;
      turn_on_right_footsw_led();;
      delay(50);
    #elif defined (DM_FX_TWO)
      digitalWrite(PIN_ARD_LED_G, HIGH);
      digitalWrite(PIN_ARD_LED_Y, LOW);
      delay(50);
      digitalWrite(PIN_ARD_LED_G, LOW);
      digitalWrite(PIN_ARD_LED_Y, HIGH);
      delay(50);    
    #endif
  }

  #if defined (DM_FX)
    turn_on_left_footsw_led();
    turn_off_right_footsw_led();;
  #elif defined (DM_FX_TWO)
    digitalWrite(PIN_ARD_LED_Y, HIGH);
    digitalWrite(PIN_ARD_LED_G, LOW);
  #endif 

  while (1) {
    for (int i=0;i<error_number;i++) {

      #if defined (DM_FX)
        turn_on_right_footsw_led();;
        delay(250);
        turn_off_right_footsw_led();
        delay(250);
      #elif defined (DM_FX_TWO)
        digitalWrite(PIN_ARD_LED_G, HIGH);
        delay(250);
        digitalWrite(PIN_ARD_LED_G, LOW);
        delay(250);
      #endif 
    }
    delay(2000);
  } 
}


// Code to drop back into bootloader
#define DBL_TAP_MAGIC 0xf01669ef // Randomly selected, adjusted to have first and last bit set
#define DBL_TAP_MAGIC_QUICK_BOOT 0xf02669ef
#define DBL_TAP_PTR ((volatile uint32_t *)(HSRAM_ADDR + HSRAM_SIZE - 4))

/**
 * @brief      Resets the Arduino back into the bootloader
 */
void reset_into_bootloader(void) {
    // reset without waiting for double tap (only works for one reset)
    *DBL_TAP_PTR = DBL_TAP_MAGIC;
    NVIC_SystemReset();
}
#endif 


