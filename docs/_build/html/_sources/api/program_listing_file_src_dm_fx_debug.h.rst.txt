
.. _program_listing_file_src_dm_fx_debug.h:

Program Listing for File dm_fx_debug.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_dm_fx_debug.h>` (``src/dm_fx_debug.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef DM_FX_DEBUG_H
   #define DM_FX_DEBUG_H
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   // Error codes
   #define ERROR_CODE_ILLEGAL_ROUTING    (2)
   #define ERROR_CODE_FIRMWARE_MISMATCH  (3)
   #define ERROR_CODE_DSP_NOT_BOOTING    (5)
   #define ERROR_INTERNAL                (6)
   
   #endif 
   
   typedef enum {
     MSG_ERROR = 3,            
     MSG_WARN = 2,             
     MSG_INFO = 1,             
     MSG_DEBUG = 0,            
   } DEBUG_MSG_LEVEL;
   
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   
   extern char debug_levels[4][6];
   #define DEBUG_MSG(msg, level)  if ((int) dmfx_debug_level <= level) { Serial.print(debug_levels[level]); Serial.print(": "); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.println(msg); } 
   extern DEBUG_MSG_LEVEL dmfx_debug_level;
   
   
   // Global debug flags accessible outside of class structure
   extern bool dmfx_debug_mode;
   extern bool dmfx_dsp_telemetry_mode;
   extern bool dmfx_debug_no_reset;
   
   void  display_error_status(uint8_t error_number);
   
   void reset_into_bootloader(void);
   
   #endif 
   
   #endif  // DM_FX_DEBUG_H
