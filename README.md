# High Altitude Balloon Flight Computer
![License](https://img.shields.io/badge/license-MIT-brightgreen)

![fixed2 Medium](https://github.com/user-attachments/assets/e6891c12-59c6-4485-ae5b-b37eb12b2812)

## Overview

This repository contains the code of a flight computer designed for a High Altitude Balloon (HAB) that successfully reached an altitude of 100,000 feet, and returned to the ground undamaged. It performed live data transmission, including temperature, pressure, altitude, and position.

## Features

- **Altitude Achievement**: Successfully reached 100,000 feet and landed safely.
- **Real-time Data Transmission**: Live streaming of:
  - Temperature
  - Pressure
  - Altitude
  - GPS Position
- **Robust Design**: Engineered to withstand extreme conditions at high altitudes. Mostly through the liberal application of hot glue.

## Hardware Components

- **Microcontroller**: Arduino UNO R3
- **Sensors**:
  - Temperature, Pressure and Altitude Sensor: Adafruit MPL3115A2 I2C Barometric Pressure/Altitude/Temperature Sensor
  - GPS Module: Matek Ublox GPS Module SAM-M10Q
- **Power Supply**: 8xAA Batteries, The Pi Hut Battery Pack
- **Transmitter**: Radiometrix TX2B-434MHz LoRa Radio Module
- **Onboard Storage**: Switch Electronics Micro SD Card Mini TF Reader Module  

## Software Overview

The flight computer software is developed using the Arduino system's modified C++. It is responsible for:

- Collecting data from sensors (Temperature, pressure, altitude, location etc.)
- Formatting and transmitting data via RTTY-50
- Logging data for post-flight analysis

## Base Station

After the flight computer transmitted its data, I used a Software Defined Radio (SDR) to decode the radio transmissions to keep track of the flight computer's GPS co-ordinates. The data pipeline is as follows:

1. Flight computer transmits recorded data via radio broadcast
2. Directional yagi antenna recieves radio waves
3. Radio waves are decoded to an audio signal using a Yaesu FT 790-r Transciever*
4. Audio signal passed to a laptop's sound card
5. Fldigi** is used to decode the RTTY signal from the soundcard
6. Fldigi outputs the decoded characters to the screen

\*   Amateur UHF radio transceiver. Recommended by HAB experts, as it is (relatively) cheap compared to other transcievers.

**  [Fldigi](http://www.w1hkj.com/index.html) is a piece of software that allows the user to use their computer's soundcard as a two-way data modem. It is able to interpret various radio encoding modes and output encoded data as text. 

## Images

![Photo of the HAB flight computer.](https://github.com/user-attachments/assets/d7ea2f38-bf88-4604-abfc-0bb3990452b6)

## Results and Analysis

During the test flight, the balloon successfully transmitted data up to its maximum altitude. However, we encountered issues with the altimeter only being rated for a third of the height we wanted to send it to. Additionally, I would have liked to send live photos back to the base station in flight, however time constraints prevented me from integrating the onboard GoPro cameras with the data transmission system.

![Temperature_vs_time 2](https://github.com/user-attachments/assets/48b11b56-198f-4f09-a0da-d1204faeca54)

![IMG_5316 Medium](https://github.com/user-attachments/assets/feabf078-fc2d-43f2-81a8-ead93fe424a5)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thanks to the [UKHAS wiki](https://ukhas.org.uk/doku.php?id=start), for all of its incredibly useful and beginner-friendly guides.
- Additional thanks to the members of the 2022 HAB society at the Harrodian School for their invaluable research into parachute deployment and balloon physics, as well as landing site prediction.
- Most importantly, thanks to Jake Murray and Sam Wardell, without whom none of this could have happened.


