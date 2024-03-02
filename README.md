# Elec_RaspPiPico_WaveformGen_practice

## Blink LED at GPIO 10 pin
     mkdir build
     cmake ../src
     cd blinkLED
     make
   
Connect ground, resistor, and LED like shown in `src/blinkLED/blinkLED.png`. Then connect the power of pico while pushing the white button, followed by:

     sudo cp blinkLED.uf2 /mnt/pico
     sudo sync

## Blink LED at GPIO 10 pin with Raspberry PI's PIO 
     mkdir build
     cmake ../src
     cd blinkLED_PIO
     make
   
Connect ground, resistor, and LED like shown in `src/blinkLED/blinkLED.png`. Then connect the power of pico while pushing the white button, followed by:

     sudo cp blinkLED_PIO.uf2 /mnt/pico
     sudo sync

## David's 100kHz Arbitrary Waveform Generator

## References:
- Raspberry Pi Pico C/C++ SDK (<a href="https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html">website</a>)
- DigiKey, Intro to Raspberry Pi Pico and RP2040 - C/C++ Part 3: How to Use PIO | Digi-Key Electronics (<a href="https://www.youtube.com/watch?v=_lZ1Pw6WAqI">YouTube</a>)
- Life with David, Raspberry Pi Pico PIO - Ep. 14 - Arbitrary Waveform Generator (<a href="https://www.youtube.com/watch?v=_lZ1Pw6WAqI">YouTube</a>)

