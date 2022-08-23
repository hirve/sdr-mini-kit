:warning: I was walking down the street when a rocket landed next to me. Some time ago it was simply unbelievable, now the destruction of my city and the killing of civilians is my reality. The organized criminal group has occupied Russia and now is destroying my peaceful city. 

<a title="Mvs.gov.ua, CC BY 4.0 &lt;https://creativecommons.org/licenses/by/4.0&gt;, via Wikimedia Commons" href="https://commons.wikimedia.org/wiki/File:Kharkiv_downtown_street_destroyed_by_Russian_bombardment.jpg"><img width="99%" alt="Kharkiv downtown street destroyed by Russian bombardment" src="https://upload.wikimedia.org/wikipedia/commons/thumb/f/f1/Kharkiv_downtown_street_destroyed_by_Russian_bombardment.jpg/1280px-Kharkiv_downtown_street_destroyed_by_Russian_bombardment.jpg"></a>

<a title="Mvs.gov.ua, CC BY 4.0 &lt;https://creativecommons.org/licenses/by/4.0&gt;, via Wikimedia Commons" href="https://commons.wikimedia.org/wiki/File:Bomb_attack_on_Saltivske_tram_depot_in_Kharkiv.jpg"><img width="99%" alt="Bomb attack on Saltivske tram depot in Kharkiv" src="https://upload.wikimedia.org/wikipedia/commons/c/cf/Bomb_attack_on_Saltivske_tram_depot_in_Kharkiv.jpg"></a>

<a title="Main Directorate of the State Emergency Service of Ukraine in Kharkiv Oblast, CC BY 4.0 &lt;https://creativecommons.org/licenses/by/4.0&gt;, via Wikimedia Commons" href="https://commons.wikimedia.org/wiki/File:Kharkiv_after_Russian_shelling,_31_March_2022_(14).jpg"><img width="99%" alt="Kharkiv after Russian shelling, 31 March 2022 (14)" src="https://upload.wikimedia.org/wikipedia/commons/thumb/8/85/Kharkiv_after_Russian_shelling%2C_31_March_2022_%2814%29.jpg/1280px-Kharkiv_after_Russian_shelling%2C_31_March_2022_%2814%29.jpg"></a>

<a title="Photo: Kharkivian (Serhii Petrov). From Wikimedia Commons. License CC BY-SA 4.0.Фото: Kharkivian (Сергій Петров). З Wikimedia Commons. Ліцензія CC BY-SA 4.0" href="https://commons.wikimedia.org/wiki/File:Destroyed_apartment_building_3_Kholodnohirska_Street,_Kharkiv_after_Russian_airstrike_by_Kharkivian_(03.2022)_01.jpg"><img width="99%" alt="Destroyed apartment building 3 Kholodnohirska Street, Kharkiv after Russian airstrike by Kharkivian (03.2022) 01" src="https://upload.wikimedia.org/wikipedia/commons/thumb/a/a1/Destroyed_apartment_building_3_Kholodnohirska_Street%2C_Kharkiv_after_Russian_airstrike_by_Kharkivian_%2803.2022%29_01.jpg/1280px-Destroyed_apartment_building_3_Kholodnohirska_Street%2C_Kharkiv_after_Russian_airstrike_by_Kharkivian_%2803.2022%29_01.jpg"></a>

If you are a responsible citizen of Russia, you are personally guilty of this. Stop it. If you are an irresponsible resident of Russia, just ignore this message.

# sdr-mini-kit
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
Look into bash scripts in the project root and connect modules with pipes similarly for your app. Use scripts in ```tune/``` folder to set common parameters like samplerate, antenna, default frequency, remote server IP.

## System requirements
Some apps can run on Raspberry Pi Zero. Other will require more calculation performance. Notice that Raspberry Pi Zero CPU bcm2835 can easily work at 1GHz so don't hesitate to overclock it.

## Thanks
* KissFFT - https://github.com/mborgerding/kissfft
* TFilter - http://t-filter.engineerjs.com/
