### 🚀 Day 2: UI/UX Enhancements & Feature Refinements

Based on initial testing, this set of tasks focuses on improving the user experience of the provisioning flow and adding more robust features.

*   [x] **Provisioning UI/UX:**
    *   [x] Improve the UI/UX of the available Wi-Fi networks list. The current implementation is difficult to scroll and use.
    *   [x] Clear the SSID and password fields when the provisioning page is loaded to prevent browsers from auto-filling stale data. Use the `autocomplete="off"` attribute.
    *   [x] Add a user-friendly notification system to the web page to show the status of the connection attempt (e.g., "Connecting...", "Success!", "Failed: Wrong Password"). This will likely require a status-polling mechanism.
    *   [x] Implement a dark mode for the provisioning page that respects the user's system preferences.

*   [x] **Web Server & Network:**
    *   [x] Handle the `/favicon.ico` request gracefully to prevent 404 errors in the log. A `204 No Content` response is appropriate.
    *   [x] Add `Cache-Control: no-store` headers to the provisioning page response to prevent browsers from caching the page and showing stale data on subsequent visits.

*   [x] **MQTT Configuration:**
    *   [x] Add fields to the provisioning page for configuring the MQTT broker (Host, Port, Username, Password).
    *   [x] Save these MQTT settings to NVS along with the Wi-Fi credentials.
    *   [x] Use the saved settings when initializing the MQTT client.

*   [x] **Hardware Feedback:**
    *   [x] Utilize the onboard ARGB LED for status indication.
        *   **Provisioning Mode:** White, pulsing
        *   **Connecting to Wi-Fi:** Blue, blinking
        *   **Wi-Fi Connected / MQTT Connecting:** Green, blinking
        *   **MQTT Connected:** Solid Green
        *   **Error State (Connection Failed):** Solid Red
