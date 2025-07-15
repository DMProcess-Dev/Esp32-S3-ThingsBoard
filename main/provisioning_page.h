#pragma once

static const char provisioning_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Wi-Fi Provisioning</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
    :root {
        --bg-color: #f2f2f2;
        --fg-color: #ffffff;
        --text-color: #333333;
        --input-border-color: #cccccc;
        --primary-color: #007aff;
        --primary-hover-color: #0056b3;
        --secondary-color: #555555;
        --shadow-color: rgba(0, 0, 0, 0.1);
        --separator-color: #eeeeee;
    }

    @media (prefers-color-scheme: dark) {
        :root {
            --bg-color: #1c1c1e;
            --fg-color: #2c2c2e;
            --text-color: #ffffff;
            --input-border-color: #444444;
            --primary-color: #0a84ff;
            --primary-hover-color: #0060df;
            --secondary-color: #8e8e93;
            --shadow-color: rgba(0, 0, 0, 0.3);
            --separator-color: #3a3a3c;
        }
    }

    body {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
        background-color: var(--bg-color);
        color: var(--text-color);
        display: flex;
        justify-content: center;
        align-items: center;
        min-height: 100vh;
        margin: 0;
        padding: 1rem;
        box-sizing: border-box;
    }
    .container {
        background-color: var(--fg-color);
        padding: 2rem;
        border-radius: 0.75rem;
        box-shadow: 0 10px 25px var(--shadow-color);
        width: 100%;
        max-width: 800px;
        display: flex;
        gap: 2rem;
    }
    .form-container, .wifi-list-container {
        flex: 1;
    }
    h1, h2 {
        text-align: center;
        margin-bottom: 1.5rem;
    }
    label {
        display: block;
        font-weight: 600;
        margin-bottom: 0.5rem;
    }
    input[type="text"], input[type="password"] {
        width: 100%;
        padding: 0.75rem;
        border: 1px solid var(--input-border-color);
        border-radius: 0.375rem;
        box-sizing: border-box;
        margin-bottom: 1rem;
        font-size: 1rem;
        background-color: var(--bg-color);
        color: var(--text-color);
    }
    input[type="submit"] {
        width: 100%;
        background-color: var(--primary-color);
        color: white;
        padding: 0.85rem;
        border: none;
        border-radius: 0.375rem;
        font-size: 1rem;
        font-weight: 600;
        cursor: pointer;
        transition: background-color 0.2s;
    }
    input[type="submit"]:hover {
        background-color: var(--primary-hover-color);
    }
    #scan-button {
        width: 100%;
        background-color: var(--secondary-color);
        color: white;
        padding: 0.75rem;
        border: none;
        border-radius: 0.375rem;
        font-size: 1rem;
        cursor: pointer;
        margin-bottom: 1rem;
    }
    #wifi-list {
        list-style: none;
        padding: 0;
        margin: 0;
        height: 200px;
        overflow-y: auto;
        border: 1px solid var(--input-border-color);
        border-radius: 0.375rem;
    }
    #wifi-list li {
        padding: 0.75rem;
        border-bottom: 1px solid var(--separator-color);
        cursor: pointer;
        display: flex;
        justify-content: space-between;
        align-items: center;
    }
    #wifi-list li:last-child {
        border-bottom: none;
    }
    #wifi-list li:hover {
        background-color: var(--bg-color);
    }
    .wifi-signal {
        font-weight: normal;
        color: var(--secondary-color);
    }
    #status-container {
        margin-top: 1.5rem;
        text-align: center;
    }

    @media (max-width: 768px) {
        .container {
            flex-direction: column;
            padding: 1rem;
        }
    }
</style>
</head>
<body>
    <div class="container">
        <div class="form-container">
            <h1>Configure Device</h1>
            <form action="/connect" method="post">
                <h2>Wi-Fi</h2>
                <label for="ssid">SSID</label>
                <input type="text" id="ssid" name="ssid" required autocomplete="off">
                <label for="password">Password</label>
                <input type="password" id="password" name="password" autocomplete="off">
                <hr>
                <h2>MQTT</h2>
                <label for="mqtt_host">Host</label>
                <input type="text" id="mqtt_host" name="mqtt_host" value="193.164.4.51">
                <label for="mqtt_port">Port</label>
                <input type="text" id="mqtt_port" name="mqtt_port" value="1883">
                <label for="mqtt_user">Username</label>
                <input type="text" id="mqtt_user" name="mqtt_user" autocomplete="off">
                <label for="mqtt_pass">Password</label>
                <input type="password" id="mqtt_pass" name="mqtt_pass" autocomplete="off">
                <input type="submit" value="Connect">
            </form>
            <div id="status-container" style="display: none;">
                <p id="status-message"></p>
            </div>
        </div>
        <div class="wifi-list-container">
            <h2>Available Networks</h2>
            <button id="scan-button">Scan for Networks</button>
            <ul id="wifi-list"></ul>
        </div>
    </div>
<script>
document.addEventListener('DOMContentLoaded', function() {
    const scanButton = document.getElementById('scan-button');
    const wifiList = document.getElementById('wifi-list');
    const ssidInput = document.getElementById('ssid');
    const form = document.querySelector('form');
    const statusContainer = document.getElementById('status-container');
    const statusMessage = document.getElementById('status-message');

    function getAuthModeString(authMode) {
        switch (authMode) {
            case 0: return 'Open';
            case 1: return 'WEP';
            case 2: return 'WPA_PSK';
            case 3: return 'WPA2_PSK';
            case 4: return 'WPA_WPA2_PSK';
            case 5: return 'WPA2_ENTERPRISE';
            case 6: return 'WPA3_PSK';
            case 7: return 'WPA2_WPA3_PSK';
            default: return 'Unknown';
        }
    }

    function fetchWifiNetworks() {
        wifiList.innerHTML = '<li>Scanning...</li>';
        fetch('/api/wifi-scan')
            .then(response => response.json())
            .then(data => {
                wifiList.innerHTML = '';
                if (data.length === 0) {
                    wifiList.innerHTML = '<li>No networks found</li>';
                } else {
                    data.forEach(net => {
                        const listItem = document.createElement('li');
                        listItem.classList.add('wifi-item');
                        listItem.innerHTML = `<span>${net.ssid}</span><span class="wifi-signal">${net.rssi} dBm, ${getAuthModeString(net.authmode)}</span>`;
                        listItem.addEventListener('click', () => {
                            ssidInput.value = net.ssid;
                        });
                        wifiList.appendChild(listItem);
                    });
                }
            })
            .catch(error => {
                console.error('Error fetching Wi-Fi networks:', error);
                wifiList.innerHTML = '<li>Error scanning for networks</li>';
            });
    }

    let statusInterval;

    function pollStatus() {
        fetch('/api/status')
            .then(response => response.json())
            .then(data => {
                switch (data.status) {
                    case 0: // IDLE
                        statusMessage.textContent = 'Idle';
                        break;
                    case 1: // CONNECTING
                        statusMessage.textContent = 'Connecting...';
                        break;
                    case 2: // CONNECTED
                        statusMessage.textContent = 'Success! Device connected.';
                        statusMessage.style.color = 'green';
                        clearInterval(statusInterval);
                        break;
                    case 3: // FAILED
                        statusMessage.textContent = 'Connection Failed. Please check credentials and retry.';
                        statusMessage.style.color = 'red';
                        clearInterval(statusInterval);
                        break;
                }
            })
            .catch(error => {
                console.error('Error fetching status:', error);
                statusMessage.textContent = 'Error fetching status.';
                clearInterval(statusInterval);
            });
    }

    form.addEventListener('submit', function(event) {
        event.preventDefault();
        const formData = new FormData(form);
        const searchParams = new URLSearchParams();
        for (const pair of formData) {
            searchParams.append(pair[0], pair[1]);
        }

        fetch('/connect', {
            method: 'POST',
            body: searchParams,
        }).then(response => {
            if (response.ok) {
                statusContainer.style.display = 'block';
                statusInterval = setInterval(pollStatus, 2000);
            }
        });
    });

    scanButton.addEventListener('click', fetchWifiNetworks);

    // Initial scan on page load
    fetchWifiNetworks();
});
</script>
</body>
</html>
)rawliteral";