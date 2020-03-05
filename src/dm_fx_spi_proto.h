#ifndef DM_FX_SPI_PROTO_H
#define DM_FX_SPI_PROTO_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// Protocol constants
#define HEADER_INSTANCE_BLOCK         (0x8001)
#define HEADER_AUDIO_ROUTING_BLOCK    (0x8002)
#define HEADER_CONTROL_ROUTING_BLOCK  (0x8003)
#define HEADER_PARAMETER_BLOCK        (0x8004)
#define HEADER_SINGLE_PARAMETER       (0x8005)
#define HEADER_SET_BYPASS             (0x8006)
#define HEADER_GET_STATUS             (0x8007)



/**
 * @brief      Starts the SPI peripheral
 */
void  spi_start(void);

/**
 * @brief      Stops the SPI peripheral
 */
void  spi_stop(void);

/**
 * @brief      Clears the SPI receive FIFO
 */
void spi_fifo_reset(void);

/**
 * @brief      Pushes an empty frame to DSP to retrieve data from DSP
 */
void 	spi_fifo_push_emptry_frame(void);

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
bool  spi_fifo_insert_block(uint16_t * data, int size);

/**
 * @brief      Transmits any frames to the DSP
 */
void spi_transmit_buffered_frames(bool reset_state);

#endif  // DOXYGEN_SHOULD_SKIP_THIS
#endif 	// DM_FX_SPI_PROTO_H