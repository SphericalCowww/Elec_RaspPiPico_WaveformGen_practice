#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"

// Our assembled program:
#include "arbitraryWaveformGen.pio.h"

#define PI 3.1428592654
#define SM_CLK_FREQ 25600000       //divid over 256 to get Hz
int main()
{
	stdio_init_all(); 
	uint OUT_PIN_NUMBER = 8;  
	uint NPINS          = 8;   
	uint bufdepth       = 256; 
	float factor;
	
    PIO pio = pio0;
	uint sm = pio_claim_unused_sm(pio, true);
	uint offset = pio_add_program(pio, &pio_byte_out_program);
    pio_byte_out_program_init(pio, sm, offset, OUT_PIN_NUMBER, NPINS, SM_CLK_FREQ);

	int wave_dma_chan_a = dma_claim_unused_channel(true);
    int wave_dma_chan_b = dma_claim_unused_channel(true);
		
    uint8_t awg_buff[256] __attribute__((aligned(256))); // remember to change when changing bufdepth 
	for (int i = 0; i < bufdepth; ++i) {
		factor = (float) i/bufdepth;

//////////////////////////// Put the AWG formula here:
		//awg_buff[i] = 128+(sin((factor)*2*PI)*127);
        awg_buff[i] = 128+(sin((factor)*2*PI)*80);

        
        //if(i < 128) 
        //    awg_buff[i] = bufdepth - 1;
        //else
        //    awg_buff[i] = 0;
//////////////////////////////////////////////////////
	}
		
    dma_channel_config wave_dma_chan_a_config = dma_channel_get_default_config(wave_dma_chan_a);
    channel_config_set_transfer_data_size(&wave_dma_chan_a_config, DMA_SIZE_32); 
    channel_config_set_read_increment(    &wave_dma_chan_a_config, true);
    channel_config_set_write_increment(   &wave_dma_chan_a_config, false);
	channel_config_set_chain_to(          &wave_dma_chan_a_config, wave_dma_chan_b); 
    channel_config_set_dreq(              &wave_dma_chan_a_config, DREQ_PIO0_TX0); 
	channel_config_set_ring(              &wave_dma_chan_a_config, false, 8);
	
    dma_channel_config wave_dma_chan_b_config = dma_channel_get_default_config(wave_dma_chan_b);
    channel_config_set_transfer_data_size(&wave_dma_chan_b_config, DMA_SIZE_32);
    channel_config_set_read_increment(    &wave_dma_chan_b_config, true);
    channel_config_set_write_increment(   &wave_dma_chan_b_config, false);
	channel_config_set_chain_to(          &wave_dma_chan_b_config, wave_dma_chan_a);
	channel_config_set_dreq(              &wave_dma_chan_b_config, DREQ_PIO0_TX0);
	channel_config_set_ring(              &wave_dma_chan_b_config, false, 8);
	
    dma_channel_configure(
        wave_dma_chan_a,
        &wave_dma_chan_a_config,
        &pio0_hw->txf[sm],          // Write address (sm1 transmit FIFO)
        awg_buff,                   // Read values from fade buffer
        bufdepth,                   // 256 values to copy
        false                       // Don't start yet.
	);
    dma_channel_configure(
        wave_dma_chan_b,
        &wave_dma_chan_b_config,
        &pio0_hw->txf[sm],
        awg_buff,
        bufdepth,
        false 
    );
    dma_start_channel_mask(1u << wave_dma_chan_a);

	while(true) {
        for  (int k = 0; k<256; ++k)  {
			printf("awg_buff[%i]: %i sm: %i \n", k, awg_buff[k], sm);
			printf(" wave_dma_chan_a: %i wave_dma_chan_b: %i \n", wave_dma_chan_a, wave_dma_chan_b);
			//printf("ch#: %i &dma_hw->ch[wave_dma_chan_b].al2_write_addr_trig: %i \n", dma_hw->ch[wave_dma_chan_b].al2_write_addr_trig, &dma_hw->ch[wave_dma_chan_b].al2_write_addr_trig);
			sleep_ms(100);
		}	
    }
}





