# ESP32-S3 ThingsBoard Complete Setup Guide

**Complete end-to-end setup from device creation to live dashboard with alerts in 20 minutes.**

## Prerequisites

- ESP32-S3 firmware flashed and running
- ThingsBoard server access: `http://193.164.4.51:8080`
- Login credentials: `tenant@thingsboard.org` / `tenant`

---

## Phase 1: Device Creation & Access Token

### 1. Create New Device

1. **Login** to ThingsBoard: `http://193.164.4.51:8080`
2. **Navigate** to: `http://193.164.4.51:8080/entities/devices`
3. **Click** the **"+"** icon to Add Device
4. **Device Name**: Enter `ESP32-TEST` (or your preferred name)
5. **Customer Assignment**: Optional - leave default or assign to customer
6. **Click** **"Add"** to create device

### 2. Get Access Token

After device creation, the **Connectivity** dialog appears automatically:

1. **Copy** the access token from the command shown:
   ```bash
   mosquitto_pub -d -q 1 -h 193.164.4.51 -p 1883 -t v1/devices/me/telemetry \
   -u "Rx00r4fahdwhoFyungoN" -m "{temperature:25}"
   ```
   
2. **Access Token**: `Rx00r4fahdwhoFyungoN` (example - yours will be different)
3. **Save** this token - you'll need it for ESP32 provisioning

### 3. Test Connectivity (Optional)

If you have `mosquitto_pub` installed, test connection:

```bash
mosquitto_pub -d -q 1 -h 193.164.4.51 -p 1883 -t v1/devices/me/telemetry \
-u "YOUR_ACCESS_TOKEN" -m "{temperature:25}"
```

**Expected Output:**
```
Client null sending CONNECT
Client null received CONNACK (0)
Client null sending PUBLISH (d0, q1, r0, m1, 'v1/devices/me/telemetry', ... (16 bytes))
Client null received PUBACK (Mid: 1, RC:0)
Client null sending DISCONNECT
```

**Verification**: Check device page - you'll see:
```
State: Inactive
Latest telemetry:
Time: 2025-07-24 13:11:40
Key: temperature
Value: 25
```

---

## Phase 2: ESP32 Provisioning

### 1. Connect to ESP32 Provisioning

1. **Connect** to Wi-Fi network: `ESP32-Provisioning` (password: `password`)
2. **Navigate** to: `http://192.168.4.1`
3. **Configure** Wi-Fi and MQTT settings in the provisioning page

### 2. Configure MQTT Settings

**In the provisioning page:**
- **MQTT Host**: `193.164.4.51`
- **MQTT Port**: `1883` (for MQTT) or `8883` (for MQTTS)
- **Access Token**: Paste the token from Step 1.2
- **Configure** your Wi-Fi credentials

### 3. Complete Provisioning

1. **Submit** the configuration
2. **ESP32 reboots** and connects to your Wi-Fi
3. **LED Status**: 
   - Blue: Connecting to Wi-Fi
   - Green: Connected and transmitting data
   - Red: Connection error

### 4. Verify Data Flow

1. **Return** to ThingsBoard device page
2. **Check "Latest telemetry"** tab
3. **Confirm** data updates every 5 seconds:
   - `temperature` (°C)
   - `rssi` (dBm) 
   - `heap` (bytes)
   - `uptime` (seconds)

---

## Phase 3: Professional Dashboard Creation

### 1. Import Custom Widget

1. **Navigate** to: **Resources → Widgets Library → Widgets**
2. **Click** the **"+"** button
3. **Select** **"Import widget"**
4. **Browse** and select: `esp32_system_status_widget_individual.json`
5. **Import** the widget successfully

### 2. Create Dashboard

1. **Go** to **Dashboards** from sidebar
2. **Click** **"Create new dashboard"**
3. **Title**: Enter dashboard name (e.g., "ESP32-S3 Monitor")
4. **Click** **"Add"** to create
5. **Open** the dashboard and click **"Edit"** (pencil icon)

### 3. Add Custom Widget

1. **Click** **"Add widget"** button
2. **Find** and select the imported **"ESP32-S3 System Status"** widget
3. **Configure** widget settings:
   - **Time window**: "Last 10 minutes"
   - **Aggregation**: "None"
   - **Add datasource**: Select your ESP32 device (ESP32-TEST)
   - **Add series**: Select all telemetry keys:
     - `rssi`
     - `temperature` 
     - `heap`
     - `uptime`
4. **Click** **"Add"** to add widget to dashboard

### 4. Finalize Dashboard

1. **Position** and resize widget as needed
2. **Apply changes** and **Save** dashboard
3. **Exit** edit mode

**Result**: Professional glass-morphism dashboard with:
- Animated temperature gauge with needle movement
- 5-bar Wi-Fi signal strength indicator  
- Memory usage progress bar
- Multi-segment uptime display (days/hours/minutes)

---

## Phase 4: Advanced Monitoring & Alerts

### 1. Rule Chain Setup

**Option A: Import Rule Chain (if available)**
If you have `rssi_alarm_chain.json`:
1. **Rule Chains** → **"+"** → **"Import rule chain"**
2. **Select** the JSON file and import
3. **Set as root** rule chain

**Option B: Manual Rule Chain Creation**
Follow the detailed steps in `thingsboard_rule_chain_setup.md` to create:
- RSSI threshold filter (< -70 dBm)
- Alarm creation and clearing
- Mattermost webhook integration

### 2. Mattermost Webhook Configuration

1. **Create** Mattermost incoming webhook in your channel
2. **Copy** webhook URL
3. **Configure** in rule chain's "Send to Mattermost" node:
   - **Method**: POST
   - **URL**: Your Mattermost webhook URL
   - **Headers**: `Content-Type: application/json`

### 3. Set Rule Chain as Root

1. **Open** your rule chain
2. **Click** **"Set root"** button in title bar
3. **Alternative**: **Device Profiles** → Select profile → **Rule chain** tab

### 4. Test Alert System

**Trigger Test:**
- Move ESP32 away from Wi-Fi router to reduce RSSI
- Wait for RSSI to drop below -70 dBm
- **Expected**: Mattermost notification with formatted alert

**Alert Format:**
```
🚨 Critical Alarm: Low Signal Strength
• Device: ESP32-TEST
• Time: 24 Jul 2025, 13:15:42 (UTC+3)
• RSSI Value: -75
• Alarm Type: Low Signal Strength  
• Severity: Critical
```

---

## Phase 5: Verification & Testing

### End-to-End Checklist

**✅ Device Setup:**
- Device created in ThingsBoard
- Access token generated and configured
- ESP32 provisioned and connected (green LED)

**✅ Data Flow:**
- Telemetry updating every 5 seconds
- All 4 data keys present: temperature, rssi, heap, uptime
- Values realistic and changing

**✅ Dashboard:**
- Custom widget imported successfully
- Dashboard displays all metrics with live updates
- Professional styling with animations working

**✅ Alerts (Optional):**
- Rule chain configured and set as root
- Mattermost webhook receiving formatted alerts
- Alarms clear when RSSI improves

### Common Issues & Quick Fixes

**❌ No telemetry data:**
- Check ESP32 LED status (should be green)
- Verify access token matches device token
- Confirm Wi-Fi connectivity

**❌ Widget import fails:**
- Use `MANUAL_WIDGET_CREATION.md` for manual setup
- Verify JSON file not corrupted
- Check ThingsBoard version compatibility

**❌ Dashboard not updating:**
- Set time window to "Real-time" 
- Refresh interval ≤ 5 seconds
- Clear browser cache (Ctrl+F5)

**❌ Alerts not triggering:**
- Confirm rule chain set as root
- Check RSSI threshold in rule chain (< -70)
- Verify Mattermost webhook URL

---

## Expected Final Result

**Professional IoT Monitoring System:**
- Real-time ESP32-S3 telemetry dashboard
- Animated gauges and professional styling
- Automated alerts for signal quality issues
- Mobile-responsive design for monitoring anywhere

**Data Ranges:**
- **Temperature**: 25-45°C (ESP32-S3 internal sensor)
- **RSSI**: -20 to -90 dBm (Wi-Fi signal strength)
- **Memory**: ~320KB total, showing free heap
- **Uptime**: Continuous counter from device boot

Your ThingsBoard setup is now complete with professional monitoring and alerting capabilities!

---

## Resources

- **Custom Widget**: `esp32_system_status_widget_individual.json`
- **Manual Widget Guide**: `MANUAL_WIDGET_CREATION.md`
- **Rule Chain Setup**: `thingsboard_rule_chain_setup.md`
- **ThingsBoard Documentation**: https://thingsboard.io/docs/