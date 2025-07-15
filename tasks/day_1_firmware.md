### ⚙️ Day 1: Firmware (Wi-Fi Provisioning & Telemetry)
*   [x] **Wi-Fi Provisioning**:
    *   [x] If no credentials are found in NVS, start the ESP32 in Access Point (AP) mode.
    *   [x] Implement a lightweight web server to serve a provisioning page.
    *   [x] Design a clean, polished HTML page with a form for SSID and password.
    *   [x] Add a feature to scan for and display available Wi-Fi networks.
    *   [x] Handle the form submission, save the credentials to NVS, and restart the device.
    *   [x] If credentials *are* found in NVS, connect to the Wi-Fi in Station (STA) mode.
*   [x] **Telemetry & MQTT**:
    *   [x] Once connected, start the MQTT client (using the `mqtt/tcp` example as a base).
    *   [ ] Implement logic to read built-in sensors: chip temperature, Wi-Fi RSSI, and free heap (`esp_get_free_heap_size()`).
    *   [ ] Add a simple uptime counter (seconds since boot).
    *   [ ] Structure the data into a single JSON payload string: `{"temperature": X, "rssi": Y, "heap": Z, "uptime": U}`.
*   [ ] **Device Configuration**:
    *   [ ] Add two `menuconfig` entries for device-specific tokens: `CONFIG_DEVICE_TOKEN_A` and `CONFIG_DEVICE_TOKEN_B`. Use conditional compilation (`#ifdef`) to select the token at build time.
*   [ ] **Hardware Feedback**:
    *   [ ] Implement logic to blink the on-board LED (GPIO 48) once on each successful MQTT publish.
*   [x] **Crucial Test:** Temporarily point the MQTT client to a public broker (e.g., `broker.hivemq.com`). Use a desktop client like `MQTTX` to subscribe to the topic and verify that your JSON is being sent correctly before involving ThingsBoard.
