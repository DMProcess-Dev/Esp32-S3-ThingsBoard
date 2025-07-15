
#pragma once

static const char provisioning_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Wi-Fi Provisioning</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
    body {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
        background-color: #f2f2f2;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
    }
    .container {
        background-color: white;
        padding: 2rem;
        border-radius: 0.75rem;
        box-shadow: 0 10px 25px rgba(0, 0, 0, 0.1);
        width: 100%;
        max-width: 400px;
        box-sizing: border-box;
    }
    h1 {
        font-size: 1.5rem;
        color: #333;
        margin-bottom: 1.5rem;
        text-align: center;
    }
    label {
        display: block;
        font-weight: 600;
        color: #555;
        margin-bottom: 0.5rem;
    }
    input[type="text"], input[type="password"] {
        width: 100%;
        padding: 0.75rem;
        border: 1px solid #ccc;
        border-radius: 0.375rem;
        box-sizing: border-box;
        margin-bottom: 1rem;
        font-size: 1rem;
    }
    input[type="submit"] {
        width: 100%;
        background-color: #007aff;
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
        background-color: #0056b3;
    }
    #wifi-list-container {
        margin-top: 2rem;
    }
    #scan-button {
        width: 100%;
        background-color: #555;
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
        max-height: 150px;
        overflow-y: auto;
        border: 1px solid #ccc;
        border-radius: 0.375rem;
    }
    #wifi-list li {
        padding: 0.75rem;
        border-bottom: 1px solid #eee;
        cursor: pointer;
    }
    #wifi-list li:last-child {
        border-bottom: none;
    }
    #wifi-list li:hover {
        background-color: #f0f0f0;
    }
</style>
</head>
<body>
    <div class="container">
        <h1>Configure Wi-Fi</h1>
        <form action="/connect" method="post">
            <label for="ssid">SSID</label>
            <input type="text" id="ssid" name="ssid" required>
            <label for="password">Password</label>
            <input type="password" id="password" name="password">
            <input type="submit" value="Connect">
        </form>
        <div id="wifi-list-container">
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
                        listItem.innerHTML = `<strong>${net.ssid}</strong> (RSSI: ${net.rssi}, Auth: ${getAuthModeString(net.authmode)})`;
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

    scanButton.addEventListener('click', fetchWifiNetworks);

    // Initial scan on page load
    fetchWifiNetworks();
});
</script>
</body>
</html>
)rawliteral";
