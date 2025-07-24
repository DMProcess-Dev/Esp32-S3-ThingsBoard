# Mosquitto MQTTS Broker Setup (Archived)

**Note:** This documentation is archived as the project no longer uses Mosquitto as its MQTT broker. The information is kept for historical reference only.

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
- **MQTTS** (port 8883) = Encrypted with SSL/TLS certificates (what this project used to use)

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