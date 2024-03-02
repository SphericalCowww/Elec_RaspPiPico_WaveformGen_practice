add_executable(arbitraryWaveformGen
	arbitraryWaveformGen.c
	arbitraryWaveformGen.pio
)
pico_generate_pio_header(arbitraryWaveformGen ${CMAKE_CURRENT_LIST_DIR}/arbitraryWaveformGen.pio)

#target_sources(arbitraryWaveformGen.pio PRIVATE arbitraryWaveformGen.c)
# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(arbitraryWaveformGen
        pico_stdlib
        hardware_pio
		hardware_dma
        hardware_irq
		hardware_structs
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(arbitraryWaveformGen)

# Enable usb output, disable uart output
pico_enable_stdio_usb(arbitraryWaveformGen 1)
pico_enable_stdio_uart(arbitraryWaveformGen 0)





