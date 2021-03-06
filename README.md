# pocketDSP
A pocket-sized DSP for audio applications.

This device consists in a STM32H732 MCU, 24bit 192kHz ADC/DAC with high SiNAD, low noise opamp buffers, 3.5mm stereo jack input/output, 480x320 touchscreen display, li-ion battery and USB for battery recharging and Virtual COM Port.



Functions:


DSP: Create your own FX preset by selecting various standalone effects such as reverb, delay, phaser, IIR/FIR filters via the touchscreen display. Save/Load in the SD Card.


Spectrum Analyzer: Applies the FFT algorithm to the input signal, makes measurements on the amplitude of the harmonics and saves the data on the SD card.


Sound card: Connect the device to your pc to use it as a HiFi USB Sound Card.


Synthesizer: send midi commands through the USB interface and create sounds using the display to change the oscillators and effects parameters.



-- software still under development --

<img src="https://github.com/iamBVC/pocketDSP/blob/master/img/pcb_with_display.png" width="40%"/><img src="https://github.com/iamBVC/pocketDSP/blob/master/img/pcb_without_display.png" width="40%"/>



GUI Screens:

<img src="https://github.com/iamBVC/pocketDSP/blob/master/img/gui_mainscreen.png" width="40%"/><img src="https://github.com/iamBVC/pocketDSP/blob/master/img/gui_dspmain.png" width="40%"/><img src="https://github.com/iamBVC/pocketDSP/blob/master/img/gui_dspedit.png" width="40%"/><img src="https://github.com/iamBVC/pocketDSP/blob/master/img/gui_oscilloscope.png" width="40%"/><img src="https://github.com/iamBVC/pocketDSP/blob/master/img/gui_fft.png" width="40%"/>
