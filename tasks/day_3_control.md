### 💡 Day 3: Control (RPC) & Polish
*   [ ] **Firmware: RPC Logic**
    *   [ ] In `app_main.c`, subscribe to the ThingsBoard RPC request topic: `v1/devices/me/rpc/request/+`.
    *   [ ] In the MQTT event handler, add logic to parse incoming RPC messages (e.g., `{"method":"setLed", "params":true}`).
    *   [ ] Implement the GPIO toggle logic based on the boolean value of `params`.
    *   [ ] Re-flash both boards with the new firmware.
*   [ ] **ThingsBoard: RPC Widget & Export**
    *   [ ] In your dashboard, add a new widget. Find a "Control" widget like "GPIO Control".
    *   [ ] Configure the widget to target your device and to send the RPC method `setLed` on toggle.
    *   [ ] Test it. Clicking the switch on the dashboard should instantly toggle the LED on the physical board.
    *   [ ] Export your Rule Chain: `thingsboard/rule_chain_demo.json`.
    *   [ ] Export your custom-configured widget: `thingsboard/widget_led.json`.
    *   [ ] Use Postman or `curl` to make a REST API call to fetch device telemetry. Take a screenshot and save it as `screenshots/postman_api_test.png`.
