// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#include "dreammakerfx.h"
#include "dm_fx_ui.h"


#ifndef DOXYGEN_SHOULD_SKIP_THIS


/************************************************************************
 *
 *                        UI Controls
 *
 ***********************************************************************/

bool footswitch_right_pressed_event = false;
bool footswitch_left_pressed_event = false;


/**
 * @brief      Right footswitch interrupt handler
 */
void footswitch_right_pressed_isr(void) {

  static int last_press = 0;
  if (millis() < last_press + 150) {
    return;
  }
  last_press = millis();

  DEBUG_MSG("Right pressed", MSG_DEBUG);

  footswitch_right_pressed_event = true;

  if (pedal.tap_control_enabled && pedal.tap_footswitch == FOOTSWITCH_RIGHT) {
    DEBUG_MSG("Tap registered", MSG_DEBUG);
    pedal.register_tap();
  }

  

}

/**
 * @brief      Left footswitch interrupt handler
 */
void footswitch_left_pressed_isr(void){

  static int last_press = 0;
  if (millis() < last_press + 150) {
    return;
  }
  last_press = millis();

  DEBUG_MSG("Left pressed", MSG_DEBUG);

  footswitch_left_pressed_event = true;

  if (pedal.tap_control_enabled && pedal.tap_footswitch == FOOTSWITCH_LEFT) {
    DEBUG_MSG("Tap registered", MSG_DEBUG);
    pedal.register_tap();
  }
}


/**
 * @brief      Writes a value to the LP5569 RGB LED controller
 *
 * @param[in]  addr  The address
 * @param[in]  val   The value
 */
static void    lp5569_write(int addr, int val) {
  Wire2.beginTransmission(0x40);
  Wire2.write(addr);  
  Wire2.write(val); 
  Wire2.endTransmission();
}

/**
 * @brief      Reads a value from the LP5569 RGB LED controller
 *
 * @param[in]  addr  The address
 *
 * @return     Register value read
 */
static int    lp5569_read(int addr) {
  Wire2.beginTransmission(0x40);
  Wire2.write(addr);  
  Wire2.endTransmission();
  Wire2.requestFrom(0x40, 1); 
  while(Wire2.available() == 0);
  int v = Wire2.read();   
  return v;  
  
}


/**
 * @brief      Initialize the RGB LED controller
 */
void    rgb_leds_init(void) {

  #if defined (DM_FX_TWO)
    Wire2.begin();

    // Rising edge to enable 
    pinMode(RGB_LED_ENABLE, OUTPUT);
    digitalWrite(RGB_LED_ENABLE, HIGH);
    delay(10);

    // Set up charge pump and clock
    lp5569_write(0x2F, B00011001);  

    // Enable the LP5569
    lp5569_write(0, 0x40);    

  #endif 

}

#if 0
#define LED_RIGHT     0
#define LED_CENTER    1
#define LED_LEFT      2
#endif 
void    rgb_write(int led_num, int r, int g, int b) {

#if 0
  Serial.print("LED: ");
  Serial.print(led_num);
  Serial.print(" : ");
  Serial.println(r);
#endif
  switch (led_num) {
    case 0:
      lp5569_write(0x16, r);
      lp5569_write(0x17, g);
      lp5569_write(0x18, b);
      break;

    case 1:
      lp5569_write(0x19, r);
      lp5569_write(0x1A, g);
      lp5569_write(0x1B, b);
      break;

    case 2:
      lp5569_write(0x1C, r);
      lp5569_write(0x1D, g);
      lp5569_write(0x1E, b);
      break;
    default:
      break;
  }


}

static bool left_led_state = false;
static bool right_led_state = false;
static bool center_led_state = false;


/**
 * @brief      Turns on the left LED to red
 */

void    turn_on_left_footsw_led(void) {
  if (left_led_state) return;
  left_led_state = true;

  #if defined (DM_FX)
    turn_on_left_footsw_led();
  #elif defined (DM_FX_TWO)
    rgb_write(LED_LEFT, 150, 0, 0);
  #endif 
}

/**
 * @brief      Turns off the left LED
 */
void    turn_off_left_footsw_led(void) {
  if (!left_led_state) return;
  left_led_state = false;

  #if defined (DM_FX)
    turn_off_left_footsw_led();
  #elif defined (DM_FX_TWO)
    rgb_write(LED_LEFT, 0, 0, 0);
  #endif 
}

/**
 * @brief      Turns on the right LED
 */

void    turn_on_right_footsw_led(void) {
  if (right_led_state) return;
  right_led_state = true;

  #if defined (DM_FX)
    turn_on_right_footsw_led();
  #elif defined (DM_FX_TWO)
    rgb_write(LED_RIGHT, 150, 0, 0);
  #endif 
}

/**
 * @brief      Turns off the right LED
 */
void    turn_off_right_footsw_led(void) {
  if (!right_led_state) return;
  right_led_state = false;

  #if defined (DM_FX)
    turn_off_right_footsw_led();
  #elif defined (DM_FX_TWO)
    rgb_write(LED_RIGHT, 0, 0, 0);
  #endif 
}

/**
 * @brief      Turns on the center LED (if it exists)
 */
void    turn_on_center_footsw_led(void) {
  if (center_led_state) return;
  center_led_state = true;

  #if defined (DM_FX_TWO)
    rgb_write(LED_CENTER, 150, 0, 0);
  #endif 
}

/**
 * @brief      Turns off the center LED (if it exists)
 */
void    turn_off_center_footsw_led(void) {
  if (!center_led_state) return;
  center_led_state = false;

  #if defined (DM_FX_TWO)
    rgb_write(LED_CENTER, 0, 0, 0);
  #endif 
}


/**
 * @brief      Sets the RGB color of the left LED
 *
 * @param[in]  r     Red value (0-255)
 * @param[in]  g     Green value (0-255)
 * @param[in]  b     Blue value (0-255)
 */
void    turn_on_left_footsw_led_rgb(uint8_t r, uint8_t g, uint8_t b) {
  left_led_state = true;

  #if defined (DM_FX)
    turn_on_left_footsw_led();
  #elif defined (DM_FX_TWO)
    rgb_write(LED_LEFT, r, g, b);
  #endif 
}

/**
 * @brief      Sets the RGB color of the right LED
 *
 * @param[in]  r     Red value (0-255)
 * @param[in]  g     Green value (0-255)
 * @param[in]  b     Blue value (0-255)
 */
void    turn_on_right_footsw_led_rgb(uint8_t r, uint8_t g, uint8_t b) {

  right_led_state = true;

  #if defined (DM_FX)
    turn_on_right_footsw_led();
  #elif defined (DM_FX_TWO)
    rgb_write(LED_RIGHT, r, g, b);
  #endif 
}

/**
 * @brief      Sets the RGB color of the center LED
 *
 * @param[in]  r     Red value (0-255)
 * @param[in]  g     Green value (0-255)
 * @param[in]  b     Blue value (0-255)
 */
void    turn_on_center_footsw_led_rgb(uint8_t r, uint8_t g, uint8_t b) {

  center_led_state = true;
  #if defined (DM_FX_TWO)
    rgb_write(LED_CENTER, r, g, b);
  #endif 
}






void user_pb_pressed(void) __attribute__((weak));
void user_pb_pressed(void){

  DEBUG_MSG("User PB pressed", MSG_INFO);
}


#endif //  DOXYGEN_SHOULD_SKIP_THIS

