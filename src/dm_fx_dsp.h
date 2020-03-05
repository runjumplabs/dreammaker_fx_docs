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

/**
 * Current state of the DSP
 */
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


/**
 * @brief      Waits for DSP to stop accessing boot flash
 *
 * @return     False if a timeout occurs (DSP never stopped accessing flash)
 */
bool  wait_for_dsp_spi_flash_access_to_cease(void);

/**
 * @brief      Waits for the DSP to finish booting and be ready for a canvas
 */
void  wait_for_dsp_to_boot(void);

/**
 * @brief      Waits for the DSP to report the firmware version after boot
 *
 * @return     True if it was reported, false if timed out
 */
bool wait_for_dsp_firmware(void);


/**
 * @brief      Waits for DSP to complete startup routine
 */
void  wait_for_dsp_to_be_ready(void);

/**
 * @brief      Waits for the DSP to finish booting and be ready for a canvas
 */
void  wait_for_dsp_to_boot(void);

/**
 * @brief      Waits for recently downloaded canvas to start running
 *
 * @return     True if it does, false if it times out
 */
bool wait_for_canvas_to_start(void);

/**
 * @brief      Resets the DSP and waits for it to stop booting
 */
bool  dsp_reset(void);

/**
 * @brief      Reads any serial telemetry data from the DSP and displays it
 */
void display_data_from_sharc(void);

/**
 * @brief      Routes canvas errors to serial interface
 */
void report_canvas_errors(void) ;

/**
 * @brief      Flashes the DSP boot flash and then resets the DSP
 *
 * @return     True if successful, false if not
 */
bool dsp_update_firmware_image(void);

#endif  // DOXYGEN_SHOULD_SKIP_THIS
#endif 	// DM_FX_DSP_H
