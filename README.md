# sdr-mini-kit
Building SDR mapping IQ streams with linux pipes.

## Features
* Taking IQ stream from SoapySDR, Airspy HF+, and Sound card input
* Generating IQ stream with noise or empty signal
* Transmitting internet audio streams into IQ stream
* Lowpass filtering and decimating IQ stream
* Piping streams to TCP, GQRX, Icecast, playing it as audio
* Generating waterfall image from IQ stream

## Installation
Install dependencies:
```
sudo apt install ffmpeg icecast2
```
### SoapySDR
If you want to get IQ stream from SoapySDR install it from https://github.com/pothosware/SoapySDR/

### Airspy
If you want to get IQ stream from Airspy install thier software from https://github.com/airspy

### Sound card
You can take IQ stream from sound card stereo input, just make sure ```arecord``` command is working in your command line.

## Usage
Look into bash scripts in the project root and connect modules with pipes similarly for your app.

## Thanks
* KissFFT - https://github.com/mborgerding/kissfft
* TFilter - http://t-filter.engineerjs.com/
