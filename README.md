# High Altitude Balloon Flight Computer
![License](https://img.shields.io/badge/license-MIT-brightgreen)

image goes here

## Overview

This repository contains the code of a flight computer designed for a High Altitude Balloon (HAB) that successfully reached an altitude of 100,000 feet, and returned. It performed live data transmission, including temperature, pressure, altitude, and position.

## Features

- **Altitude Achievement**: Successfully reached 100,000 feet.
- **Real-time Data Transmission**: Live streaming of:
  - Temperature
  - Pressure
  - Altitude
  - GPS Position
- **Robust Design**: Engineered to withstand extreme conditions at high altitudes.

## Hardware Components

- **Microcontroller**: Arduino UNO R3
- **Sensors**:
  - Temperature, Pressure and Altitude Sensor: Adafruit MPL3115A2 I2C Barometric Pressure/Altitude/Temperature Sensor
  - GPS Module: Matek Ublox GPS Module SAM-M10Q
- **Power Supply**: 8xAA Batteries, The Pi Hut Battery Pack
- **Transmitter**: Radiometrix NTX2B-434MHz LoRa Radio Module
- **Onboard Storage**: Switch Electronics Micro SD Card Mini TF Reader Module  

## Software Overview

The flight computer software is developed using the Arduino system's modified C++. It is responsible for:

- Collecting data from sensors
- Formatting and transmitting data via RTTY-50
- Logging data for post-flight analysis

## Base Station

After the flight computer transmitted its data, I used a Software Defined Radio (SDR) to decode the radio transmissions to keep track of the flight computer's GPS co-ordinates. The data flow was as follows:

- Flight computer transmits 
For recieving the transmitted data, we used a Yagi antenna, and a Yaesu FT 790-r Transciever to decode it
For decoding the recieved data, we used [Fldigi](http://www.w1hkj.com/index.html). Fldigi is a 

## Images

tbd

## Results

During the test flight, the balloon successfully transmitted data up to its maximum altitude. However, we encountered issues with the altimeter ; Time constraints prevented me from integrating the onboard GoPro cameras with the data transmission system

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Special thanks to Jake Murray and Sam Wardell, without whom none of this could have happened.
- Additional thanks to the members of the 2022 HAB society at the Harrodian School


