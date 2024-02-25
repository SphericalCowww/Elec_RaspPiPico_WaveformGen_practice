#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    const uint LED_PIN = 10;
    const int sleepTime = 1000;

    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while(true) {
        gpio_put(LED_PIN, 1);
        printf("ON");
        sleep_ms(sleepTime);
        gpio_put(LED_PIN, 0);
        printf("OFF");
        sleep_ms(sleepTime);
    }
    return 0;
}
