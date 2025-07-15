# ESP32-S3 ThingsBoard Demo

This project is a demonstration firmware for an ESP32-S3 microcontroller designed to connect to the ThingsBoard IoT platform. It includes a robust web-based provisioning system to configure Wi-Fi credentials and demonstrates a connection to a public MQTT broker.

## Current Features

- **Web-Based Wi-Fi Provisioning**:
  - On first boot, the device starts in Access Point mode.
  - Connect to the **`ESP32-Provisioning`** Wi-Fi network (password: `password`).
  - Navigate to `http://192.168.4.1`. The page comes pre-filled with default credentials for quick setup.
- **Dynamic Wi-Fi Scanning**:
  - The provisioning page includes a "Scan for Networks" button with an improved UI that gracefully handles long network names.
- **Secure MQTT Connectivity**:
  - After provisioning, the device connects to the configured Wi-Fi network and MQTT broker.
  - **SSL/TLS Support**: Full SSL certificate verification for secure MQTTS connections on port 8883.
  - **Certificate Validation**: Proper CA certificate verification ensures secure communication.
- **Enhanced LED Status Indicator**:
  - The onboard ARGB LED provides a detailed visual indication of the device's status:
    - **White:** Provisioning mode
    - **Blue:** Connecting to Wi-Fi
    - **Yellow:** Wi-Fi connected, but MQTT is disconnected or has an error.
    - **Green:** Fully connected to Wi-Fi and MQTT.
    - **Flashing White:** A telemetry message has just been published.
    - **Red:** Wi-Fi connection failed.

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

## Remote MQTT Broker (Docker) Setup

This project includes a `docker-compose.yml` file to deploy a **secure** Eclipse Mosquitto MQTT broker to a remote server using Docker. The following steps will guide you through the process.

**Note:** These commands assume you are in the project's root directory on your remote server. The newer `docker compose` (with a space) command is used.

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

Copy and paste the following content into the file. This configuration disables anonymous access and points to a password file.
```ini
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log
allow_anonymous false
password_file /mosquitto/config/password.txt
listener 1883
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

The Mosquitto broker runs as a non-root user (ID `1883`), but for security, its configuration files should be owned by `root`. The broker only needs to *read* them. The data and log directories, however, must be *writable* by the `1883` user.

Run these commands to set the correct, secure ownership and permissions:

```bash
# Set ownership of the config files to root for security
sudo chown root:root thingsboard/mosquitto/config/mosquitto.conf
sudo chown root:root thingsboard/mosquitto/config/password.txt

# Set ownership of the data and log directories to the mosquitto user
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

### 7. Re-provision the ESP32

With the broker now requiring authentication, you must re-provision your device with the new credentials:
1.  Connect to the **`ESP32-Provisioning`** Wi-Fi network.
2.  Go to `http://192.168.4.1`.
3.  Enter your Wi-Fi SSID and password.
4.  Enter the MQTT credentials you created in Step 3:
    *   **Username:** `esp32-device`
    *   **Password:** The secure password you chose.
5.  Click **Connect**.