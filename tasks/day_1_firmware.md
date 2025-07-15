### ⚙️ Day 1: Firmware (Telemetry & MQTT)
*   [ ] Start with the ESP-IDF `mqtt/tcp` example as a base for `main/app_main.c`.
*   [ ] Implement logic to read built-in sensors: chip temperature, Wi-Fi RSSI, and free heap (`esp_get_free_heap_size()`).
*   [ ] Add a simple uptime counter (seconds since boot).
*   [ ] Structure the data into a single JSON payload string: `{"temperature": X, "rssi": Y, "heap": Z, "uptime": U}`.
*   [ ] Configure `menuconfig` for Wi-Fi credentials (SSID & Password).
*   [ ] Add two `menuconfig` entries for device-specific tokens: `CONFIG_DEVICE_TOKEN_A` and `CONFIG_DEVICE_TOKEN_B`. Use conditional compilation (`#ifdef`) to select the token at build time.
*   [ ] Implement logic to blink the on-board LED (GPIO 48) once on each successful MQTT publish.
*   [ ] **Crucial Test:** Temporarily point the MQTT client to a public broker (e.g., `broker.hivemq.com`). Use a desktop client like `MQTTX` to subscribe to the topic and verify that your JSON is being sent correctly before involving ThingsBoard.
