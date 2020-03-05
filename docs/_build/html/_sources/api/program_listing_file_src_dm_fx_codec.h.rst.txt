
.. _program_listing_file_src_dm_fx_codec.h:

Program Listing for File dm_fx_codec.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_dm_fx_codec.h>` (``src/dm_fx_codec.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   #ifndef DM_FX_CODEC_H
   #define DM_FX_CODEC_H
   
   
   
   /************************************************************************
    *
    *                        ADAU1761 support
    *
    ***********************************************************************/
   
   #define    ADAU1761_REG_CLOCK_CONTROL             (0x4000)   /* Clock control */
   #define    ADAU1761_REG_PLL_CONTROL_0             (0x4002)   /* PLL control 0 */
   #define    ADAU1761_REG_PLL_CONTROL_1             (0x4003)   /* PLL control 1 */
   #define    ADAU1761_REG_PLL_CONTROL_2             (0x4004)   /* PLL control 2 */
   #define    ADAU1761_REG_PLL_CONTROL_3             (0x4005)   /* PLL control 3 */
   #define    ADAU1761_REG_PLL_CONTROL_4             (0x4006)   /* PLL control 4 */
   #define    ADAU1761_REG_PLL_CONTROL_5             (0x4007)   /* PLL control 5 */
   #define    ADAU1761_REG_DIG_MIC_JACK_DETECT       (0x4008)   /* Dig mic/jack detect */
   #define    ADAU1761_REG_REC_POWER_MGMT            (0x4009)   /* Rec power mgmt */
   #define    ADAU1761_REG_REC_MIXER_LEFT_0          (0x400A)   /* Rec Mixer Left 0 */
   #define    ADAU1761_REG_REC_MIXER_LEFT_1          (0x400B)   /* Rec Mixer Left 1 */
   #define    ADAU1761_REG_REC_MIXER_RIGHT_0         (0x400C)   /* Rec Mixer Right 0 */
   #define    ADAU1761_REG_REC_MIXER_RIGHT_1         (0x400D)   /* Rec Mixer Right 1 */
   #define    ADAU1761_REG_LEFT_DIFF_INPUT_VOL       (0x400E)   /* Left diff input vol */
   #define    ADAU1761_REG_RIGHT_DIFF_INPUT_VOL      (0x400F)   /* Right diff input vol */
   #define    ADAU1761_REG_RECORD_MIC_BIAS           (0x4010)   /* Record mic bias */
   #define    ADAU1761_REG_ALC_0                     (0x4011)   /* ALC 0 */
   #define    ADAU1761_REG_ALC_1                     (0x4012)   /* ALC 1 */
   #define    ADAU1761_REG_ALC_2                     (0x4013)   /* ALC 2 */
   #define    ADAU1761_REG_ALC_3                     (0x4014)   /* ALC 3 */
   #define    ADAU1761_REG_SERIAL_PORT_0             (0x4015)   /* Serial Port 0 */
   #define    ADAU1761_REG_SERIAL_PORT_1             (0x4016)   /* Serial Port 1 */
   #define    ADAU1761_REG_CONVERTER_0               (0x4017)   /* Converter 0 */
   #define    ADAU1761_REG_CONVERTER_1               (0x4018)   /* Converter 1 */
   #define    ADAU1761_REG_ADC_CONTROL               (0x4019)   /* ADC control */
   #define    ADAU1761_REG_LEFT_DIGITAL_VOL          (0x401A)   /* Left digital vol */
   #define    ADAU1761_REG_RIGHT_DIGITAL_VOL         (0x401B)   /* Right digital vol */
   #define    ADAU1761_REG_PLAY_MIXER_LEFT_0         (0x401C)   /* Play Mixer Left 0 */
   #define    ADAU1761_REG_PLAY_MIXER_LEFT_1         (0x401D)   /* Play Mixer Left 1 */
   #define    ADAU1761_REG_PLAY_MIXER_RIGHT_0        (0x401E)   /* Play Mixer Right 0 */
   #define    ADAU1761_REG_PLAY_MIXER_RIGHT_1        (0x401F)   /* Play Mixer Right 1 */
   #define    ADAU1761_REG_PLAY_L_R_MIXER_LEFT       (0x4020)   /* Play L/R mixer left */
   #define    ADAU1761_REG_PLAY_L_R_MIXER_RIGHT      (0x4021)   /* Play L/R mixer right */
   #define    ADAU1761_REG_PLAY_L_R_MIXER_MONO       (0x4022)   /* Play L/R mixer mono */
   #define    ADAU1761_REG_PLAY_HP_LEFT_VOL          (0x4023)   /* Play HP left vol */
   #define    ADAU1761_REG_PLAY_HP_RIGHT_VOL         (0x4024)   /* Play HP right vol */
   #define    ADAU1761_REG_LINE_OUTPUT_LEFT_VOL      (0x4025)   /* Line output left vol */
   #define    ADAU1761_REG_LINE_OUTPUT_RIGHT_VOL     (0x4026)   /* Line output right vol */
   #define    ADAU1761_REG_PLAY_MONO_OUTPUT          (0x4027)   /* Play mono output */
   #define    ADAU1761_REG_POP_CLICK_SUPPRESS        (0x4028)   /* Pop/click suppress */
   #define    ADAU1761_REG_PLAY_POWER_MGMT           (0x4029)   /* Play power mgmt */
   #define    ADAU1761_REG_DAC_CONTROL_0             (0x402A)   /* DAC Control 0 */
   #define    ADAU1761_REG_DAC_CONTROL_1             (0x402B)   /* DAC Control 1 */
   #define    ADAU1761_REG_DAC_CONTROL_2             (0x402C)   /* DAC Control 2 */
   #define    ADAU1761_REG_SERIAL_PORT_PAD           (0x402D)   /* Serial port pad */
   #define    ADAU1761_REG_CONTROL_PORT_PAD_0        (0x402F)   /* Control Port Pad 0 */
   #define    ADAU1761_REG_CONTROL_PORT_PAD_1        (0x4030)   /* Control Port Pad 1 */
   #define    ADAU1761_REG_JACK_DETECT_PIN           (0x4031)   /* Jack detect pin */
   #define    ADAU1761_REG_DEJITTER_CONTROL          (0x4036)   /* Dejitter control */
   #define    ADAU1761_REG_CRC_3                     (0x40C0)   /* CRC_3 */
   #define    ADAU1761_REG_CRC_2                     (0x40C1)   /* CRC_2 */
   #define    ADAU1761_REG_CRC_1                     (0x40C2)   /* CRC_1 */
   #define    ADAU1761_REG_CRC_0                     (0x40C3)   /* CRC_0 */
   #define    ADAU1761_REG_CRC_ENABLE                (0x40C4)   /* CRC enable */
   #define    ADAU1761_REG_GPIO0_PIN_CONTROL         (0x40C6)   /* GPIO0 pin control */
   #define    ADAU1761_REG_GPIO1_PIN_CONTROL         (0x40C7)   /* GPIO1 pin control */
   #define    ADAU1761_REG_GPIO2_PIN_CONTROL         (0x40C8)   /* GPIO2 pin control */
   #define    ADAU1761_REG_GPIO3_PIN_CONTROL         (0x40C9)   /* GPIO3 pin control */
   #define    ADAU1761_REG_WATCHDOG_ENABLE           (0x40D0)   /* Watchdog enable */
   #define    ADAU1761_REG_WATCHDOG_VALUE_2          (0x40D1)   /* Watchdog value 2 */
   #define    ADAU1761_REG_WATCHDOG_VALUE_1          (0x40D2)   /* Watchdog value 1 */
   #define    ADAU1761_REG_WATCHDOG_VALUE_0          (0x40D3)   /* Watchdog value 0 */
   #define    ADAU1761_REG_WATCHDOG_ERROR            (0x40D4)   /* Watchdog error */
   #define    ADAU1761_REG_DSP_SAMPLING_RATE_SETTING (0x40EB)   /* DSP sampling rate setting */
   #define    ADAU1761_REG_SERIAL_INPUT_ROUTE_CTRL   (0x40F2)   /* Serial input route ctrl */
   #define    ADAU1761_REG_SERIAL_OUTPUT_ROUTE_CTRL  (0x40F3)   /* Serial output route ctrl */
   #define    ADAU1761_REG_SERIAL_DATA_PIN_CONFIG    (0x40F4)   /* Serial data pin config */
   #define    ADAU1761_REG_DSP_ENABLE                (0x40F5)   /* DSP enable */
   #define    ADAU1761_REG_DSP_RUN                   (0x40F6)   /* DSP run */
   #define    ADAU1761_REG_DSP_SLEW_MODES            (0x40F7)   /* DSP slew modes */
   #define    ADAU1761_REG_SERIAL_PORT_SAMPLING_RATE (0x40F8)   /* Serial port sampling rate */
   #define    ADAU1761_REG_CLOCK_ENABLE_0            (0x40F9)   /* Clock Enable 0 */
   #define    ADAU1761_REG_CLOCK_ENABLE_1            (0x40FA)   /* Clock Enable 1 */
   
   void adau1761_initialize(void);
   
   void  adau1761_write_register(uint16_t addr, uint8_t val);
   
   
   
   
   
   /************************************************************************
    *
    *                        WM8731 support
    *
    ***********************************************************************/
   
   #define WM8731_R0_LEFT_LINE_CTRL     (0x00)
   #define WM8731_R1_RIGHT_LINE_CTRL    (0x02)
   #define WM8731_R2_LEFT_HP_CTRL       (0x04)
   #define WM8731_R3_RIGHT_HP_CTRL      (0x06)
   #define WM8731_R4_ANALOG_PATH_CTRL   (0x08)
   #define WM8731_R5_DIGITAL_PATH_CTRL  (0x0A)
   #define WM8731_R6_PWR_DOWN_CTRL      (0x0C)
   #define WM8731_R7_DIGITAL_IFACE_CTRL (0x0E)
   #define WM8731_R8_SAMPLING_CTRL      (0x10)
   #define WM8731_R9_ACTIVE_CTRL        (0x12)
   #define WM8731_RF_RESET_CTRL         (0x1E)
   
   
   void  wm8731_write_register(uint8_t addr, uint8_t val);
   
   void  wm8731_initialize(void);
   
   
   
   
   #endif      // DM_FX_CODEC_H
   #endif      // DOXYGEN_SHOULD_SKIP_THIS
