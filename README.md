# BareMetalCamera

![Build](https://github.com/niaBaldoni/BareMetalCamera/actions/workflows/build.yml/badge.svg)

BareMetalCamera is a from-scratch OV5640 bring-up on the RP2040 using PIO and DMA, with no vendor camera drivers. It recreates a DCMI-style parallel camera interface entirely in software.

## About This Project

This is an educational project focused on low-level embedded systems. The goal is to build a camera interface from scratch: no libraries, no pre-existing drivers, and no dedicated camera peripheral. Just the RP2040 datasheet, the OV5640 datasheet, and a fair amount of register archaeology.

This project builds a DCMI-style interface on hardware that does not natively provide one, providing a hands-on way to understand parallel camera interfaces.

**Why?** 

Camera sensors are a great stress test for embedded systems. Interfacing with one requires I2C configuration, clock generation, high-throughput parallel data capture, PIO state machines, DMA transfers, and careful timing across multiple clock domains.

Rather than relying on an existing library, the focus here is to understand the entire pipeline end-to-end by building it manually on the RP2040.

## Current Status

### Milestone 1: ChipID Read 

Successfully established communication with the OV5640 camera module.

#### What's working:
- SCCB (I2C equivalent) communication with camera
- PWM XCLK generation (~31 MHz) operational
- Power sequencing (RESET and PWDN pins)
- Reading Chip ID registers

#### Output:

```
OV5640 SCCB test
Chip ID High Byte: 0x56
Chip ID Low Byte: 0x40
Chip ID: 0x5640
```

This confirms the Pico can talk to the camera and read its registers.

## Hardware

- Raspberry Pi Pico (RP2040)
- Waveshare OV5640 Camera Board (C)

## Build Instructions

Requires Pico SDK installed and configured.

```bash
    cd BareMetalCamera
    mkdir build && cd build
    cmake ..
    make
```

Flash `BareMetalCamera.uf2` to your Pico.

## Milestones

- [x] Verified SCCB/I2C communication
- [ ] Configure camera registers for basic operation
- [ ] Implement PIO state machine for parallel data capture
- [ ] Set up DMA for frame transfers
- [ ] Actually get an image out of this thing

## Non-Goals

This project focuses on low-level bring-up and understanding. It is not intended to be a polished or production-ready camera driver.

Higher-level abstractions, advanced image tuning, and broader hardware support are deliberately out of scope, though they may be explored in separate projects or future iterations.

## Resources

### Hardware Documentation
- [OV5640 Datasheet (PDF)](https://files.waveshare.com/upload/d/da/OV5640_DataSheet.pdf)
- [Waveshare OV5640 Camera Board (C) User Manual (PDF)](https://files.waveshare.com/upload/5/52/OV5640_Camera_Board_%28C%29_User_Manual_EN.pdf)
- [RP2040 Datasheet (PDF)](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
- [Raspberry Pi Pico W pinouts](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#picow-technical-specification)

### Pico SDK
- [Raspberry Pi Pico SDK Documentation](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)
- [PIO (Programmable I/O) Guide](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html#pio)

