# sdr-mini-kit
 Building SDR apps by mapping IQ streams with pipes in CLI or bash scripts. 
 
## Features
* Taking IQ stream from SoapySDR, Airspy HF+ and Sound card input
* Generating IQ stream with noise or empty signal
* Transmitting internet audio streams into IQ stream
* Lowpass filtering and decimating IQ stream
* Piping streams to TCP, GQRX, Icecast, playing it as an audio
* Generating waterfall image from IQ stream

## Installation
Install dependencies:
```
sudo apt install ffmpeg icecast2
```
Install IQ sources:
#### SoapySDR
Install it from https://github.com/pothosware/SoapySDR/

#### Airspy
If you want to get IQ stream from Airspy install their software from https://github.com/airspy

#### Sound card
You can take IQ stream from sound card stereo input, just make sure ```arecord``` command is working in your command line.

### Build C modules
Open project root folder and run ```./build```

### Starting Icecast
If you want to use Icecast, go to the **icecast** folder and run ```./start-icecast```

## Usage
Look into bash scripts in the project root and connect modules with pipes similarly for your app.

## System requirements
Some apps can run on Raspberry Pi Zero. Other will require more calculation performance. Notice that Raspberri Pi Zero CPU bcm2835 can easily work at 1GHz so don't hesitate to overclock it.

## Thanks
* KissFFT - https://github.com/mborgerding/kissfft
* TFilter - http://t-filter.engineerjs.com/
