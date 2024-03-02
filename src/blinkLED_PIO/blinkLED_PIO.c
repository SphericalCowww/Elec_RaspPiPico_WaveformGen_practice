#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blinkLED_PIO.pio.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    static const uint LED_PIN = 10;
    static const float pio_freq = 2000;       // 2000Hz is the minimum freq

    PIO pio = pio0;
    uint stateMachine = pio_claim_unused_sm(pio, true);
    uint memoryOffset = pio_add_program(pio, &blinkLED_PIO_program);
    float clockDiv = ((float) clock_get_hz(clk_sys))/pio_freq;
    blinkLED_PIO_init(pio, stateMachine, memoryOffset, LED_PIN, clockDiv);
    pio_sm_set_enabled(pio, stateMachine, true);


    const int sleepTime = 1000;
    while(true) {
        sleep_ms(sleepTime);            // does nothing, everything done by PIO
    }
    return 0;
}
