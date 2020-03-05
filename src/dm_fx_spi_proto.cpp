// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved. 
// This code is licensed under MIT license (see license.txt for details)

#include <SPI.h>

#include "dreammakerfx.h"
#include "dm_fx_spi_proto.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/************************************************************************
 *
 *                        SPI Protocol
 *
 ***********************************************************************/

#define SPI_SPEED_HZ    		8000000


#define MAX_SPI_BLOCK_SIZE      (2048)
#define SPI_FIFO_SIZE           (2048)
#define SPI_FIFO_MASK           (SPI_FIFO_SIZE-1)

#define SPI_RX_FRAME_SIZE       (SPI_DSP_STAT_FRAME_SIZE + 3)
#define SPI_RX_PAYLOAD_SIZE     (SPI_DSP_STAT_FRAME_SIZE)

// Frame constants 
#define FRAME_HEADER_1                (0x80FD)
#define FRAME_HEADER_2                (0x80FE)
#define FRAME_TERMINATOR              (0x80FF)


// SPI Interface
uint16_t    spi_tx_fifo[SPI_FIFO_SIZE];
uint16_t    spi_rx_frame[SPI_RX_FRAME_SIZE];



// DSP status structure
DSP_STATUS   dsp_status;

// Initialize SPI communications
uint16_t  spi_tx_wr_ptr = 0;
uint16_t  spi_tx_rd_ptr = 0;
int16_t   spi_rx_wr_ptr = 0;
SPI_RX_STATE  spi_rx_state = SPI_RX_WAITING;
uint16_t  spi_service_last_millis = 0;



/**
 * @brief      Starts the SPI peripheral
 */
void  spi_start(void) {

  // Start SPI port
  pinMode(SPI_SS_PIN, OUTPUT);
  digitalWrite(SPI_SS_PIN, HIGH);    
  SPI.begin();  

  // Reset SPI frame in case we just updated firmware
  spi_transmit_buffered_frames(true); 


}

/**
 * @brief      Stops the SPI peripheral
 */
void  spi_stop(void) {

  // Stop SPI peripheral
  SPI.end();
  pinMode(PIN_DSP_SPI_FLASH_SELECT, INPUT);

}

/**
 * @brief      Clears the SPI receive FIFO
 */
void 	spi_fifo_reset(void) {
  spi_tx_wr_ptr = 0;
  spi_tx_rd_ptr = 0;
  memset(spi_rx_frame, 0, sizeof(spi_rx_frame));
  memset(&dsp_status, 0, sizeof(dsp_status));
}



/**
  * @brief Push a 16-bit word into the FIFO
  */ 
static bool spi_fifo_push(uint16_t val) {

  if (SPI_FIFO_MASK & (spi_tx_wr_ptr+1) == spi_tx_rd_ptr) {
    DEBUG_MSG("Not enough room in FIFO", MSG_WARN);
    return false;
  }
  spi_tx_fifo[spi_tx_wr_ptr++] = val;
    
  spi_tx_wr_ptr &= SPI_FIFO_MASK;

  return true;

}

/**
 * @brief      Pushes an empty frame to DSP to retrieve data from DSP
 */
void 	spi_fifo_push_emptry_frame(void) {
    for (int i=0;i<SPI_RX_FRAME_SIZE;i++) {
      spi_fifo_push(0);
    }   
}


/**
 * @brief Returns the number of free words in the FIFO
 */
static uint16_t spi_fifo_available_space(void) {
  if (spi_tx_wr_ptr > spi_tx_rd_ptr) {
    return SPI_FIFO_SIZE - (spi_tx_wr_ptr - spi_tx_rd_ptr);
  } 

  else if (spi_tx_rd_ptr > spi_tx_wr_ptr) {
    return (spi_tx_rd_ptr - spi_tx_wr_ptr) - 1;
  } 

  else {
    return SPI_FIFO_SIZE;
  }
}

/**
 * @brief  Adds a SPI transmission frame to the SPI FIFO
 * 
 * A SPI transmission block basically has the lenght of the block in the first location
 * and is followed by the block of data
 *
 * @param      data  The data
 * @param[in]  size  The size
 * 
 * return   False if size is greater than max SPI block size
 */
bool  spi_fifo_insert_block(uint16_t * data, int size) {

#if 0
  char msg[64];
  sprintf(msg,"Inserting block of size %d", size);
  DEBUG_MSG(msg, MSG_INFO);
#endif
  // Check if block exceeds size
  if (size > MAX_SPI_BLOCK_SIZE-3) {
    DEBUG_MSG("SPI block size too big", MSG_ERROR);
    display_error_status(ERROR_INTERNAL);
  }

  // Check if block will fit in the fifo
  if (size > spi_fifo_available_space()) {
    DEBUG_MSG("SPI FIFO full", MSG_ERROR);
    display_error_status(ERROR_INTERNAL);
  }

  // Add frame to FIFO

  // 1. Add frame headers
  spi_fifo_push(FRAME_HEADER_1);
  spi_fifo_push(FRAME_HEADER_2);
  
  // 2. Add frame size
  spi_fifo_push(size);

  // 3. Add frame payload
  for (int i=0;i<size;i++) {
    spi_fifo_push(data[i]);
  }

  // 4. Add frame terminator
  spi_fifo_push(FRAME_TERMINATOR);

}




void spi_process_received_frame(uint16_t * rx_frame) {
  

  #if 0
    Serial.println("-------");
    for (int i=0;i<SPI_RX_PAYLOAD_SIZE;i++) {
      Serial.println(rx_frame[i], HEX);
    }
  #endif
  dsp_status.firmware_ver = rx_frame[SPI_DSP_STAT_FIRMWARE_MAJ] << 16;
  dsp_status.firmware_ver |= rx_frame[SPI_DSP_STAT_FIRMWARE_MIN];
  dsp_status.firmware_valid = true;
  if (dsp_status.firmware_ver < 10000 || dsp_status.firmware_ver > 99999) {
    dsp_status.firmware_valid = false;
  }

  dsp_status.loading_percentage = 100.0 * ((float) rx_frame[SPI_DSP_STAT_MIPS_PERCENT] * (1.0/65536.0)); 
  dsp_status.amplitude =  ((float) rx_frame[SPI_DSP_STAT_AMPLITUDE]) / 65536.0;
  dsp_status.new_note = rx_frame[SPI_DSP_STAT_NEW_NOTE];

  // Get system state
  uint16_t sys_state = rx_frame[SPI_DSP_STAT_SYS_STATE];
  dsp_status.state_flags = sys_state;
  dsp_status.state_booted = ((sys_state & SYS_VALID) == SYS_VALID)?true:false;
  dsp_status.state_initialized = (sys_state & SYS_INITIALIZED)?true:false;
  dsp_status.state_lf_audio_running = (sys_state & SYS_LF_AUDIO)?true:false;
  dsp_status.state_hf_audio_running = (sys_state & SYS_HF_AUDIO)?true:false;
  dsp_status.state_canvas_running  = (sys_state & SYS_CANVAS_OK)?true:false;



  dsp_status.state_err_allocation  = (sys_state & SYS_ERR_ALLOC)?true:false;
  dsp_status.state_err_param  = (sys_state & SYS_ERR_PARAM)?true:false;
  dsp_status.state_err_corrupt  = (sys_state & SYS_ERR_CRPT)?true:false;
  dsp_status.state_err_other  = (sys_state & SYS_ERR_OTHER)?true:false;

  
}


/**
 * @brief      Transmits any frames to the DSP
 */
void spi_transmit_buffered_frames(bool reset_state) {

  static uint8_t  rx_frame_state = 0;
  static uint16_t rx_frame_ptr = 0;
  static uint16_t rx_frame_buf[16];

  if (reset_state) {
    rx_frame_state = SPI_RX_WAITING;
    return;
  }

  // If there are no words in the fifo, return
  if (spi_tx_wr_ptr == spi_tx_rd_ptr) {
    return;
  }  


  // If we last serviced the spi port less than 10 milliseconds ago, hold off
  uint32_t now = millis();
  if (spi_service_last_millis + 10 > now) {
    return;
  }
  spi_service_last_millis = now;

  // Begin SPI transaction
  SPI.beginTransaction(SPISettings(SPI_SPEED_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(SPI_SS_PIN, LOW);

  uint16_t rx_word;
  while (spi_tx_wr_ptr != spi_tx_rd_ptr) {

    // SPI TX/RX operation
    rx_word = SPI.transfer16(spi_tx_fifo[spi_tx_rd_ptr++]);
    if (spi_tx_rd_ptr >= SPI_FIFO_SIZE) {
      spi_tx_rd_ptr = 0;
    }

    //Serial.println(rx_word, HEX);

    // SPI process received frame
    if (spi_rx_state == SPI_RX_RECEIVING && rx_word == FRAME_TERMINATOR) {
      spi_rx_state = SPI_RX_WAITING;
      spi_process_received_frame(spi_rx_frame);
    } else if (spi_rx_state == SPI_RX_RECEIVING) {
      spi_rx_frame[spi_rx_wr_ptr++] = rx_word;

      // if we've received a complete frame, set state to ready
      if (spi_rx_wr_ptr >= SPI_RX_PAYLOAD_SIZE) {
        spi_rx_wr_ptr = SPI_RX_PAYLOAD_SIZE - 1;
      }
    }
    else if (spi_rx_state == SPI_RX_WAITING && rx_word == FRAME_HEADER_1) {
      spi_rx_state = SPI_RX_HEADER_1_RX;
    } 
    else if (spi_rx_state == SPI_RX_HEADER_1_RX && rx_word == FRAME_HEADER_2) {
      spi_rx_state = SPI_RX_RECEIVING;
      spi_rx_wr_ptr = 0;

    } 
    
  }

  // End transaction
  digitalWrite(SPI_SS_PIN, HIGH);  
  SPI.endTransaction();


}
#endif // DOXYGEN_SHOULD_SKIP_THIS
