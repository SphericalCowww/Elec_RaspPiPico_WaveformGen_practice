add_executable(blinkLED_PIO
        blinkLED_PIO.c
        blinkLED_PIO.pio
        )
pico_generate_pio_header(blinkLED_PIO ${CMAKE_CURRENT_LIST_DIR}/blinkLED_PIO.pio)

# pull in common dependencies
target_sources(blinkLED_PIO PRIVATE blinkLED_PIO.c)
target_link_libraries(blinkLED_PIO PRIVATE pico_stdlib hardware_pio)

# enable usb output, disable uart output
pico_enable_stdio_usb(blinkLED_PIO 1)
pico_enable_stdio_uart(blinkLED_PIO 0)


# create map/bin/hex file etc.
pico_add_extra_outputs(blinkLED_PIO)

