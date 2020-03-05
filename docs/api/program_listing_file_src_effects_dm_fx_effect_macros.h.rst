
.. _program_listing_file_src_effects_dm_fx_effect_macros.h:

Program Listing for File dm_fx_effect_macros.h
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_effects_dm_fx_effect_macros.h>` (``src/effects/dm_fx_effect_macros.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef DM_FX_EFFECT_MACROS
   #define DM_FX_EFFECT_MACROS
   
   #define CHECK_LAST_RUN(FUNC, NAME) static uint32_t FUNC ## NAME ## _last = 0;  if (millis() < FUNC ## NAME ## _last + 30) { return; } FUNC ## NAME ## _last = millis();
   #define CHECK_LAST_ENABLED() if (param_enabled) { return; } 
   #define CHECK_LAST_DISABLED() if (!param_enabled) { return; } 
   #define CHECK_LAST(VALUE, PARAM_NAME) if (VALUE == PARAM_NAME) { return; }
   
   #endif  // DM_FX_EFFECT_MACROS
