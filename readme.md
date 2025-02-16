# Home automation system based on Bluetooth Mesh and STM32

## Table of Contents
-[Overview](#Overview)
-[Features](#Features)
-[Getting started](#Getting%20started)
-[Usage](#Usage)
-[Notes](#Notes)

## Overview
This project was created as part of my BSC at University of Ljubljana, Faculty of Computer and Information Science.
It implements a home automation system based on Bluetooth Mesh which is designed to run on STM32 hardware. Through
a GUI it is possible to scan and add new nodes to the network, change settings for individual nodes, safely
remove nodes from the network, turn on/off connected appliances, dim lights, and collect sensor measurements.

There are three separate projects, each for a type of node. Two of them are based on STM32CubeWB, specifically
the Bluetooth Mesh examples (with heavy modification). H7 project is built from the ground up.

## Features
- Based on STM32CubeWB
- Friendly GUI made with TouchGFX
- Custom UART protocol
- Modular system architecture based on layers, which can be easily modified
- Modular command interface
- Support for multiple sensors
- Built-in ScioSence APC1 driver
- Built-in on/off and dimmer
- Debug options
- FSM
- FreeRTOS
- Custom 3D-printable cases

## Getting started
### Prerequisites
To build and run this project, you will need:
- At least three STM32 development boards (**ONE** [STM32H750B-DK](https://www.st.com/en/evaluation-tools/stm32h750b-dk.html#st_all-features_sec-nav-tab), **MULTIPLE** [P-NUCLEO-WB55](https://www.st.com/en/evaluation-tools/p-nucleo-wb55.html#overview))
- STM32CubeIDE
Optional components:
- [ScioSence APC1 sensor](https://www.sciosense.com/apc1-air-quality-combo-sensor/)

### Explanation for each project
`H7MeshProvisioner` is intended for STM32H750B-DK and includes GUI, acts as the master
for the UART protocol, implements the modular system architecture, command interface,
debug options, FSM, and part of sensor support. Through this STM32 board, the user will
interact with the Bluetooth Mesh network. 
Files:
- `H7MeshProvisioner/STM32CubeIDE/Application/User/libs/` Contains backend libraries.
- `H7MeshProvisioner/Core/Src/` Contains files for setup.
- `H7MeshProvisioner/TouchGFX/` Contains GUI.

`BLE_MeshLightingProvisioner` is the embedded provisioner and acts as a gateway for the user
to interact with the network. It communicates with STM32H750B-DK (user) through the custom
UART protocol as slave. It is based on the BLE_MeshLightingProvisioner example found in STM32CubeWB, but
with heavy modification. To be used with P-NUCLEO-WB55.
Files:
- `BLE_MeshLightingProvisioner/STM32CubeIDE/Application/Core/Libs/` Contains libraries that expand embedded provisioner capabilities.
- `BLE_MeshLightingProvisioner/STM32_WPAN/app/` Contains Bluetooth Mesh files (with modifications).

`BLE_MeshLightingPRFNode` is the project to be used with every node in the network (except the
embedded provisioner and H7). It enables relay, proxy and friend features with generic on/off, 
generic level, generic power and sensor models (with their associated models).
It is based on BLE_MeshLightingPRFNode example found in STM32CubeWB, but
with heavy modification. To be used with P-NUCLEO-WB55.
Files:
- `BLE_MeshLightingPRFNode/STM32CubeIDE/Application/Core/libs/` Contains sensor driver (APC1), dimmer & device settings.
- `BLE_MeshLightingPRFNode/STM32_WPAN/app/` Contains Bluetooth Mesh files (with modifications).

Each project contains a `device_settings.h` which sets the inner workings of a node through a set
of flags.

### Installation
1. Clone this repository.
2. Open the desired project in STM32CubeIDE.
3. Based on the compiled project, connect the correct STM32 board and flash the firmware to it.

### Wiring diagram
STM32H750B-DK with flashed H7MeshProvisioner project and one P-NUCLEO-WB55 with flashed BLE_MeshLightingProvisioner
must be connected together in order for the UART communication to take place. This is essential,
as it ensures user interaction with the Bluetooth Mesh network. To create the connection, follow the diagram below.

![wiring diagram](connection.png)

## Usage
1. Power on the STM32 boards with the flashed firmware.
2. Through the GUI, search for unprovisioned devices and add them to the network.
3. Once provisioning is complete, you can change settings for each individual node (name, room placement).
4. Based on what models are enabled on a node, you can test the communication by either turning a connected device on or off, dim a light or get sensor measurements.
5. When you are done using a node, it can be removed from the network through the GUI.

## Notes
This project is considered as done as I don't plan to create any updates in the near future. However, contributions are welcome as there are
multiple things that can still be implemented, for example:
- expand on model support,
- save network configuration to some sort of permanent storage,
- additional error handling,
- add more sensors.

Driver for ScioSence APC1 is available [here](https://github.com/NejcVene/NucleoF411RE-APC1).
A much more in-depth explanation is available [here](#) **(only in Slovene)**.
Video demonstration is available [here](#).
Although relay feature is enabled by default it is not tested, therefor the behavior of a node connected to the network and a mobile app
is unknown.