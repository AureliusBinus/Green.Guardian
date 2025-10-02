# Waveshare ESP32-S3-Nano Board Definition

This directory contains a custom board definition for the **Waveshare ESP32-S3-Nano** development board, which is not officially supported by PlatformIO.

## About the Board

The Waveshare ESP32-S3-Nano is a compact ESP32-S3 development board featuring:

- **MCU**: ESP32-S3 dual-core processor @ 240MHz
- **Flash**: 16MB
- **RAM**: 512KB SRAM + 8MB PSRAM
- **Connectivity**: Wi-Fi 802.11 b/g/n, Bluetooth 5.0 LE
- **Form Factor**: Ultra-compact NANO size
- **USB**: Type-C connector with USB-to-UART bridge

## Usage

To use this board definition in your PlatformIO project, copy the `waveshare_esp32s3_nano.json` file to your project's `boards/` directory and reference it in your `platformio.ini`:

```ini
[env:waveshare_esp32s3_nano]
platform = espressif32
board = waveshare_esp32s3_nano
framework = arduino
```

## Global Installation

If you want to make this board definition permanently available across all PlatformIO projects, copy the `waveshare_esp32s3_nano.json` file to:

```path
C:\Users\<username>\.platformio\platforms\espressif32\boards\waveshare_esp32s3_nano.json
```

Replace `<username>` with your actual Windows username. This will make the board available system-wide for all your PlatformIO projects.

## Resources

- **Product Wiki**: [https://www.waveshare.com/wiki/ESP32-S3-Nano](https://www.waveshare.com/wiki/ESP32-S3-Nano)
- **Product Store**: [https://www.waveshare.com/esp32-s3-nano.htm](https://www.waveshare.com/esp32-s3-nano.htm)

## Board Configuration

The board definition includes:

- 16MB flash memory configuration
- PSRAM support enabled
- USB CDC on boot for easier programming
- QIO flash mode for optimal performance
- Arduino framework compatibility
