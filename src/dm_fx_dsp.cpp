// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#include <SPI.h>

#include "dreammakerfx.h"
#include "dm_fx_dsp.h"


#ifndef DOXYGEN_SHOULD_SKIP_THIS


/**
 * @brief      Waits for DSP to stop accessing boot flash
 *
 * @return     False if a timeout occurs (DSP never stopped accessing flash)
 */
bool  wait_for_dsp_spi_flash_access_to_cease(void) {
  
  // Wait for SPI flash select line to stop strobing
  pinMode(PIN_DSP_SPI_FLASH_SELECT, INPUT); 
  uint32_t start = millis();
  uint32_t now = millis();
  uint32_t cntr = 0;
  uint32_t iters = 0;
  while (cntr < 250 && millis() < start + 3000) {
    if (digitalRead(PIN_DSP_SPI_FLASH_SELECT) == LOW) {
      iters++;
      now = millis();
    }
    cntr = millis() - now;
  }
  //Serial.println(iters);
  if (millis() > start + 3000) {
    DEBUG_MSG("DSP boot timeout", MSG_ERROR);
    return false;
  }

  return true;

}


/**
 * @brief      Resets the DSP and waits for it to stop booting
 */
bool  dsp_reset(void) {

  // Set reset pin to output
  pinMode(PIN_DSP_RESET, OUTPUT);  
  digitalWrite(PIN_DSP_RESET, HIGH);

  // Flush existing data from SPI fifo
  dsp_status.firmware_valid = false;
  dsp_status.firmware_ver = 0;
  spi_fifo_reset();

  // Turn off SPI so there is no contention on the boot process
  spi_stop();

  // Strobe reset
  delay(50);
  digitalWrite(PIN_DSP_RESET, LOW);
  delay(100);
  digitalWrite(PIN_DSP_RESET, HIGH);
  delay(20);


  return wait_for_dsp_spi_flash_access_to_cease();

}


/**
 * @brief      Waits for recently downloaded canvas to start running
 *
 * @return     True if it does, false if it times out
 */
bool wait_for_canvas_to_start(void) {

  DEBUG_MSG("Starting", MSG_DEBUG); 

  dsp_status.state_canvas_running = false;
  int delay = 25;
  int timeout_cntr_1s = 1000/delay;

  while (!dsp_status.state_canvas_running && timeout_cntr_1s) {
    spi_fifo_push_emptry_frame();  
    spi_transmit_buffered_frames(false);    
    int now = millis();
    while (millis() < now + delay) {
      display_data_from_sharc();
    }
    timeout_cntr_1s--;
  }
  if (!timeout_cntr_1s) {
    DEBUG_MSG("Canvas never started running", MSG_ERROR); 
    display_error_status(ERROR_CODE_DSP_NOT_BOOTING);
  }

  DEBUG_MSG("Complete", MSG_DEBUG); 

  return dsp_status.state_canvas_running;
} 





/**
 * @brief      Waits for the DSP to report the firmware version after boot
 *
 * @return     True if it was reported, false if timed out
 */
bool wait_for_dsp_firmware(void) {


  dsp_status.firmware_valid = false;
  int delay_time = 25;
  int timeout_cntr_1s = 5000/delay_time;

  while (!dsp_status.firmware_valid && timeout_cntr_1s--) {
    spi_fifo_push_emptry_frame();
    spi_transmit_buffered_frames(false);
    if (!dsp_status.firmware_valid) {
      delay(delay_time);
    }
  }

  if (!dsp_status.firmware_valid) {
    return false;
  } else {
    return true;
  }

}

void  display_dsp_firmware(void) {
  // Display firmwware version
  char msg[64], ver[10];
  String firmware_str = String(dsp_status.firmware_ver);
  firmware_str.replace("0",".");
  firmware_str.toCharArray(ver, sizeof(ver));
  sprintf(msg, "DSP firmware version: %s", ver);
  DEBUG_MSG(msg, MSG_INFO);  
}


/**
 * @brief      Waits for the DSP to finish booting and be ready for a canvas
 */
void  wait_for_dsp_to_boot(void) {

  DEBUG_MSG("Starting", MSG_DEBUG);

  int success_cntr = 5;
  bool booted = false;
  while (!booted && success_cntr) {
    spi_start();
    booted = wait_for_dsp_firmware();
    if (!booted) {
      DEBUG_MSG("DSP did not boot, attempting reset", MSG_WARN);
      spi_stop();
      if (!dmfx_debug_no_reset) {        
        dsp_reset();
      } else {
        DEBUG_MSG("DSP does not appear to be running but cannot reset since we're in no reset mode", MSG_ERROR);
        display_error_status(ERROR_CODE_DSP_NOT_BOOTING);
      } 
    }
    success_cntr--;
  }

  if (!success_cntr) {
    DEBUG_MSG("DSP was not able to boot", MSG_ERROR);
    display_error_status(ERROR_CODE_DSP_NOT_BOOTING);
  }

  display_dsp_firmware();

  DEBUG_MSG("Complete", MSG_DEBUG);

}

/**
 * @brief      Waits for DSP to complete startup routine
 */
void  wait_for_dsp_to_be_ready(void) {

  DEBUG_MSG("Starting", MSG_DEBUG);

  int delay = 25;
  

  bool ready = false;
  int success_cntr = 5;
  while (!ready && success_cntr) {
    int timeout_cntr_3s = 3000/delay;
    // wait for audio to start
    while ((dsp_status.state_flags & 0x70) != 0x70 && timeout_cntr_3s) {
      spi_fifo_push_emptry_frame();  
      spi_transmit_buffered_frames(false);    
      int now = millis();
      while (millis() < now + delay) {
        display_data_from_sharc();
      }
      timeout_cntr_3s--;
    }

    if (timeout_cntr_3s) {
      ready = true;
    } else {
      success_cntr--;
      if (!dmfx_debug_no_reset) {        
        DEBUG_MSG("Timeout waiting for DSP to start-up: resetting", MSG_ERROR);
        dsp_reset();
        wait_for_dsp_to_boot();
      }  else {
        DEBUG_MSG("DSP does not appear to be running but cannot reset since we're in no reset mode", MSG_ERROR);      
        display_error_status(ERROR_CODE_DSP_NOT_BOOTING);
      }
    }
  }
  DEBUG_MSG("Complete", MSG_DEBUG);

} 


/**
 * @brief      Routes canvas errors to serial interface
 */
void report_canvas_errors(void) {
  if (dsp_status.state_err_allocation) {
    DEBUG_MSG("Allocation error encountered while initializing effects", MSG_ERROR);
    display_error_status(ERROR_INTERNAL);
  }
  if (dsp_status.state_err_param) {
    DEBUG_MSG("Parameter error encountered while initializing effects", MSG_ERROR);
    display_error_status(ERROR_INTERNAL);
  }
  if (dsp_status.state_err_corrupt) {
    DEBUG_MSG("Corruption error encountered while initializing effects", MSG_ERROR);
    display_error_status(ERROR_INTERNAL);
  }
  if (dsp_status.state_err_other) {
    DEBUG_MSG("Other error encountered while initializing effects", MSG_ERROR);
    display_error_status(ERROR_INTERNAL);
  }
}




/**
 * @brief      Reads any serial telemetry data from the DSP and displays it
 */
void display_data_from_sharc(void) {
  static int line_indx = 0;
  static char line[256];

  while (Serial1.available() > 0) {
    char b = Serial1.read();
    if (b == '\n') {
      if (line_indx > 5) {
        line[line_indx] = 0;
        DEBUG_MSG(line, MSG_INFO);
      }
      line_indx = 0;
    } else {
      line[line_indx++] = b;
      if (line_indx >= sizeof(line)) {
        line_indx = sizeof(line) - 1;
      }
    }
  }
}




// Defines for the SPI flash memory
#define CMD_SPI_READ            (0x03)
#define CMD_SPI_PROG_PAGE       (0x02)
#define CMD_SPI_SECTOR_ERASE    (0x20)
#define CMD_SPI_BLOCK_ERASE     (0xD8)
#define CMD_SPI_CHIP_ERASE      (0xC7)
#define CMD_SPI_READ_STATUS     (0x05)
#define CMD_SPI_WRITE_STATUS    (0x05)
#define CMD_SPI_WRITE_EN        (0x06)
#define CMD_SPI_WRITE_EN_NV     (0x50)


#define SPI_STATUS_BUSY         (0x1)
#define SPI_STATUS_WRITE_EN     (0x2)


/**
 * @brief Asserts the reset line on the DSP
 * 
 */
void  dsp_assert_reset(void) {
  digitalWrite(SPI_SHARC_RESET, LOW);
  delay(1);
}

/**
 * @brief Deasserts the reset line on the DSP
 */
void  dsp_deassert_reset(void) {
  digitalWrite(SPI_SHARC_RESET, HIGH);
}

/**
 * @brief Starts a SPI transfer
 */
static void  spi_flash_start_transfer(void) {

    // SHARC bootloader defaults to LSB first
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(SPI_SHARC_SELECT, LOW);
}

/**
 * @brief Ends a SPI transfer
 */
static void  spi_flash_end_transfer(void) {
 
    SPI.endTransaction();
    digitalWrite(SPI_SHARC_SELECT, HIGH);
    delay(1);
}


/**
 * @brief Reads the status register in the spi flash
 */
static uint8_t spi_flash_read_status_register() {
    
    uint8_t result;
    
    spi_flash_start_transfer();
    
    SPI.transfer(CMD_SPI_READ_STATUS);   
    result = SPI.transfer(0x0);  
        
    spi_flash_end_transfer();
    
    return result;

}


/**
 * @brief Sends a single byte command to the flash
 */
static void spi_flash_send_byte(uint8_t val) {
   
    // Send a single byte command
    spi_flash_start_transfer();
    SPI.transfer(val);   
    spi_flash_end_transfer();
}

/**
 * @brief Checks to see if the SPI flash is busy
 */
static bool spi_flash_check_busy(void) {
    
    uint8_t val = spi_flash_read_status_register();
    
    if (val & SPI_STATUS_BUSY) return true;
    else return false;
}


/**
 * @brief Erases the entire flash memory
 */
static void    spi_flash_erase_chip(void) {

    if (Serial && dmfx_debug_mode) {
      Serial.print(" - Firmware update: erasing flash...");
    }
    spi_flash_send_byte(CMD_SPI_WRITE_EN);
    spi_flash_send_byte(CMD_SPI_CHIP_ERASE);
    while (spi_flash_check_busy());

    if (Serial && dmfx_debug_mode) {
      Serial.println(" complete");
    }

}

/**
 * @brief Not sure if needed but clears the non-volatile 
 * write protection bits in the event that they get set
 */
static void   spi_flash_clear_protect(void) {

    spi_flash_start_transfer();
    SPI.transfer(CMD_SPI_WRITE_EN_NV);
    spi_flash_end_transfer();


    spi_flash_start_transfer();
    
    SPI.transfer(CMD_SPI_WRITE_STATUS);   
    SPI.transfer(0x0);  
        
    spi_flash_end_transfer();
  
}


/**
 * @brief flips the bit order of a byte.  The DSP defaults to LSB first but the 
 * SPI commands need to be MSB first.  Since we can't change the interface
 * mid command, we need to manually flip the bits.  No worries though, we have all
 * day.
 */
static uint8_t flip_bit_order(uint8_t num) 
{ 
    unsigned int reverse_num = 0; 
    int i; 
    for (i=0; i<8; i++) 
    { 
        if((num & (1 << i))) 
           reverse_num |= 1 << ((8 - 1) - i);   
   } 
   return reverse_num; 
} 

/**
 * @brief Writes a 256 byte page to flash memory
 */
static bool    spi_flash_page_write(uint32_t address, const uint8_t * vals, uint16_t count) {
    
    if (count > 256) {
        return false;
    }

  static int page_led_cntr = 0;
  static int led_cntr = 0;
  if (page_led_cntr++ > 8) {

    if (led_cntr++ & 1) {
      turn_on_right_footsw_led();
      turn_off_left_footsw_led();
    } else {
      turn_on_right_footsw_led();
      turn_off_left_footsw_led();      
    }
    
    page_led_cntr = 0;
  }
      
    
    uint8_t spi_block[4] = {CMD_SPI_PROG_PAGE};
    
    // Load address
    spi_block[1] = (address >> 16) & 0xFF;
    spi_block[2] = (address >> 8) & 0xFF;
    spi_block[3] = (address >> 0) & 0xFF;


    spi_flash_send_byte(CMD_SPI_WRITE_EN);   
     
    spi_flash_start_transfer();
    for (int i=0;i<4;i++) {
       SPI.transfer(spi_block[i]);  
    }
    for (int i=0;i<count;i++) {
      SPI.transfer(flip_bit_order(vals[i]));        
    }
    spi_flash_end_transfer();    
    
    // 3. Wait for transaction to complete
    while (spi_flash_check_busy());
        
    return true;
}


const PROGMEM uint8_t firmware_image[] = {
    #include "dm_fx_dsp_firmware_image.dat"
};

/**
 * @brief      Flashes the DSP boot flash and then resets the DSP
 *
 * @return     True if successful, false if not
 */
bool dsp_update_firmware_image(void) {

  uint8_t * vals = (uint8_t *) firmware_image;
  uint32_t count = sizeof(firmware_image);

  // Set up SPI select pin
  pinMode(SPI_SHARC_SELECT, OUTPUT); 
  digitalWrite(SPI_SHARC_SELECT, HIGH); 

  pinMode(SPI_SHARC_RESET, OUTPUT); 
  digitalWrite(SPI_SHARC_RESET, HIGH); 


  // Start SPI port
  SPI.begin();  

  dsp_assert_reset();

  spi_flash_clear_protect();
  spi_flash_erase_chip();

  uint32_t page_count = (count >> 8) + 1;
  uint32_t address = 0;

  if (Serial && dmfx_debug_mode) {
    Serial.print(" - Firmware update: programming...");
  }

  turn_on_right_footsw_led();
  turn_off_left_footsw_led();

  uint32_t byte_cnt = 0;
  for (int i=0;i<page_count;i++) {    
    spi_flash_page_write(address, &vals[address], 256); 
    address += 256;
  }

  turn_on_right_footsw_led();
  turn_on_left_footsw_led();
  
  // Turn SPI select back into input so SHARC can boot
  pinMode(SPI_SHARC_SELECT, INPUT); 
  SPI.end(); 


	if (Serial && dmfx_debug_mode) {
	  Serial.println(" complete");
	}
  

  turn_off_right_footsw_led();
  turn_off_left_footsw_led();

  dsp_deassert_reset();

  return true;
  
}

#endif 