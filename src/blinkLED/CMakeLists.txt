add_executable(blinkLED
        blinkLED.c
        )

# pull in common dependencies
target_link_libraries(blinkLED pico_stdlib)

pico_enable_stdio_usb(blinkLED 1)
pico_enable_stdio_uart(blinkLED 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(blinkLED)

