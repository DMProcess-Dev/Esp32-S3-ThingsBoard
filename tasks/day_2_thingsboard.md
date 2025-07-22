### ☁️ Day 2: ThingsBoard (Ingestion, Rules, Dashboard)
*   [x] **AM: Core Setup & Ingestion**
    *   [x] Get the official ThingsBoard CE `docker-compose.yml` and run `docker-compose up -d`.
    *   [x] Log in to ThingsBoard at `http://193.164.4.51:8080/login` (Default: `tenant@thingsboard.org` / `tenant`).
    *   [x] Create a device in the ThingsBoard UI (e.g., `ESP32-S3-A`).
    *   [x] For the device, copy its access token from the "Credentials" tab.
    *   [x] **Provision the device**: On first boot, the ESP32 creates a Wi-Fi AP named `ESP32-Provisioning` (password: `password`). Connect to it, navigate to `http://192.168.4.1`, and enter your Wi-Fi credentials, ThingsBoard host (e.g., your computer's IP), and the device access token.
    *   [x] In the ThingsBoard UI, navigate to the device's "Latest Telemetry" tab and confirm that `temperature`, `rssi`, `heap`, and `uptime` data is arriving every 5 seconds.
*   [x] **PM: Rule Chain & Alarms**
    *   [x] Created a rule chain to generate a critical alarm when RSSI is low.
    *   [x] The rule chain sends a detailed notification to a Mattermost webhook.
    *   [x] The alarm is automatically cleared when the signal returns to normal.
    *   [x] For a complete guide, see the new documentation: `thingsboard/thingsboard_rule_chain_setup.md`.