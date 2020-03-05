
.. _program_listing_file_src_dm_fx_spi_proto.h:

Program Listing for File dm_fx_spi_proto.h
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_dm_fx_spi_proto.h>` (``src/dm_fx_spi_proto.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

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
   
   
   
   void  spi_start(void);
   
   void  spi_stop(void);
   
   void spi_fifo_reset(void);
   
   void    spi_fifo_push_emptry_frame(void);
   
   bool  spi_fifo_insert_block(uint16_t * data, int size);
   
   void spi_transmit_buffered_frames(bool reset_state);
   
   #endif  // DOXYGEN_SHOULD_SKIP_THIS
   #endif  // DM_FX_SPI_PROTO_H
