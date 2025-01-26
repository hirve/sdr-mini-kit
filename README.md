# Sdr Mini Kit
 Building SDR apps by mapping IQ streams with pipes in CLI or bash scripts. 
 
## Features
* Taking IQ stream from SoapySDR, Airspy HF+ and Sound card input
* Generating IQ stream with noise or empty signal
* Transmitting internet audio streams into IQ stream
* Lowpass filtering and decimating IQ stream
* Piping streams to TCP, GQRX, Icecast, playing it as an audio
* Generating waterfall image from IQ stream
* Playing and streaming waterfall as video
* SSB demodulation

## Installation
### Install dependencies
```
sudo apt install ffmpeg icecast2 sox
```
When Icecast asks to configure it say no, because we will use the config we have in the project.

### Install IQ sources
#### SoapySDR
Install it from https://github.com/pothosware/SoapySDR/

#### Airspy
If you want to get IQ stream from Airspy install their software from https://github.com/airspy

#### Sound card
You can take IQ stream from sound card stereo input, just make sure ```arecord``` command is working in your command line.

### Build C modules
Open the project root folder and run ```./build```

### Starting Icecast
If you want to use Icecast, go to the **icecast** folder and run ```./start-icecast```, the port is 4625.
If you want to share the Icecast stream via Internet - don't forget to replace the **hackme** password with some stronger.

## Usage
Look into bash scripts in the ```app-examples/``` folder, copy some of them into the ```app/``` folder, edit or connect modules from ```bin/``` folder with pipes another way you want. Use scripts in ```tune/``` folder to set common parameters like samplerate, antenna, default frequency, remote server IP or just hardcode these parameters in your apps.

## System requirements
Some apps can run on Raspberry Pi Zero. Other will require more calculation performance. Notice that Raspberry Pi Zero CPU bcm2835 can easily work at 1GHz so don't hesitate to overclock it.

## Thanks
* KissFFT - https://github.com/mborgerding/kissfft
* TFilter - http://t-filter.engineerjs.com/
