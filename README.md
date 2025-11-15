# Football Eye Q firmware

## Description

This repository houses the firmware project developed using the Arduino framework. The project focuses on creating a mesh network that is controlled through a web server. Within this network, each node is responsible for driving an RGB LED strip. Notably, the controller device within the network functions as a Wi-Fi access point and hosts a web server. This web server allows users to issue a series of patterns, which are subsequently deployed across the entire network.

## Table of Contents

- [Features](#features)
- [Materials](#materials)
- [IDE_Installation](#IDE_Installation)
- [Libraries_Installation](#Libraries_installation)
- [Usage](#usage)
- [DeviceModes](#DeviceModes)

## Features

List the main features of your project, such as:

- Wi-Fi Access point.
- Web Server.
- ESP-NOW Mesh network implementation.
- Ability to drive up to 60 Leds.

## Materials

- ESP32-S3 N16R8 Development board.
- WS2813-5V-RGB-LED-STRIP

## IDE_Installation

- Download and install Arduino IDE version 1.8.15 https://www.arduino.cc/en/software/OldSoftwareReleases
- Start Arduino and open the preferences window https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html
- Enter this link ( https://espressif.github.io/arduino-esp32/package_esp32_index.json ) into Additional Board Manager URLs field.
- Open Boards Manager from Tools > Board menu and install esp32 platform from espressif. install version 2.0.11.
- Open tools , Boards and select ESP32S3 Dev Module, keep board settings as default.
- Install ESP32 file system uploader, it is well explained in this link, https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide

## Libraries_Installation

- Open sketch tab and the library manager, install the following libraries with the corresponding versions.
- FastLed version 3.6.0
- SPIFFS version 2.0.0
- FS version 2.0.0
- ArduinoJson version 6.21.3
- WiFi version 2.0.0
- ESP Async WebServer version 1.2.3
- AsyncTCP version 1.1.4

# Usage

- Download (Clone) the project.
- From this link download the repository https://bitbucket.org/guifeseka/football-eye-q/src/master/
- Unzip the project.
- Open the .ino file.
- connect the controller device USB-C port.
- **IMPORTANT NB:**--->**Before connecting the device to your laptop make sure the device is OFF and connect it to the USB port next to the charging port, never the charging port**
- Once connected check device manager on windows to verify that a serial com port device has been added.
- On arduino with the project open select tools/port/COMx the com port associated with your device.
- From tools open tools and click on ESP32 Skecth data upload, this will flash the WEB server files on to the file system on the ESP32S3, the blue led on the dev board should be lit blue while flashing.
- Once that is completed upload the applciation from the arduino IDE upload button.
- To verify that programming succeded open the serial port of the device and chekc the logs as it should be displaying the logs in Wi-Fi Manager mode.

# DeviceModes

Since the source code for both node and controller devices is contained within this repo, in order to programmatically switch between node and controller mode, refer to DeviceModel.h where setting the DEVICE_MODE_OPERATION definition will set the appropriate code for each mode, note that the project needs to be recompiled when switching between modes, **also when the device is a node the file system upload step does not need to be performed.**

```
#ifndef DEVICE_MODE_CONTROLLER
#define DEVICE_MODE_CONTROLLER 0
#endif

#ifndef DEVICE_MODE_NODE
#define DEVICE_MODE_NODE 1
#endif

#ifndef DEVICE_MODE_OPERATION
#define DEVICE_MODE_OPERATION DEVICE_MODE_CONTROLLER
#endif
```
