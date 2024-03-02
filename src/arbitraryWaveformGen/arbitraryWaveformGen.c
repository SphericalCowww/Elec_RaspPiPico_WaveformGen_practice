#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"

// Our assembled program:
#include "arbitraryWaveformGen.pio.h"

#define PI 3.1428592654
#define SM_CLK_FREQ 10000
int main()
{
	stdio_init_all();           // Set up to print out
	uint OUT_PIN_NUMBER = 8;    // Start of output pin group
	uint NPINS          = 8;    // Number of output pins
	uint bufdepth       = 256;  // The number of samples in the AWG buffer table, must be a power of 2 (i.ie, 2,4,8,16,32,64...)
	float factor;
	
	// Choose which PIO instance to use (there are two instances)
    PIO pio = pio0;
	uint sm = pio_claim_unused_sm(pio, true);
	uint offset = pio_add_program(pio, &pio_byte_out_program);
    pio_byte_out_program_init(pio, sm, offset, OUT_PIN_NUMBER, NPINS, SM_CLK_FREQ);

	// wave_dma_chan_a and wave_dma_chan_b loads AWG buffer table to PIO in ping pong method
	int wave_dma_chan_a = dma_claim_unused_channel(true);
    int wave_dma_chan_b = dma_claim_unused_channel(true);
		
	// Define the waveform buffer to hold the waveform
    uint8_t awg_buff[256] __attribute__((aligned(256))); 

	for (int i = 0; i < bufdepth; ++i) {
		factor=(float)i/bufdepth; // Convert interger division to floating point

//////////////////////////// Put the AWG formula here:
		awg_buff[i] = 128+(sin((factor)*2*PI)*127); // Loads the AWG Buffer table with values of the wave
//////////////////////////////////////////////////////
	}
		
	// Set up the wave_dma_chan_a DMA channel
    dma_channel_config wave_dma_chan_a_config = dma_channel_get_default_config(wave_dma_chan_a);
    // Transfers 32-bits at a time, increment read address so we pick up a new table value each
    // time, don't increment write address so we always transfer to the same PIO register.
    channel_config_set_transfer_data_size(&wave_dma_chan_a_config, DMA_SIZE_32); //Transfer 32 bits at a time (4 bytes)
    channel_config_set_read_increment(    &wave_dma_chan_a_config, true);
    channel_config_set_write_increment(   &wave_dma_chan_a_config, false);
	channel_config_set_chain_to(          &wave_dma_chan_a_config, wave_dma_chan_b);    // After block has been transferred, wave_dma_chan b
    channel_config_set_dreq(              &wave_dma_chan_a_config, DREQ_PIO0_TX0);      // Transfer when PIO asks for a new value
	channel_config_set_ring(              &wave_dma_chan_a_config, false, 8);           // Wrap every 256 bytes
	
	// Setup the wave_dma_chan_b DMA channel
    dma_channel_config wave_dma_chan_b_config = dma_channel_get_default_config(wave_dma_chan_b);
    channel_config_set_transfer_data_size(&wave_dma_chan_b_config, DMA_SIZE_32);
    channel_config_set_read_increment(    &wave_dma_chan_b_config, true);
    channel_config_set_write_increment(   &wave_dma_chan_b_config, false);
	channel_config_set_chain_to(          &wave_dma_chan_b_config, wave_dma_chan_a);
	channel_config_set_dreq(              &wave_dma_chan_b_config, DREQ_PIO0_TX0);
	channel_config_set_ring(              &wave_dma_chan_b_config, false, 8);
	
    // Setup the first wave DMA channel for PIO output
    dma_channel_configure(
        wave_dma_chan_a,
        &wave_dma_chan_a_config,
        &pio0_hw->txf[sm],          // Write address (sm1 transmit FIFO)
        awg_buff,                   // Read values from fade buffer
        bufdepth,                   // 256 values to copy
        false                       // Don't start yet.
	);
    // Setup the second wave DMA channel for PIO output
    dma_channel_configure(
        wave_dma_chan_b,
        &wave_dma_chan_b_config,
        &pio0_hw->txf[sm],          // Write address (sm1 transmit FIFO)
        awg_buff,                   // Read values from fadeoff buffer
        bufdepth,                   // 256 values to copy
        false                       // Don't start yet.
    );
	
    // Everything is ready to go. Now start the first DMA
    dma_start_channel_mask(1u << wave_dma_chan_a);

    // This section for communicating with terminal; either debugging or future control
	while(true) {
        for  (int k = 0; k<256; ++k)  {
			printf("awg_buff[%i]: %i sm: %i \n", k, awg_buff[k], sm);
			printf(" wave_dma_chan_a: %i wave_dma_chan_b: %i \n", wave_dma_chan_a, wave_dma_chan_b);
			//printf("ch#: %i &dma_hw->ch[wave_dma_chan_b].al2_write_addr_trig: %i \n", dma_hw->ch[wave_dma_chan_b].al2_write_addr_trig, &dma_hw->ch[wave_dma_chan_b].al2_write_addr_trig);
			sleep_ms(100);
		}	
    }
}





