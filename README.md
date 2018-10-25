# Metec Braille cell driver

[![Build Status](https://travis-ci.org/bertrandmartel/metec-braille-driver.svg?branch=master)](https://travis-ci.org/bertrandmartel/metec-braille-driver)
[![Build status](https://ci.appveyor.com/api/projects/status/pdviuiomaut1rsml?svg=true)](https://ci.appveyor.com/project/bertrandmartel/metec-braille-driver)
[![NuGet](https://img.shields.io/nuget/v/MetecDriverLibrary.svg)](https://nuget.org/packages/MetecDriverLibrary)
[![License](http://img.shields.io/:license-mit-blue.svg)](LICENSE.md)

Driver for [Metec Braille-line 20 Cell standard](http://web.metec-ag.de/downloads/braille-line-20cell.pdf) module for Arduino & Raspberry PI

This module is composed of 20 [B11 Metec Braille cells](http://web.metec-ag.de/downloads/b11b12-elektronik-1.pdf) driven by piezo-actuators which also feature one interaction button per cell (eg 20 buttons for this module). The module also needs a [DC/DC converter (5V to 200V)](http://web.metec-ag.de/downloads/dcdc-converter-5to200v.pdf)

This library is compatible with :

- [x] Arduino (tested on Uno & Mini)
- [x] Raspberry PI Linux & wiringPi library
- [x] Rapsberry PI Windows IoT core OS

This repo also includes an example using this driver and [liblouis](https://github.com/liblouis/liblouis) altogether on Raspberry PI (Linux/wiringPi)

## Wiring

### Arduino Uno

![arduino Uno breadboard view](https://user-images.githubusercontent.com/5183022/47248430-a652b600-d40a-11e8-8ced-310bd6c8ae7f.png)

<p align="center">
	<img src="https://user-images.githubusercontent.com/5183022/47248424-a5ba1f80-d40a-11e8-8e9f-79540d522d15.png"/>
</p>

### Arduino Mini

![arduino Mini breadboard view](https://user-images.githubusercontent.com/5183022/47248429-a652b600-d40a-11e8-9e56-dde1b47eabe3.png)

<p align="center">
	<img src="https://user-images.githubusercontent.com/5183022/47248423-a5ba1f80-d40a-11e8-9e67-aebf52e5a0e1.png"/>
</p>

### Raspberry PI

![Raspberry PI breadboard view](https://user-images.githubusercontent.com/5183022/47248425-a5ba1f80-d40a-11e8-9ae6-27ba3e887656.png)

<p align="center">
	<img src="https://user-images.githubusercontent.com/5183022/47248422-a5218900-d40a-11e8-83f3-a8cceafdcf55.png"/>
</p>

## Setup

```bash
git clone git@github.com:bertrandmartel/metec-braille-driver.git
cd metec-braille-driver
git submodule update --init --recursive
```

## Project structure

    .
    ├── src                              # library source
    ├── include                          # library includes
    ├── arduino      
    │   ├── arduino-makefile             # makefile for arduino (build environment)
    │   ├── examples                     # examples containing one folder per example
    │   └── Makefile                     # library Makefile (each sample call this Makefile)
    ├── raspberry      
    │   ├── linux-wiringpi               # Linux build with wiringPi library folder
    │   │   ├── examples                 # examples folder
    │   │   ├── raspberry-dev            # toolchain & wiringPi build using cmake
    │   │   ├── out                      # library output directory
    │   │   ├── out_liblouis             # liblouis output directory
    │   │   ├── CmakeList.txt            # cmake config for building MetecDriver and liblouis
    │   │   ├── liblouis_install.sh      # liblouis table install script (to run on Raspberry PI)
    │   │   ├── run.sh                   # build script
    │   │   └── clean.sh                 # clean raspberry pi folder (remove cache, output dir etc...)
    │   ├── windows-iot                  # Windows IoT core folder
    │   │   └── MetecDriver 
    │   │       ├── MetecDriverLibrary   # library project compiling C++/CX into Windows Runtime Component
    │   │       ├── ExampleButtonSimple  # buttons input example in C#
    │   │       ├── ExampleButtons       # buttons input + write pattern example in C#
    │   │       ├── ExamplePatternChange # change pattern every x sec example in C++
    │   │       ├── MultiCellsExample    # write multiple pattern example in C++
    │   │       └── SingleCellExample    # write single pattern example in C++
    │   └── liblouis                     # liblouis submodule build folder
    ├── 8pin-adapter                     # KiCad files for 2.00mm to 2.52mm 8 pin adapter
    └─── fritzing                        # Fritzing sketch for wiring pictures

## Library Usage

* **[Go to Arduino README](./arduino)**
* **[Go to Raspberry PI Linux + wiringPi README](./raspberry/linux-wiringpi)**
* **[Go to Raspberry PI Windows IoT Core README](./raspberry/windows-iot)**

## 8 Pin Adapter 

The 8 pin adapter 2.00mm to 2.54mm was created on [KiCad EDA](http://kicad-pcb.org/), files are available under `8pin-adapter` :

![8 Pin Adapter](https://user-images.githubusercontent.com/5183022/47248426-a5ba1f80-d40a-11e8-8a8c-5aa408a3e197.png)

![8 Pin Adapter](https://user-images.githubusercontent.com/5183022/47248427-a5ba1f80-d40a-11e8-8f7b-7ef568995aa7.png)

## Protocol

### Braille pattern

3 signals are used : 

* STRB (OUTPUT)
* CLK (OUTPUT)
* DIN (OUTPUT)

A shift register is used to write the Braille pattern, the data is written to DIN between each clock transition (data is written on CLK falling edge). All bit are latched on STRB falling edge.

The bit order for the Braille cell is the following XORed : 

![bit order](https://user-images.githubusercontent.com/5183022/47248421-a5218900-d40a-11e8-9b38-fee9b4d3111d.png)

You can use [this tool](https://bertrandmartel.github.io/braille-dot-converter/) to test values

For example the following write letter "V" aka ⠧ which is [BRAILLE-1236 in unicode](http://www.unicode.org/charts/PDF/U2800.pdf) : 

![braille pattern](https://user-images.githubusercontent.com/5183022/47248433-a652b600-d40a-11e8-9e03-e979d0b7086b.png)

explanation for above : ⠧ gives 11000011 (XORed), bit order is 6/7/2/1/0/5/4/3 so it gives : 11011000, and XORed : 00100111

### Interaction buttons

3 signals are used : 

 * STRB (OUTPUT)
 * CLK (OUTPUT)
 * DOUT (INPUT)

The buttons can be read when strobe is high between each clock transtion (falling edge). B11 module can have 2 buttons so we need to read 20 * 2 times each time we check for button state :

![button interaction](https://user-images.githubusercontent.com/5183022/47248432-a652b600-d40a-11e8-94ef-a5eadad55eda.png)

explanation for above: the 2 last bit read are 01 which means the first button has been pressed 

## More resources

* This amazing post on Arduino forum : https://forum.arduino.cc/index.php?topic=236677.0 special thanks to metec_ab

* [dot converter tool](https://bertrandmartel.github.io/braille-dot-converter/) written in React JS is useful to quickly test a Braille pattern

* [liblouis](https://github.com/liblouis/liblouis) the open source Braille translator which can be used on Raspberry PI along with this driver (see Raspberry PI library usage)

* [Raspberry PI cross compile guide](https://medium.com/@au42/the-useful-raspberrypi-cross-compile-guide-ea56054de187)

* [raspberry-dev](https://github.com/bertrandmartel/raspberry-dev) : a quick development environment to develop for Raspberry PI

* [arduino-dev](https://github.com/bertrandmartel/arduino-makefile) for developping Arduino using a single Makefile

* [Fritzing parts](https://github.com/bertrandmartel/fritzing-parts) I've made for README aesthetics

## License

The MIT License (MIT) Copyright (c) 2018 Bertrand Martel