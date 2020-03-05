
.. _program_listing_file_src_dm_fx_dsp.h:

Program Listing for File dm_fx_dsp.h
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_dm_fx_dsp.h>` (``src/dm_fx_dsp.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   // Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
   // This code is licensed under MIT license (see license.txt for details)
   
   #ifndef DM_FX_DSP_H
   #define DM_FX_DSP_H
   
   
   typedef struct {
     uint16_t  index;
     float     freq;
     float     amplitude;
     float     duration_ms;
   } DSP_NOTE;
   
   typedef struct {
     bool      firmware_valid;
     uint32_t  firmware_ver;
     float     loading_percentage;
     float     amplitude;
     DSP_NOTE  notes;
     bool      new_note;
     bool      state_booted;
     bool      state_initialized;
     bool      state_lf_audio_running;
     bool      state_hf_audio_running;
     bool      state_canvas_running;
     bool      state_err_allocation;
     bool      state_err_param;
     bool      state_err_corrupt;
     bool      state_err_other;
     uint16_t  state_flags;
   } DSP_STATUS;
   
   // Global DSP status variable
   extern DSP_STATUS   dsp_status;
   
   #ifndef DOXYGEN_SHOULD_SKIP_THIS
   
   
   bool  wait_for_dsp_spi_flash_access_to_cease(void);
   
   void  wait_for_dsp_to_boot(void);
   
   bool wait_for_dsp_firmware(void);
   
   
   void  wait_for_dsp_to_be_ready(void);
   
   void  wait_for_dsp_to_boot(void);
   
   bool wait_for_canvas_to_start(void);
   
   bool  dsp_reset(void);
   
   void display_data_from_sharc(void);
   
   void report_canvas_errors(void) ;
   
   bool dsp_update_firmware_image(void);
   
   #endif  // DOXYGEN_SHOULD_SKIP_THIS
   #endif  // DM_FX_DSP_H
