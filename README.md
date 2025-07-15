# ESP32-S3 ThingsBoard Demo

This project is a demonstration firmware for an ESP32-S3 microcontroller designed to connect to the ThingsBoard IoT platform. It includes a robust web-based provisioning system to configure Wi-Fi credentials and demonstrates a connection to a public MQTT broker.

## Current Features

- **Web-Based Wi-Fi Provisioning**:
  - On first boot (or if no credentials are stored), the device starts in Access Point mode.
  - Connect to the **`ESP32-Provisioning`** Wi-Fi network (password: `password`).
  - Navigate to `http://192.168.4.1` in a web browser.
- **Dynamic Wi-Fi Scanning**:
  - The provisioning page includes a "Scan for Networks" button that dynamically finds and lists available Wi-Fi networks, showing their SSID, signal strength (RSSI), and security type.
- **MQTT Connectivity**:
  - After provisioning, the device connects to the configured Wi-Fi network.
  - It performs an internet connectivity check by resolving `google.com`.
  - It connects to the public HiveMQ MQTT broker (`broker.hivemq.com`) and is ready to send and receive data.
- **Persistent Configuration**:
  - The firmware is currently configured to always start in provisioning mode for development purposes.
- **ARGB LED Status Indicator**:
  - The onboard ARGB LED provides a visual indication of the device's status:
    - **White:** Provisioning mode
    - **Blue:** Connecting to Wi-Fi
    - **Green:** Connected to Wi-Fi and MQTT
    - **Red:** Error state (e.g., Wi-Fi or MQTT connection failed)

## Getting Started

### Prerequisites

- [ESP-IDF v5.4.1](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32s3/get-started/index.html)
- An ESP32-S3 development board

### Build and Flash

1.  **Set the Target Chip**:
    ```bash
    idf.py set-target esp32s3
    ```

2.  **Build and Flash the Firmware**:
    Connect the ESP32-S3 board and run the following command, replacing `/dev/ttyUSB0` with your device's serial port if necessary.
    ```bash
    idf.py -p /dev/ttyUSB0 flash
    ```

3.  **Monitor the Output**:
    To view the log output from the device, run:
    ```bash
    idf.py -p /dev/ttyUSB0 monitor
    ```

## Project Status

This project is currently in active development. The next steps involve integrating with a local ThingsBoard instance and implementing telemetry data collection.
