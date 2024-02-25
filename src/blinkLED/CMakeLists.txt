add_executable(blinkLED
        blinkLED.c
        )

# pull in common dependencies
target_link_libraries(blinkLED pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(blinkLED)

