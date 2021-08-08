# pocketDSP
A pocket-sized DSP for audio applications.

This device consists in a STM32H732 MCU, 24bit 192kHz ADC/DAC with high SiNAD, low noise opamp buffers, 3.5mm stereo jack input/output, 320x480 touchscreen display, li-ion battery and USB for battery recharging and Virtual COM Port.



Functions:


DSP: Create your own custom FX preset by selecting various standalone effects such as reverb, delay, phaser, IIR/FIR filters via the touchscreen display. Save/Load them from the embedded SD Card.


Spectrum Analyzer: Applies the FFT algorithm to the input signal, makes measurements on the amplitude of the harmonics and saves the data on the SD card.


Sound card: Connect the device to your pc to use it as a HiFi sound card.


Synthesizer: send midi commands through the USB interface and create sounds using the display to change the oscillators and effects parameters.



-- software still under development --



![](https://github.com/iamBVC/pocketDSP/blob/main/pcb_with_display.png)
![](https://github.com/iamBVC/pocketDSP/blob/main/pcb_without_display.png)
