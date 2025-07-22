# 📊 ThingsBoard Dashboard Creation Guide

Create professional, real-time IoT dashboards for your ESP32-S3 telemetry data in under 10 minutes.

## 🎯 What You'll Build

A polished ThingsBoard dashboard featuring:
- **🌡️ Real-time Temperature Gauge** with color-coded zones
- **📶 Wi-Fi Signal Strength Meter** with visual indicators
- **💾 System Health Monitoring** (memory + uptime)
- **📈 Historical Data Charts** with trend analysis
- **⚡ Auto-refreshing every 5 seconds** matching your ESP32 telemetry

## ✅ Prerequisites

Before starting, ensure you have:
- ESP32-S3 device flashed with this project's firmware
- Device connected to Wi-Fi and sending telemetry data
- Access to ThingsBoard server at `http://193.164.4.51:8080`
- ThingsBoard login credentials: `tenant@thingsboard.org` / `tenant`

### 🔍 Verify Your Setup

Your ESP32 should be sending this telemetry data every 5 seconds:
```json
{
  "temperature": 25.6,    // °C from built-in sensor
  "rssi": -45,           // dBm Wi-Fi signal strength
  "heap": 245760,        // bytes free memory  
  "uptime": 120          // seconds since boot
}
```

## 🚀 Quick Start (5 Minutes)

### Step 1: Access ThingsBoard
1. Open browser and navigate to `http://193.164.4.51:8080`
2. Login with credentials: `tenant@thingsboard.org` / `tenant`
3. You should see the ThingsBoard main dashboard

### Step 2: Verify Device Connection
1. Click **"Devices"** in the left sidebar
2. Look for your ESP32 device (should show green "Active" indicator)
3. Click on your device name
4. Select **"Latest telemetry"** tab
5. **✅ Verify**: You should see `temperature`, `rssi`, `heap`, and `uptime` updating every 5 seconds

### Step 3: Create Your Dashboard
1. Click **"Dashboards"** in left sidebar
2. Click **"+"** button → **"Create new dashboard"**
3. Name: **"ESP32-S3 Telemetry Monitor"**
4. Click **"Add"** to create
5. Click **"Open dashboard"** → **"Edit"** (pencil icon)

🎉 **Ready to add widgets!** Continue to the detailed configuration section below.

### 🚀 Pro Option: Import Custom Widget

For the ultimate professional experience, import our custom **ESP32-S3 System Status** widget using one of three methods:

#### Method 1: Individual Widget Import (Recommended)
1. **Download**: Use the `esp32_system_status_widget_individual.json` file from your project
2. **Import**: Widget Library → "+" → "Import widget" → Select the JSON file
3. **Add to Dashboard**: In edit mode → "Add widget" → "ESP32-S3 System Status"
4. **Configure**: Select your ESP32 device → All telemetry data will auto-populate

#### Method 2: Widget Bundle Import
1. **Download**: Use the `esp32_system_status_widget_bundle.json` file from your project
2. **Import**: Widget Library → "Widget bundles" → "+" → "Import widgets bundle" → Select the JSON file
3. **Add to Dashboard**: In edit mode → "Add widget" → Navigate to "ESP32 IoT Widgets" bundle → "ESP32-S3 System Status"
4. **Configure**: Select your ESP32 device → All telemetry data will auto-populate

#### Method 3: Manual Creation (If Import Fails)
If the JSON import methods don't work with your ThingsBoard version:

1. **Follow Manual Guide**: Use the comprehensive [Manual Widget Creation Guide → MANUAL_WIDGET_CREATION.md](./MANUAL_WIDGET_CREATION.md)
2. **Copy-Paste Ready**: Complete HTML, CSS, and JavaScript code provided
3. **Step-by-Step**: Detailed instructions with screenshots and troubleshooting

### Widget Features
This single widget replaces all individual widgets with a stunning, professional display featuring:
- **🌡️ Animated temperature gauge** with color zones and needle movement
- **📶 5-bar signal strength** indicator with real-time updates
- **💾 Progress bar memory** usage with gradient colors
- **⏱️ Multi-segment uptime** display (days/hours/minutes)
- **🎨 Glass-morphism design** with gradient background and blur effects

---

## 🎛️ Detailed Widget Configuration

### 🌡️ Temperature Gauge Widget

Create a professional temperature display with color-coded zones:

**Widget Setup:**
1. **Add Widget**: Click **"+ Add widget"**
2. **Widget Bundle**: Select **"Analog gauges"**
3. **Widget Type**: Choose **"Radial gauge"**
4. **Add Datasource**: Click **"+ Add datasource"**

**Data Configuration:**
- **Type**: "Entity"
- **Entity**: Select your ESP32 device
- **Attribute**: Select "Latest telemetry"
- **Key**: `temperature`
- **Label**: "Temperature"

**Gauge Settings (Advanced tab):**
```javascript
// Gauge Configuration
Min value: 0
Max value: 60
Units: "°C"
Show value: true
Show min/max: true

// Color Ranges
Green zone: 0-30°C    (Normal operating range)
Yellow zone: 30-45°C  (Warm - caution zone)
Red zone: 45-60°C     (Hot - alert zone)
```

**Appearance:**
- **Title**: "Temperature (°C)"
- **Show title**: ✅
- **Title color**: #333333
- **Background color**: #ffffff

### 📶 RSSI Signal Strength Widget

Monitor Wi-Fi connectivity with visual signal strength:

**Widget Setup:**
1. **Add Widget**: Click **"+ Add widget"**
2. **Widget Bundle**: Select **"Analog gauges"** 
3. **Widget Type**: Choose **"Linear gauge"**

**Data Configuration:**
- **Entity**: Your ESP32 device
- **Key**: `rssi`
- **Label**: "Signal Strength"

**Gauge Settings:**
```javascript
// RSSI Configuration
Min value: -100
Max value: -20
Units: "dBm"

// Signal Quality Zones
Red zone: -100 to -80 dBm    (Poor signal)
Yellow zone: -80 to -50 dBm  (Fair signal) 
Green zone: -50 to -20 dBm   (Excellent signal)
```

**Appearance:**
- **Title**: "Wi-Fi Signal Strength"
- **Description**: "Signal quality indicator"

### 💾 System Health Widgets

Monitor your ESP32's system performance:

#### Memory Usage Widget
**Widget Type**: "Digital gauges" → "Simple card"
- **Key**: `heap`
- **Title**: "Free Memory"
- **Units**: Configure to show KB (divide by 1024)
- **Value format**: `${heap/1024:.1f} KB`

#### Uptime Widget  
**Widget Type**: "Cards" → "Simple card"
- **Key**: `uptime`
- **Title**: "System Uptime"
- **Value format**: Convert seconds to hours/minutes
- **Function**: `${Math.floor(uptime/3600)}h ${Math.floor((uptime%3600)/60)}m`

### 📈 Historical Data Charts

Add trend analysis with time-series charts:

**Widget Setup:**
1. **Widget Bundle**: "Charts"
2. **Widget Type**: "Timeseries - Flot"
3. **Add multiple data keys**: `temperature`, `rssi`

**Chart Configuration:**
- **Time window**: "Last 1 hour"
- **Real-time**: ✅ Enable
- **Aggregation**: "None" (for real-time data)
- **Y-axis**: Separate axes for temperature (°C) and RSSI (dBm)

---

## 🎨 Dashboard Polish & Layout

### Professional Layout Design

**Grid Layout (2x3):**
```
┌─────────────────┬─────────────────┬─────────────────┐
│   Temperature   │  RSSI Signal    │  Memory Usage   │
│     Gauge       │     Meter       │     Card        │
├─────────────────┼─────────────────┼─────────────────┤
│           Historical Charts                        │
│         (Temperature & RSSI Trends)               │
└─────────────────────────────────────────────────────┘
```

### Color Scheme & Branding

**Recommended Colors:**
- **Background**: `#f8f9fa` (Light gray)
- **Widget backgrounds**: `#ffffff` (White)
- **Primary accent**: `#007bff` (Blue)
- **Success**: `#28a745` (Green)
- **Warning**: `#ffc107` (Yellow)
- **Danger**: `#dc3545` (Red)

### Dashboard Settings

**Global Configuration:**
1. **Time window**: "Real-time - Last 1 hour"
2. **Refresh interval**: 5 seconds (matches ESP32 telemetry)
3. **Mobile layout**: Enable responsive design
4. **Auto-scale**: ✅ Enable for all widgets

**Save Your Work:**
1. Click **"Apply changes"** (top right)
2. Click **"Save"** to persist dashboard
3. **Exit edit mode** to view final result

---

## 🧪 Testing Your Dashboard

### Functionality Verification

**Real-time Updates:**
- **Temperature**: Touch ESP32 chip → should see temperature rise
- **RSSI**: Move device around → signal strength should change
- **Memory**: Monitor heap usage during operation
- **Uptime**: Should increment every second

**Dashboard Features:**
- **Auto-refresh**: Verify widgets update every 5 seconds
- **Mobile view**: Test on phone/tablet
- **Time range**: Change historical chart time windows
- **Export**: Test dashboard export/import functionality

### Performance Validation

**Expected Behavior:**
- ✅ Widgets update smoothly without flickering
- ✅ No data gaps or missing telemetry points
- ✅ Responsive layout on different screen sizes
- ✅ Color-coded alerts trigger at correct thresholds

---

## 🔧 Troubleshooting Guide

### Common Issues & Solutions

#### ❌ "No data available" or blank widgets

**Possible Causes:**
1. ESP32 not connected to ThingsBoard
2. Wrong device selected in widget
3. Incorrect telemetry key names

**Solutions:**
```bash
# Check ESP32 serial output
idf.py monitor

# Look for these messages:
✅ "MQTT_EVENT_CONNECTED"
✅ "Got IP: xxx.xxx.xxx.xxx"  
❌ "MQTT_EVENT_ERROR" or "Connection failed"
```

**ThingsBoard Verification:**
1. Devices → Your ESP32 → "Latest telemetry" tab
2. Should see data updating every 5 seconds
3. Verify keys: `temperature`, `rssi`, `heap`, `uptime`

#### ❌ Widgets not updating in real-time

**Check Dashboard Settings:**
- Time window: "Real-time" not "Last X hours"
- Refresh interval: 5 seconds or less
- Auto-refresh: Enabled

#### ❌ Temperature gauge showing unrealistic values

**ESP32 Temperature Sensor Notes:**
- Range: Approximately 20-50°C during normal operation
- Higher values (40-50°C) are normal during processing
- Values above 60°C may indicate overheating

#### ❌ RSSI values seem incorrect

**Wi-Fi Signal Strength Reference:**
- -30 dBm: Excellent (very close to router)
- -50 dBm: Good signal strength
- -70 dBm: Fair signal strength  
- -80 dBm: Poor signal strength
- -90 dBm: Very poor signal

### Recovery Procedures

**Reset ESP32 to Provisioning Mode:**
```bash
idf.py erase-flash
idf.py flash monitor
```

**Clear Dashboard and Start Over:**
1. Dashboards → Your dashboard → Delete
2. Follow Quick Start guide from Step 3

---

## ⚡ Pro Tips & Advanced Features

### Dashboard Optimization

**Performance Tips:**
- **Limit widgets**: Too many widgets can slow performance
- **Aggregate data**: Use aggregation for historical charts
- **Reasonable time windows**: Don't load months of data at once
- **Mobile-first**: Design widgets to work on small screens

### Advanced Customization

**Custom Widget Functions:**
```javascript
// Convert heap bytes to MB with 1 decimal
return Math.round(value / 1024 / 1024 * 10) / 10;

// Format uptime as human-readable
var hours = Math.floor(value / 3600);
var minutes = Math.floor((value % 3600) / 60);
return hours + "h " + minutes + "m";

// Temperature with emoji indicators
if (value < 30) return value + "°C ❄️";
else if (value < 40) return value + "°C 🌡️";  
else return value + "°C 🔥";
```

### Alarm Configuration

**Temperature Alert Setup:**
1. **Devices** → Your ESP32 → **"Alarms"** tab
2. **Create alarm**: "High Temperature"
3. **Condition**: `temperature > 50`
4. **Severity**: "Critical"
5. **Action**: Email notification (optional)

**RSSI Alert Setup:**
1. **Create alarm**: "Poor Signal"
2. **Condition**: `rssi < -80`
3. **Severity**: "Warning" 
4. **Clear condition**: `rssi > -70`

### User Access Management

**Create Read-Only Dashboard Access:**
1. **Tenants** → **"Customers"** → **"+ Add customer"**
2. **Customer users** → **"+ Add user"** 
3. **Dashboard permissions** → Assign specific dashboards
4. **Perfect for**: Stakeholder access without admin privileges

---

## 🎯 Expected Final Result

Your completed dashboard should feature:

**Visual Elements:**
- 🌡️ **Temperature gauge**: Smooth needle movement, color zones
- 📶 **RSSI meter**: Bar-style indicator with signal quality colors  
- 💾 **System cards**: Clean digital displays for memory/uptime
- 📈 **Time series**: Multi-line chart showing trends

**Functionality:**
- ⚡ **Real-time updates**: All widgets refresh every 5 seconds
- 📱 **Mobile responsive**: Works perfectly on phone/tablet
- 🎨 **Professional styling**: Consistent colors and layout
- ⏰ **Historical data**: Ability to view past trends

**Data Accuracy:**
- Temperature readings from ESP32-S3 internal sensor
- Wi-Fi signal strength from your network connection  
- Live memory usage showing available heap
- Accurate uptime counter from device boot

🎉 **Congratulations!** You now have a professional IoT dashboard showcasing your ESP32-S3 telemetry data in real-time.

---

## 📚 Additional Resources

- **ThingsBoard Documentation**: https://thingsboard.io/docs/
- **Widget Development**: Custom widget creation guides
- **ESP32-S3 Technical Reference**: Sensor specifications and limits
- **MQTT Protocol**: Understanding telemetry data transmission

For questions or improvements to this guide, check the project repository issues section.

---

## 🎨 Custom Widget Deep Dive

### ESP32-S3 System Status Widget Features

The included custom widget (`esp32_system_status_widget.json`) is a production-ready, professional IoT dashboard component designed specifically for your ESP32-S3 telemetry data.

#### 🏗️ Technical Architecture

**Modern Design System:**
- **Glass-morphism UI**: Translucent card with backdrop blur effects
- **Gradient Background**: Professional blue-to-purple gradient
- **Responsive Grid**: Adapts from desktop (2x2) to mobile (1x4) layout
- **Typography Hierarchy**: Clear information priority with multiple text sizes
- **Color Psychology**: Intuitive green/yellow/red status indicators

**Real-time Animations:**
- **Temperature Gauge**: Smooth needle rotation with arc progress
- **Signal Bars**: Dynamic 5-bar indicator with color transitions
- **Memory Progress**: Animated gradient fill with easing
- **Status Indicators**: Pulsing connection dots and live timestamps

#### 🛠️ Widget Customization

**Settings Panel (Configurable via UI):**
```javascript
Temperature Thresholds:
- Normal: < 30°C (Green zone)
- Warning: 30-45°C (Yellow zone) 
- Critical: > 45°C (Red zone)

Signal Quality Zones:
- Excellent: > -50 dBm (5 bars, green)
- Good: -50 to -60 dBm (4 bars, green)
- Fair: -60 to -70 dBm (3 bars, yellow)
- Poor: < -80 dBm (1-2 bars, red)

Memory Settings:
- Total ESP32 Memory: 320KB (configurable)
- Warning Threshold: < 40% free
- Critical Threshold: < 20% free
```

#### 📱 Responsive Behavior

**Desktop (≥768px):**
- Temperature gauge: Large semicircle with prominent needle
- Grid layout: 2x2 with temperature taking 2 rows
- All metrics visible simultaneously

**Mobile (<768px):**
- Stacked layout: Single column, optimal for scrolling
- Compact gauges: Smaller but still fully functional
- Touch-optimized: Appropriate hit targets and spacing

#### 🔧 Import Troubleshooting

**Common Import Issues:**

❌ **"Invalid widget format" error**
- **Solution**: Ensure the JSON file is not corrupted during download
- **Check**: File should be exactly 21KB and contain valid JSON

❌ **Widget appears but shows "No data"**
- **Verification Steps**:
  1. Device is sending telemetry: Check "Latest telemetry" tab
  2. Correct data keys: Must be `temperature`, `rssi`, `heap`, `uptime`
  3. Widget datasource: Ensure ESP32 device is selected

❌ **Styling looks broken**
- **Browser Support**: Requires modern browser (Chrome 88+, Firefox 87+, Safari 14+)
- **Clear Cache**: Hard refresh (Ctrl+F5) to reload CSS

#### 🎯 Performance Optimization

**Optimized for IoT Data Rates:**
- **Smooth animations**: 60fps gauge movements without stuttering
- **Memory efficient**: Minimal DOM manipulation and CSS transitions
- **Real-time updates**: Handles 5-second telemetry intervals perfectly
- **Battery friendly**: CSS animations use hardware acceleration

**Best Practices:**
- **Single widget**: Replace multiple basic widgets for better performance
- **Real-time timewindow**: Set to "Last 1 hour" for optimal responsiveness
- **Auto-refresh**: 5 seconds matches ESP32 telemetry rate

#### 🔮 Future Enhancements

The widget architecture supports easy extension for additional features:

**Planned Features:**
- **Historical mini-charts**: Sparklines showing recent trends
- **Alert animations**: Visual notifications for threshold breaches
- **Theme variations**: Dark mode, high contrast, colorblind-friendly
- **Additional metrics**: Add new sensors with minimal code changes

**Customization API:**
The widget exposes settings schema for easy customization without code changes. All thresholds, colors, and display options are configurable through ThingsBoard's widget settings panel.

---

### 📊 Widget vs Individual Components Comparison

| Feature | Individual Widgets | Custom ESP32 Widget |
|---------|-------------------|-------------------|
| **Screen Space** | 4+ separate widgets | 1 unified widget |
| **Visual Cohesion** | Mixed styles | Professional consistency |
| **Performance** | Multiple DOM updates | Single optimized update |
| **Mobile Experience** | Poor layout | Responsive design |
| **Customization** | Basic settings | Advanced configuration |
| **Animations** | Static displays | Smooth transitions |
| **Professional Appeal** | Good | Exceptional |

The custom widget provides enterprise-grade visualization that transforms your ESP32 data into a professional IoT monitoring solution.