# ESP32-S3 ThingsBoard IoT Solution

This project is a production-ready firmware for ESP32-S3 microcontrollers that securely connects to the ThingsBoard IoT platform. It includes a robust web-based provisioning system with ThingsBoard access token authentication, enterprise-grade security features, and comprehensive real-time telemetry capabilities with professional dashboard visualization.

## Current Features

- **Web-Based Wi-Fi Provisioning**:
  - On first boot, the device starts in Access Point mode.
  - Connect to the **`ESP32-Provisioning`** Wi-Fi network (password: `password`).
  - Navigate to `http://192.168.4.1`. The page comes pre-filled with default credentials for quick setup.
- **Dynamic Wi-Fi Scanning**:
  - The provisioning page includes a "Scan for Networks" button with an improved UI that gracefully handles long network names.
- **ThingsBoard Integration**:
  - **Access Token Authentication**: Web-based provisioning with ThingsBoard device access tokens
  - **Smart Protocol Detection**: Automatic MQTT (port 1883) vs MQTTS (port 8883) selection
  - **Pre-configured Setup**: Default ThingsBoard server (193.164.4.51) and demo access token
  - **Seamless Telemetry**: Compatible with ThingsBoard's `v1/devices/me/telemetry` format
  - **Enterprise Security**: Full MQTTS/TLS implementation with certificate management
- **Enhanced LED Status Indicator**:
  - The onboard ARGB LED (GPIO 48) provides detailed visual indication of device status:
    - **White:** Provisioning mode active
    - **Blue:** Connecting to Wi-Fi network
    - **Green:** Fully connected to Wi-Fi and MQTT/ThingsBoard
    - **Red:** Connection error (Wi-Fi or MQTT connection failure)
    - **Brightness:** Configurable (0-255, default 25)
- **Real-Time Telemetry**:
  - **Temperature Monitoring**: Built-in ESP32-S3 sensor (Range: -10°C ~ 80°C, ±1°C accuracy)
  - **System Metrics**: RSSI, heap memory, uptime tracking
  - **Transmission**: JSON payload every 5 seconds over MQTT to ThingsBoard
  - **Current Memory**: ~323KB free heap at startup
- **Certificate Management System**:
  - Secure certificate storage in NVS with integrity checking
  - Automatic certificate initialization on first boot
  - Development certificate fallback for testing
  - Certificate rotation support without firmware updates
- **Professional Dashboard System**:
  - Comprehensive dashboard creation guide
  - Custom widget bundle with professionally designed visualization components
  - Responsive design that works on both desktop and mobile devices
  - Support for importing pre-built widgets or manual creation

## 🚀 Quick Start (5 Minutes)

### For Immediate Testing:
1. **Flash Firmware**: `idf.py -p /dev/ttyUSB0 flash monitor`
2. **First Boot**: Device automatically starts in provisioning mode (no credentials stored)
3. **Connect to Wi-Fi**: Join `ESP32-Provisioning` network (password: `password`)
4. **Configure Device**: Navigate to `http://192.168.4.1` - all ThingsBoard settings are pre-filled!
5. **Smart Boot**: After restart, device connects directly to stored Wi-Fi and ThingsBoard
6. **LED Status**: Green = fully connected to ThingsBoard, transmitting telemetry every 5 seconds

### Smart Device Behavior:
- **First Boot**: Automatic provisioning mode (no Wi-Fi credentials stored)
- **After Configuration**: Smart boot directly to Wi-Fi connection (credentials stored)
- **ThingsBoard Ready**: Pre-configured with server IP and demo access token for instant testing
- **Reset to Provisioning**: Use `idf.py erase-flash` to clear credentials and return to provisioning mode

## 📊 ThingsBoard Integration & Dashboard

### Complete Setup Guides

**🎯 New User? Start Here:**
**[Complete Setup Guide → Thingsboard_Complete_Setup.md](./thingsboard/docs/Thingsboard_Complete_Setup.md)**
- Device creation & access token setup
- ESP32 provisioning with ThingsBoard connection
- Professional dashboard creation with custom widgets
- Advanced monitoring with alerts & Mattermost integration

**📋 Dashboard Only:**
**[Quick Dashboard Guide → Thingsboard_Dashboard.md](./docs/Thingsboard_Dashboard.md)**
- For existing ThingsBoard devices
- Widget import and configuration
- Professional styling and layouts

### Features Included
- **Real-time telemetry**: Temperature, RSSI, heap memory, uptime (5-second updates)
- **Professional widgets**: Glass-morphism design with animations
- **Mobile responsive**: Works on desktop, tablet, and mobile
- **Smart alerts**: RSSI thresholds with Mattermost notifications
- **Easy import**: Pre-built widget JSON files included

## Getting Started (Detailed Setup)

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

## Troubleshooting

### SSL/TLS Certificate Issues

**Expected Log Messages (Success):**

**First Boot (Initial Setup):**
```
I (451) PROVISIONING_EXAMPLE: CA certificate not found in NVS, initializing...
I (451) PROVISIONING_EXAMPLE: CA certificate stored in NVS successfully
I (451) PROVISIONING_EXAMPLE: ThingsBoard/Mosquitto CA certificate initialized successfully
I (451) PROVISIONING_EXAMPLE: No Wi-Fi credentials found, starting provisioning mode
I (621) PROVISIONING_EXAMPLE: WiFi AP started. SSID:ESP32-Provisioning password:password
```

**Subsequent Boots (Smart Boot):**
```
I (451) PROVISIONING_EXAMPLE: CA certificate already exists in NVS, skipping initialization
I (451) PROVISIONING_EXAMPLE: Wi-Fi credentials found, connecting directly
I (451) PROVISIONING_EXAMPLE: Connecting to stored Wi-Fi network: YourSSID
I (24391) PROVISIONING_EXAMPLE: CA certificate loaded from security NVS namespace
I (24881) PROVISIONING_EXAMPLE: MQTT_EVENT_CONNECTED
```

**Common SSL/TLS Errors:**

1. **"CA certificate not found in NVS"**
   - **Cause**: Certificate not properly stored or NVS corruption
   - **Solution**: Flash updated firmware with automatic certificate initialization

2. **"SSL/TLS error occurred - error code: 0x8010"**
   - **Cause**: Certificate validation failure or expired certificate
   - **Solution**: Verify MQTT broker certificate matches stored CA certificate

3. **"MQTT connection failed"**
   - **Cause**: Incorrect MQTTS port or SSL configuration
   - **Solution**: Ensure MQTT broker uses port 8883 and matches certificate

### Memory Issues
- **Low heap warning**: Device has ~323KB free at startup
- **Certificate storage**: Requires ~2KB NVS space per certificate (one-time write)
- **Boot optimization**: Certificate initialization skipped on subsequent boots for faster startup
- **Monitor**: Use `esp_get_free_heap_size()` for memory tracking

### Certificate Management
- **First Boot**: Automatically initializes and stores CA certificate in NVS
- **Subsequent Boots**: Detects existing certificate and skips initialization (faster boot time)
- **Memory Efficient**: Certificate written only once, reducing NVS wear
- **Manual Reset**: To force certificate re-initialization, erase NVS: `idf.py erase-flash`

### Network Troubleshooting
- **Wi-Fi connection**: LED shows blue during connection, green when successful
- **MQTTS connection**: LED turns red on SSL/TLS failures
- **Telemetry**: Check for `MQTT_EVENT_PUBLISHED` messages every 5 seconds

## Project Status

This project is **production-ready** with enterprise-grade security features:
- ✅ **SSL/TLS Security**: Full MQTTS encryption with certificate validation
- ✅ **Automatic Certificate Management**: One-time certificate initialization with NVS optimization
- ✅ **Real-Time Telemetry**: Temperature monitoring and system metrics transmission
- ✅ **Robust Provisioning**: Web-based Wi-Fi and MQTT configuration
- ✅ **LED Status Indicators**: Visual feedback for all connection states
- ✅ **Memory Optimized**: ~323KB free heap with efficient certificate storage
- ✅ **Professional Dashboard System**: Comprehensive visualization with custom widgets
- ⏳ **RPC Control Features**: Partially implemented (see Day 3 tasks)

## Development Roadmap

### Completed Features
- **Day 1**: Core firmware with Wi-Fi provisioning and basic telemetry
- **Day 2**: UI/UX enhancements, security features, and ThingsBoard integration
- **Dashboard System**: Professional visualization components and documentation

### Upcoming Features
- **Day 3**: RPC control functionality for remote device control
- **REST API Integration**: Examples for retrieving telemetry via REST APIs
- **Enhanced Widget Library**: Additional specialized widgets for specific use cases

## Frequently Asked Questions (FAQ)

### Q: What's the difference between MQTT and MQTTS?
**A:** 
- **MQTT** (port 1883) = Unencrypted, insecure communication
- **MQTTS** (port 8883) = Encrypted with SSL/TLS certificates (what this project uses)

### Q: Do I need to generate certificates manually?
**A:** The ESP32 firmware includes the CA certificate automatically. You only need to generate server certificates for your MQTT broker.

### Q: Why does the ESP32 connect automatically after the first setup?
**A:** The firmware implements smart boot logic:
- **First boot**: No credentials → starts provisioning mode
- **After setup**: Credentials stored → connects directly to Wi-Fi
- **Certificate optimization**: CA certificate stored once, reused on subsequent boots

### Q: How do I reset the ESP32 to provisioning mode?
**A:** Run `idf.py erase-flash` to clear all NVS data (Wi-Fi credentials and certificates). The device will return to provisioning mode on next boot.

### Q: Does the device always start in provisioning mode?
**A:** No! The device only starts provisioning mode if no Wi-Fi credentials are stored. After initial setup, it connects directly to the configured network.