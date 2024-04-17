#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blinkLED_PIO.pio.h"

int main() {
    static const uint  led_pin  = 10;
    static const float pio_freq = 1;       // not exactly in H

    stdio_init_all();
    PIO pio = pio0;
    uint memory_offset = pio_add_program(pio, &blinkLED_PIO_program);
    printf("Loaded program at %d\n", memory_offset);

    uint stateMachine = pio_claim_unused_sm(pio, true);
    blinkLED_PIO_program_init(pio, stateMachine, memory_offset, led_pin);
    pio_sm_set_enabled(pio, stateMachine, true);
    pio->txf[stateMachine] = clock_get_hz(clk_sys)/(2*pio_freq) - 3;    // factor 2 for on/off
    printf("Blinking pin %d at %d Hz\n", led_pin, pio_freq);

    // without the following, serial output on /dev/ttyACM0 does not connect
    while(true) { }

    /*
    // can also blink with the following independently
    const uint LED_PIN = 25;
    const int sleepTime = 1000;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while(true) {
        gpio_put(LED_PIN, 1);
        //printf("ON\n");
        sleep_ms(sleepTime);
        gpio_put(LED_PIN, 0);
        //printf("OFF\n");
        sleep_ms(sleepTime);
    }
    */
    return 0;
}
