# ESP32-S3 ThingsBoard Demo

This project is a demonstration firmware for an ESP32-S3 microcontroller designed to connect to the ThingsBoard IoT platform. It includes a web-based provisioning system to configure Wi-Fi and MQTT credentials.

## Features

- **Web-Based Provisioning**:
  - On the first boot, or if no Wi-Fi credentials are stored, the device starts in Access Point (AP) mode.
  - Connect to the **`ESP32-Provisioning`** Wi-Fi network (password: `password`).
  - Navigate to `http://192.168.4.1` in a web browser to access the provisioning page.
- **Dynamic Wi-Fi Scanning**:
  - The provisioning page can scan for and display available Wi-Fi networks.
- **Smart Boot**:
  - If Wi-Fi credentials are found in non-volatile storage (NVS), the device will automatically connect to the stored Wi-Fi network.
- **LED Status Indicator**:
  - The onboard ARGB LED (GPIO 48) provides visual feedback on the device's status:
    - **Dim White:** Provisioning mode
    - **Blue:** Connecting to Wi-Fi
    - **Green:** Connected to Wi-Fi and MQTT
    - **Red:** Connection failed
    - **Yellow:** MQTT disconnected or error
- **Real-Time Telemetry**:
  - After connecting to ThingsBoard, the device sends telemetry data every 5 seconds, including:
    - On-chip temperature
    - Wi-Fi RSSI
    - Free heap memory
    - Uptime

## Quick Start

1.  **Prerequisites**:
    - [ESP-IDF v5.4.1](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32s3/get-started/index.html)
    - An ESP32-S3 development board

2.  **Build and Flash**:
    - Set the target chip: `idf.py set-target esp32s3`
    - Build and flash the firmware: `idf.py -p /dev/ttyUSB0 flash` (replace `/dev/ttyUSB0` with your serial port)
    - Monitor the output: `idf.py -p /dev/ttyUSB0 monitor`

3.  **Provisioning**:
    - On the first boot, the device will start in provisioning mode.
    - Connect your computer or smartphone to the `ESP32-Provisioning` Wi-Fi network with the password `password`.
    - Open a web browser and go to `http://192.168.4.1`.
    - Use the web interface to scan for your Wi-Fi network, enter your credentials, and provide your ThingsBoard MQTT server details.
    - Click "Connect". The device will store the credentials and attempt to connect.

4.  **Normal Operation**:
    - On subsequent boots, the device will automatically connect to the configured Wi-Fi and MQTT server.
    - The LED will turn green upon a successful connection.

## Troubleshooting

-   **Reset to Provisioning Mode**: If you need to re-enter provisioning mode, you can erase the entire flash memory of the device by running `idf.py erase-flash`. This will clear the stored Wi-Fi and MQTT credentials.
-   **Check Logs**: Use `idf.py monitor` to view the device's log output for detailed information about the connection status and any errors.

## ThingsBoard Setup

This project is designed to work with a ThingsBoard server. You will need to:

1.  Set up a ThingsBoard instance (either cloud or self-hosted).
2.  Create a new device in ThingsBoard to get a device access token.
3.  Use the ThingsBoard server IP address or hostname and the device access token in the provisioning web page.
