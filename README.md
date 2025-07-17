# ESP32-S3 ThingsBoard Demo

This project is a demonstration firmware for an ESP32-S3 microcontroller designed to connect to the ThingsBoard IoT platform. It includes a robust web-based provisioning system to configure Wi-Fi credentials and demonstrates a connection to a public MQTT broker.

## Current Features

- **Web-Based Wi-Fi Provisioning**:
  - On first boot, the device starts in Access Point mode.
  - Connect to the **`ESP32-Provisioning`** Wi-Fi network (password: `password`).
  - Navigate to `http://192.168.4.1`. The page comes pre-filled with default credentials for quick setup.
- **Dynamic Wi-Fi Scanning**:
  - The provisioning page includes a "Scan for Networks" button with an improved UI that gracefully handles long network names.
- **Enterprise-Grade SSL/TLS Security**:
  - **Automatic Certificate Management**: ThingsBoard/Mosquitto CA certificate automatically initialized on boot
  - **MQTTS Protocol**: Secure MQTT connections on port 8883 with full SSL/TLS encryption
  - **Certificate Validation**: Complete CA certificate chain verification with expiration checking
  - **Multi-Namespace Storage**: Certificates stored in dedicated `security` NVS namespace with fallback support
  - **Production Security**: No hardcoded certificates, proper certificate rotation support
- **Enhanced LED Status Indicator**:
  - The onboard ARGB LED (GPIO 48) provides detailed visual indication of device status:
    - **White:** Provisioning mode active
    - **Blue:** Connecting to Wi-Fi network
    - **Green:** Fully connected to Wi-Fi and MQTTS with SSL/TLS encryption
    - **Red:** Connection error (Wi-Fi or MQTTS SSL/TLS failure)
    - **Brightness:** Configurable (0-255, default 50)
- **Real-Time Telemetry**:
  - **Temperature Monitoring**: Built-in ESP32-S3 sensor (Range: -10°C ~ 80°C, ±1°C accuracy)
  - **System Metrics**: RSSI, heap memory, uptime tracking
  - **Transmission**: JSON payload every 5 seconds over secure MQTTS
  - **Current Memory**: ~323KB free heap at startup

## 🚀 Quick Start (5 Minutes)

### For Immediate Testing:
1. **Flash Firmware**: `idf.py -p /dev/ttyUSB0 flash monitor`
2. **First Boot**: Device automatically starts in provisioning mode (no credentials stored)
3. **Connect to Wi-Fi**: Join `ESP32-Provisioning` network (password: `password`)
4. **Configure Device**: Navigate to `http://192.168.4.1` and enter your Wi-Fi credentials
5. **Smart Boot**: After restart, device connects directly to stored Wi-Fi (no provisioning needed)
6. **LED Status**: Green = fully connected with SSL/TLS encryption

### Smart Device Behavior:
- **First Boot**: Automatic provisioning mode (no Wi-Fi credentials stored)
- **After Configuration**: Smart boot directly to Wi-Fi connection (credentials stored)
- **Certificate Management**: CA certificate automatically initialized once, reused on subsequent boots
- **Reset to Provisioning**: Use `idf.py erase-flash` to clear credentials and return to provisioning mode

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

## Secure MQTTS Broker Setup (Docker)

Deploy a production-ready **MQTTS broker** with SSL/TLS encryption using Docker. This setup matches the ESP32 firmware's certificate configuration.

**Prerequisites:** Docker and Docker Compose installed on your server.

### 1. Initial Setup

First, clone this project onto your remote server. Then, create the necessary directory structure for Mosquitto.

```bash
mkdir -p thingsboard/mosquitto/{config,data,log,certs}
```

### 2. Generate TLS Certificates

To use secure MQTTS, you must generate a set of TLS certificates. The following commands will create a simple Certificate Authority (CA) and use it to sign a server certificate.

**Note:** Run these commands from the project root on your remote server.

**a. Generate the CA private key:**
```bash
openssl genrsa -out thingsboard/mosquitto/certs/ca.key 2048
```

**b. Generate the self-signed CA certificate:**
(You can press Enter through all the prompts for company name, etc.)
```bash
openssl req -new -x509 -days 3650 -key thingsboard/mosquitto/certs/ca.key -out thingsboard/mosquitto/certs/ca.crt
```

**c. Generate the server's private key:**
```bash
openssl genrsa -out thingsboard/mosquitto/certs/server.key 2048
```

**d. Generate a certificate signing request (CSR) for the server:**
**Important:** When prompted for the "Common Name", you **must** enter your server's public IP address (e.g., `193.164.4.51`) or its domain name.
```bash
openssl req -new -out thingsboard/mosquitto/certs/server.csr -key thingsboard/mosquitto/certs/server.key
```

**e. Sign the server certificate with your CA:**
```bash
openssl x509 -req -in thingsboard/mosquitto/certs/server.csr -CA thingsboard/mosquitto/certs/ca.crt -CAkey thingsboard/mosquitto/certs/ca.key -CAcreateserial -out thingsboard/mosquitto/certs/server.crt -days 365
```

### 3. Create Secure Configuration File

Create the Mosquitto configuration file using an editor like `nano`:
```bash
nano thingsboard/mosquitto/config/mosquitto.conf
```

Copy and paste the following content into the file. This configuration enables secure MQTTS with SSL/TLS encryption:
```ini
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log
allow_anonymous false
password_file /mosquitto/config/password.txt

# Secured listener on port 8883
listener 8883
cafile /mosquitto/certs/ca.crt
keyfile /mosquitto/certs/server.key
certfile /mosquitto/certs/server.crt
```
Save and exit the editor (in `nano`, press `Ctrl+X`, then `Y`, then `Enter`).

### 3. Create User and Password

Use the `mosquitto_passwd` utility inside a temporary Docker container to create a user and password. This ensures the password file is generated in the correct format.

**Replace `esp32-password` with your own strong, secure password.**

```bash
docker run -it --rm -v $(pwd)/thingsboard/mosquitto/config:/mosquitto/config eclipse-mosquitto:latest \
mosquitto_passwd -c -b /mosquitto/config/password.txt esp32-device esp32-password
```

### 4. Set Secure Permissions

**Important Note**: The number `1883` below is the **Linux User ID** for the Mosquitto service, NOT a port number. The secure MQTTS port is `8883`.

The Mosquitto Docker container runs as a non-root user with ID `1883` for security. Configuration files should be owned by `root` (read-only access), while data/log directories must be writable by the mosquitto user (ID `1883`).

Run these commands to set the correct, secure ownership and permissions:

```bash
# Set ownership of config files to root for security (read-only access)
sudo chown root:root thingsboard/mosquitto/config/mosquitto.conf
sudo chown root:root thingsboard/mosquitto/config/password.txt

# Set ownership of data/log directories to mosquitto user (ID: 1883, NOT port number!)
# Note: 1883 = Linux User ID, 8883 = MQTTS Port Number
sudo chown -R 1883:1883 thingsboard/mosquitto/data
sudo chown -R 1883:1883 thingsboard/mosquitto/log
```

After creating the password file, you may see a warning that the file owner is not root. The commands above fix this.

### 5. Start the Broker

You can now start the secure Mosquitto broker in detached mode.

```bash
docker compose up -d
```

### 6. Verify and Troubleshoot

Check that the container is running and not stuck in a restart loop.
```bash
docker ps
```
To view the live logs from the Mosquitto broker:
```bash
docker compose logs -f mosquitto
```
To stop the broker:
```bash
docker compose down
```

### 7. Configure ESP32 for MQTTS

Configure your ESP32 to connect to the secure MQTTS broker:
1.  Connect to the **`ESP32-Provisioning`** Wi-Fi network (password: `password`)
2.  Navigate to `http://192.168.4.1`
3.  Enter your Wi-Fi credentials
4.  Configure MQTTS settings:
    *   **Host:** Your server's IP address (e.g., `193.164.4.51`)
    *   **Port:** `8883` (MQTTS secure port)
    *   **Username:** `esp32-device`
    *   **Password:** The secure password you created
5.  Click **Connect** and watch the LED turn green for successful MQTTS connection

**Note:** The ESP32 firmware automatically uses the correct CA certificate for SSL/TLS validation.

## Frequently Asked Questions (FAQ)

### Q: Why do I see "1883" in the setup commands when the port is 8883?
**A:** These are completely different numbers:
- **1883** = Linux User ID for the Mosquitto service inside Docker containers
- **8883** = Network port number for secure MQTTS connections

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