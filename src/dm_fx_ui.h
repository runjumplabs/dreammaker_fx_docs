// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#ifndef DM_FX_UI_H
#define DM_FX_UI_H

extern bool footswitch_right_pressed_event;
extern bool footswitch_left_pressed_event;

#ifdef __cplusplus
extern "C" {
#endif

void 	footswitch_right_pressed_isr(void) __attribute__((weak));
void 	footswitch_left_pressed_isr(void) __attribute__((weak));

void    rgb_leds_init(void);
void    rgb_write(int led_num, int r, int g, int b);
void    turn_on_left_footsw_led(void);
void    turn_off_left_footsw_led(void);
void    turn_on_right_footsw_led(void);
void    turn_off_right_footsw_led(void);
void    turn_on_center_footsw_led(void);
void    turn_off_center_footsw_led(void);
void    turn_on_left_footsw_led_rgb(uint8_t r, uint8_t g, uint8_t b);
void    turn_on_right_footsw_led_rgb(uint8_t r, uint8_t g, uint8_t b);
void    turn_on_center_footsw_led_rgb(uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif 	// DM_FX_UI_H