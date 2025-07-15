### ☁️ Day 2: ThingsBoard (Ingestion, Rules, Dashboard)
*   [ ] **AM: Core Setup & Ingestion**
    *   [ ] Get the official ThingsBoard CE `docker-compose.yml` and run `docker-compose up -d`.
    *   [ ] Log in to ThingsBoard at `http://localhost:8080` (Default: `tenant@thingsboard.org` / `tenant`).
    *   [ ] Create two devices: `ESP32-S3-A` and `ESP32-S3-B`.
    *   [ ] For each device, copy its access token from the "Credentials" tab.
    *   [ ] Update your `menuconfig` with the real ThingsBoard tokens for Board A and Board B, then re-flash each board.
    *   [ ] In the ThingsBoard UI, navigate to each device's "Latest Telemetry" tab and confirm the data is arriving every 5 seconds.
*   [ ] **PM: Rule Chain & Alarms**
    *   [ ] Navigate to "Rule Chains" and create a new one: `RSSI Alarm Chain`.
    *   [ ] Drag in a `script` filter node. Use JS `return msg.rssi < -70;` and connect it to the input node via the "Success" relation.
    *   [ ] Drag in a `create alarm` node. Configure it to create a "Critical" alarm. Connect the script node's "True" output to this node.
    *   [ ] **(Optional)** Drag in a `send email` node. Connect the alarm node's "Created" output to it. **Strictly timebox this to 20 minutes.** If it doesn't work, move on. The UI alarm is the key deliverable.
    *   [ ] Enable "Debug mode" on all nodes. Trigger the alarm by shielding a board's antenna and watch the data flow live.
