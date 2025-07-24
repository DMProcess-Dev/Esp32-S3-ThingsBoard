# ESP32-S3 ThingsBoard Dashboard Setup

Professional IoT dashboard for your ESP32-S3 telemetry data - complete setup in 15 minutes.

> **📋 For Complete End-to-End Setup**: See [`Thingsboard_Complete_Setup.md`](../thingsboard/docs/Thingsboard_Complete_Setup.md) for device creation, access tokens, and full workflow.

**This guide assumes:** You already have a ThingsBoard device created and ESP32 sending telemetry data.

## Quick Start

### 1. Access ThingsBoard
- URL: `http://193.164.4.51:8080`
- Login: `tenant@thingsboard.org` / `tenant`

### 2. Verify Device
1. **Devices** → Find your ESP32 device (green "Active" status)
2. **Latest telemetry** tab → Confirm data updates every 5 seconds:
   - `temperature` (°C), `rssi` (dBm), `heap` (bytes), `uptime` (seconds)

### 3. Create Dashboard
1. **Dashboards** → **"+"** → **"Create new dashboard"**
2. Name: **"ESP32-S3 Monitor"** → **"Add"** → **"Open dashboard"** → **"Edit"**

## Professional Widget Setup

### Option A: Import Custom Widget (Recommended)
**Best visual experience with single professional widget**

1. **Download**: [`esp32_system_status_widget_individual.json`](../thingsboard/widgets/esp32_system_status_widget_individual.json) from project
2. **Resources** → **Widgets Library** → **Widgets** → **"+"** → **"Import widget"** → Select JSON file
3. **Dashboard Edit** → **"Add widget"** → **"ESP32-S3 System Status"**
4. **Configure**: Select your ESP32 device → Time window: "Last 10 minutes", Aggregation: "None" → **"Add"**

✅ **Result**: Glass-morphism dashboard with animated temperature gauge, signal bars, memory progress, and uptime display.

### Option B: Standard Widgets
**Quick setup using ThingsBoard's built-in widgets**

**Temperature Gauge:**
- **Add widget** → **"Analog gauges"** → **"Radial gauge"**
- **Entity**: Your ESP32 → **Key**: `temperature`
- **Settings**: Min: 0, Max: 60, Units: "°C"
- **Color ranges**: Green (0-30), Yellow (30-45), Red (45-60)

**Signal Strength:**
- **Add widget** → **"Analog gauges"** → **"Linear gauge"**
- **Entity**: Your ESP32 → **Key**: `rssi`
- **Settings**: Min: -100, Max: -20, Units: "dBm"

**Memory & Uptime:**
- **Add widget** → **"Cards"** → **"Simple card"**
- Memory: Key `heap`, format: `${heap/1024:.1f} KB`
- Uptime: Key `uptime`, format: `${Math.floor(uptime/3600)}h ${Math.floor((uptime%3600)/60)}m`

## Dashboard Settings

**Global Configuration:**
- **Time window**: "Real-time - Last 1 hour"
- **Refresh interval**: 5 seconds
- **Mobile layout**: ✅ Enabled

**Save**: **"Apply changes"** → **"Save"** → Exit edit mode

## Advanced Features

### Alarms Setup
**High Temperature Alert:**
1. **Devices** → Your ESP32 → **"Alarms"** tab
2. **"Create alarm"**: "High Temperature"
3. **Condition**: `temperature > 50`
4. **Severity**: "Critical"

**Poor Signal Alert:**
1. **"Create alarm"**: "Weak Signal"
2. **Condition**: `rssi < -80`
3. **Clear condition**: `rssi > -70`

### Rule Chain Integration
For advanced notifications (Mattermost, email), see [`Thingsboard_Rule_Chain_Setup.md`](./Thingsboard_Rule_Chain_Setup.md).

### User Access
**Read-only Access:**
1. **Customers** → **"+ Add customer"**
2. **Customer users** → **"+ Add user"**
3. **Dashboard permissions** → Assign specific dashboards

## Troubleshooting

**❌ No data displayed:**
- Check device "Latest telemetry" tab
- Verify keys match: `temperature`, `rssi`, `heap`, `uptime`
- Confirm ESP32 LED is green (connected)

**❌ Widgets not updating:**
- Dashboard time window: "Real-time"
- Refresh interval: ≤5 seconds
- Browser refresh (Ctrl+F5)

**❌ Custom widget import fails:**
- Use [`Manual_Widget_Creation.md`](./Manual_Widget_Creation.md) for step-by-step creation
- Verify JSON file not corrupted
- Try individual widget import instead of bundle

## Expected Result

Your completed dashboard features:
- **Real-time updates** every 5 seconds
- **Professional styling** with responsive design
- **Color-coded indicators** for system health
- **Mobile compatibility** for monitoring anywhere

Temperature shows ESP32-S3 internal sensor readings (typically 25-45°C), RSSI displays Wi-Fi signal strength, memory shows available heap, and uptime tracks system stability.

## Resources

- **Complete Setup Guide**: [`Thingsboard_Complete_Setup.md`](../thingsboard/docs/Thingsboard_Complete_Setup.md) - Device creation to live dashboard
- **Custom Widget**: [`esp32_system_status_widget_individual.json`](../thingsboard/widgets/esp32_system_status_widget_individual.json)
- **Manual Creation**: [`Manual_Widget_Creation.md`](./Manual_Widget_Creation.md)
- **Advanced Rules**: [`Thingsboard_Rule_Chain_Setup.md`](./Thingsboard_Rule_Chain_Setup.md)
- **ThingsBoard Docs**: https://thingsboard.io/docs/