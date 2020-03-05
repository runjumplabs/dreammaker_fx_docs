#ifndef DM_FX_DEBUG_H
#define DM_FX_DEBUG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// Error codes
#define ERROR_CODE_ILLEGAL_ROUTING    (2)
#define ERROR_CODE_FIRMWARE_MISMATCH  (3)
#define ERROR_CODE_DSP_NOT_BOOTING    (5)
#define ERROR_INTERNAL                (6)

#endif 

/**
 * Serial console message level (MSG_DEBUG is most verbose, SG_ERROR only reports errors)
 */
typedef enum {
  MSG_ERROR = 3,			/**< Send only error messages to the console  */
  MSG_WARN = 2,				/**< Also send warning and error messages to the console */
  MSG_INFO = 1,				/**< Also send information messages to the console in addition to warnings and errors */
  MSG_DEBUG = 0,			/**< Send internal debug messages to the console in addition to info, warnings and errors  */
} DEBUG_MSG_LEVEL;


#ifndef DOXYGEN_SHOULD_SKIP_THIS


extern char debug_levels[4][6];
#define DEBUG_MSG(msg, level)  if ((int) dmfx_debug_level <= level) { Serial.print(debug_levels[level]); Serial.print(": "); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.println(msg); } 
extern DEBUG_MSG_LEVEL dmfx_debug_level;


// Global debug flags accessible outside of class structure
extern bool dmfx_debug_mode;
extern bool dmfx_dsp_telemetry_mode;
extern bool dmfx_debug_no_reset;

/**
 * @brief      Displays the error status on the LEDs
 *
 * @param[in]  error_number  The error number
 */
void  display_error_status(uint8_t error_number);

/**
 * @brief      Resets the Arduino back into the bootloader
 */
void reset_into_bootloader(void);

#endif 

#endif 	// DM_FX_DEBUG_H