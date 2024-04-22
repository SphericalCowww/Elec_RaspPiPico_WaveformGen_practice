# Elec_RaspPiPico_WaveformGen_practice

## Blink LED at GPIO 10 pin
     mkdir build
     cd build
     cmake ../src
     cd blinkLED
     make
   
Connect ground, resistor, and LED like shown in `src/blinkLED/blinkLED.png`. Then connect the power of pico while pushing the white button, followed by:

     sudo cp blinkLED.uf2 /mnt/pico
     sudo sync

The output can also be tracked by the host machine via USB connection and ''minicom''. On terminal, see if ''/dev/ttyACM0'' connection exist:

     dmesg                
     lsusb
     ls /dev
Then do,

     minicom -b 115200 -o -D /dev/ttyACM0

## Blink LED at GPIO 10 pin with Raspberry PI's PIO 
     mkdir build
     cd build
     cmake ../src
     cd blinkLED_PIO
     make
   
Connect ground, resistor, and LED like shown in `src/blinkLED/blinkLED.png`. Then connect the power of pico while pushing the white button, followed by:

     sudo cp blinkLED_PIO.uf2 /mnt/pico
     sudo sync

State machines of PIO don't do output, so ''minicom'' will not help.
## David's 100kHz Arbitrary Waveform Generator
Using David's digital-to-analog converter (DAC) circuit and source code to build the arbitrary waveform generator. The resistor chain is patched with low-value resistors to get accurate resistance values (up to a multimeter's accuracy). The resistor group with 64 kOhmn is connected to the GPIO pin 8 of the pico,  32kOhmn to pin 9, up to 500 Ohmn to pin15, 8 pins in total with a 256-bit amplitude resolution. Notice that the orange wires are chosen to be the same length to prevent time shifts between each component; unfortunately, the same was not applied to the yellow wires. The op-amp is connected in the unity-gain configuration.
<figure>
  <img src="https://github.com/SphericalCowww/Elec_RaspPiPico_WaveformGen_practice/blob/main/dac_circuit_labeled.png" width="300">
</figure>

Coding-wise, David devised a speed-up plan swapping between 2 state machines. To modify the waveform, look into the following lines in ''arbitraryWaveformGen.c'':

     //////////////////////////// Put the AWG formula here:
     //awg_buff[i] = 128+(sin((factor)*2*PI)*127);
     awg_buff[i] = 128+(sin((factor)*2*PI)*80);

        
     //if(i < 128) 
     //    awg_buff[i] = bufdepth - 1;
     //else
     //    awg_buff[i] = 0;
     //////////////////////////////////////////////////////

To run the code, it is the same procedure:

     mkdir build
     cd build
     cmake ../src
     cd arbitraryWaveformGen
     make
   
Then connect the power of pico while pushing the white button, followed by:

     sudo cp arbitraryWaveformGen.uf2 /mnt/pico
     sudo sync

Monitor the C++ base serial output by:

     minicom -b 115200 -o -D /dev/ttyACM0

The resulting 100kHz square and sine waves are shown as follows. The waveform shape is to be desired, but hey, this simple setup gives 100kHz waveforms!
<p align="center">
  <img src="https://github.com/SphericalCowww/Elec_RaspPiPico_WaveformGen_practice/blob/main/100kHz_squareWave.png" width="500">
  <img src="https://github.com/SphericalCowww/Elec_RaspPiPico_WaveformGen_practice/blob/main/100kHz_sinWave.png" width="500">
</p>


## References:
- Raspberry Pi Pico C/C++ SDK (<a href="https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html">website</a>)
- DigiKey, Intro to Raspberry Pi Pico and RP2040 - C/C++ Part 3: How to Use PIO | Digi-Key Electronics (<a href="https://www.youtube.com/watch?v=_lZ1Pw6WAqI">YouTube</a>)
- Life with David, Raspberry Pi Pico PIO - Ep. 14 - Arbitrary Waveform Generator (<a href="https://www.youtube.com/watch?v=_lZ1Pw6WAqI">YouTube</a>, <a href="https://github.com/LifeWithDavid/Raspberry-Pi-Pico-PIO/blob/d244a4b7d0b5c187c08e7311026b45fdff7da13e/EP%2014%20AWG%20Files.txt">GitHub</a>)
- element14 presents, How Do DACs Work? - The Learning Circuit (<a href="https://www.youtube.com/watch?v=YAxrmoVtEtE&t=210s">YouTube</a>)

