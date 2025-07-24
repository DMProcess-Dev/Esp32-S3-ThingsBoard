# 🛠️ Manual Widget Creation Guide - ESP32-S3 System Status

This guide provides step-by-step instructions to manually create the ESP32-S3 System Status widget in ThingsBoard if import fails.

## 📋 Prerequisites

- ThingsBoard access with widget creation permissions
- ESP32 device sending telemetry data with keys: `temperature`, `rssi`, `heap`, `uptime`

---

## 🚀 Step-by-Step Widget Creation

### Step 1: Create Widget Bundle

1. **Navigate to Widget Library**:
   - Go to **Widget Library** in ThingsBoard
   - Click **"Widget bundles"** tab
   - Click **"+"** button → **"Create new widgets bundle"**

2. **Configure Bundle**:
   - **Title**: `ESP32 IoT Widgets`
   - **Description**: `Professional IoT dashboard widgets for ESP32 devices`
   - Click **"Add"**

### Step 2: Create New Widget

1. **Add Widget to Bundle**:
   - Open your newly created **ESP32 IoT Widgets** bundle
   - Click **"+"** button → **"Create new widget type"**

2. **Widget Configuration**:
   - **Name**: `ESP32-S3 System Status`
   - **Type**: Select **"Timeseries"**
   - **Description**: `Comprehensive system status widget showing temperature, signal strength, memory, and uptime`

### Step 3: Add HTML Template

**Copy and paste this HTML into the "HTML" tab:**

```html
<div class="esp32-status-card">
  <div class="status-header">
    <div class="device-info">
      <h3 class="device-title">ESP32-S3 System Status</h3>
      <div class="status-indicator"></div>
    </div>
    <div class="timestamp"></div>
  </div>
  <div class="status-grid">
    <div class="metric-card primary-metric">
      <div class="metric-header">
        <span class="metric-icon">🌡️</span>
        <span class="metric-title">Temperature</span>
      </div>
      <div class="temperature-gauge">
        <div class="gauge-container">
          <svg class="gauge-svg" viewBox="0 0 200 120">
            <path class="gauge-background" d="M 30 90 A 70 70 0 0 1 170 90" stroke="#e0e0e0" stroke-width="8" fill="none"/>
            <path class="gauge-active" d="M 30 90 A 70 70 0 0 1 170 90" stroke="#4CAF50" stroke-width="8" fill="none"/>
            <line class="gauge-needle" x1="100" y1="90" x2="100" y2="30" stroke="#333" stroke-width="2"/>
            <circle cx="100" cy="90" r="5" fill="#333"/>
          </svg>
        </div>
        <div class="temperature-value">
          <span class="value">--</span>
          <span class="unit">°C</span>
        </div>
        <div class="temperature-status">Normal</div>
      </div>
    </div>
    <div class="metric-card">
      <div class="metric-header">
        <span class="metric-icon">📶</span>
        <span class="metric-title">Wi-Fi Signal</span>
      </div>
      <div class="rssi-display">
        <div class="signal-bars">
          <div class="signal-bar"></div>
          <div class="signal-bar"></div>
          <div class="signal-bar"></div>
          <div class="signal-bar"></div>
          <div class="signal-bar"></div>
        </div>
        <div class="rssi-value">
          <span class="value">--</span>
          <span class="unit">dBm</span>
        </div>
        <div class="rssi-quality">Unknown</div>
      </div>
    </div>
    <div class="metric-card">
      <div class="metric-header">
        <span class="metric-icon">💾</span>
        <span class="metric-title">Free Memory</span>
      </div>
      <div class="memory-display">
        <div class="memory-progress">
          <div class="progress-track">
            <div class="progress-fill"></div>
          </div>
        </div>
        <div class="memory-value">
          <span class="value">--</span>
          <span class="unit">KB</span>
        </div>
        <div class="memory-status">Available</div>
      </div>
    </div>
    <div class="metric-card">
      <div class="metric-header">
        <span class="metric-icon">⏱️</span>
        <span class="metric-title">Uptime</span>
      </div>
      <div class="uptime-display">
        <div class="uptime-segments">
          <div class="time-segment">
            <span class="time-value">0</span>
            <span class="time-label">Days</span>
          </div>
          <div class="time-segment">
            <span class="time-value">0</span>
            <span class="time-label">Hours</span>
          </div>
          <div class="time-segment">
            <span class="time-value">0</span>
            <span class="time-label">Mins</span>
          </div>
        </div>
        <div class="uptime-stability">Starting</div>
      </div>
    </div>
  </div>
  <div class="status-footer">
    <div class="connection-status">
      <span class="connection-dot"></span>
      <span class="connection-text">Connected</span>
    </div>
    <div class="data-rate">Updates: 5s</div>
  </div>
</div>
```

### Step 4: Add CSS Styling

**Copy and paste this CSS into the "CSS" tab:**

```css
.esp32-status-card {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  border-radius: 16px;
  padding: 20px;
  color: white;
  height: 100%;
  box-sizing: border-box;
  box-shadow: 0 8px 32px rgba(0,0,0,0.3);
  position: relative;
  overflow: hidden;
}

.esp32-status-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(255,255,255,0.1);
  backdrop-filter: blur(10px);
  border-radius: 16px;
  z-index: 0;
}

.esp32-status-card > * {
  position: relative;
  z-index: 1;
}

.status-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
  padding-bottom: 15px;
  border-bottom: 1px solid rgba(255,255,255,0.2);
}

.device-info {
  display: flex;
  align-items: center;
  gap: 12px;
}

.device-title {
  margin: 0;
  font-size: 18px;
  font-weight: 600;
  text-shadow: 0 2px 4px rgba(0,0,0,0.3);
}

.status-indicator {
  width: 12px;
  height: 12px;
  border-radius: 50%;
  background: #4CAF50;
  box-shadow: 0 0 10px rgba(76,175,80,0.6);
  animation: pulse 2s infinite;
}

@keyframes pulse {
  0%, 100% { opacity: 1; }
  50% { opacity: 0.5; }
}

.timestamp {
  font-size: 12px;
  opacity: 0.8;
}

.status-grid {
  display: grid;
  grid-template-columns: 2fr 1fr;
  grid-template-rows: 1fr 1fr;
  gap: 15px;
  height: calc(100% - 120px);
}

.metric-card {
  background: rgba(255,255,255,0.15);
  border-radius: 12px;
  padding: 15px;
  backdrop-filter: blur(10px);
  border: 1px solid rgba(255,255,255,0.2);
  transition: all 0.3s ease;
}

.metric-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 20px rgba(0,0,0,0.2);
}

.primary-metric {
  grid-row: 1 / 3;
}

.metric-header {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-bottom: 10px;
}

.metric-icon {
  font-size: 16px;
}

.metric-title {
  font-size: 14px;
  font-weight: 500;
  opacity: 0.9;
}

.temperature-gauge {
  text-align: center;
  height: 100%;
  display: flex;
  flex-direction: column;
  justify-content: center;
}

.gauge-container {
  width: 120px;
  height: 80px;
  margin: 0 auto 15px;
}

.gauge-svg {
  width: 100%;
  height: 100%;
}

.gauge-active {
  transition: stroke-dasharray 0.5s ease;
}

.gauge-needle {
  transition: transform 0.5s ease;
  transform-origin: 100px 90px;
}

.temperature-value {
  margin-bottom: 8px;
}

.temperature-value .value {
  font-size: 24px;
  font-weight: 700;
  text-shadow: 0 2px 4px rgba(0,0,0,0.3);
}

.temperature-value .unit {
  font-size: 16px;
  opacity: 0.8;
  margin-left: 4px;
}

.temperature-status {
  font-size: 12px;
  opacity: 0.8;
  font-weight: 500;
}

.rssi-display {
  text-align: center;
}

.signal-bars {
  display: flex;
  justify-content: center;
  gap: 3px;
  margin-bottom: 10px;
}

.signal-bar {
  width: 8px;
  background: rgba(255,255,255,0.3);
  border-radius: 4px 4px 0 0;
  transition: all 0.3s ease;
}

.signal-bar:nth-child(1) { height: 8px; }
.signal-bar:nth-child(2) { height: 12px; }
.signal-bar:nth-child(3) { height: 16px; }
.signal-bar:nth-child(4) { height: 20px; }
.signal-bar:nth-child(5) { height: 24px; }

.signal-bar.active {
  background: #4CAF50;
  box-shadow: 0 0 8px rgba(76,175,80,0.5);
}

.rssi-value .value {
  font-size: 18px;
  font-weight: 600;
}

.rssi-value .unit {
  font-size: 12px;
  opacity: 0.8;
}

.rssi-quality {
  font-size: 11px;
  opacity: 0.8;
  margin-top: 4px;
}

.memory-display {
  text-align: center;
}

.memory-progress {
  margin-bottom: 12px;
}

.progress-track {
  width: 100%;
  height: 8px;
  background: rgba(255,255,255,0.3);
  border-radius: 4px;
  overflow: hidden;
}

.progress-fill {
  height: 100%;
  background: linear-gradient(90deg, #4CAF50, #2196F3);
  border-radius: 4px;
  transition: width 0.5s ease;
  box-shadow: 0 0 8px rgba(76,175,80,0.3);
  width: 0%;
}

.memory-value .value {
  font-size: 16px;
  font-weight: 600;
}

.memory-value .unit {
  font-size: 12px;
  opacity: 0.8;
}

.memory-status {
  font-size: 11px;
  opacity: 0.8;
  margin-top: 4px;
}

.uptime-display {
  text-align: center;
}

.uptime-segments {
  display: flex;
  justify-content: space-between;
  margin-bottom: 10px;
}

.time-segment {
  flex: 1;
  text-align: center;
}

.time-value {
  display: block;
  font-size: 16px;
  font-weight: 600;
  line-height: 1;
}

.time-label {
  display: block;
  font-size: 10px;
  opacity: 0.8;
  margin-top: 2px;
}

.uptime-stability {
  font-size: 11px;
  opacity: 0.8;
}

.status-footer {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-top: 15px;
  padding-top: 15px;
  border-top: 1px solid rgba(255,255,255,0.2);
  font-size: 12px;
}

.connection-status {
  display: flex;
  align-items: center;
  gap: 6px;
}

.connection-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: #4CAF50;
  box-shadow: 0 0 8px rgba(76,175,80,0.5);
}

.data-rate {
  opacity: 0.8;
}

@media (max-width: 768px) {
  .status-grid {
    grid-template-columns: 1fr;
    grid-template-rows: auto;
  }
  
  .primary-metric {
    grid-row: auto;
  }
  
  .device-title {
    font-size: 16px;
  }
  
  .gauge-container {
    width: 100px;
    height: 70px;
  }
  
  .temperature-value .value {
    font-size: 20px;
  }
}

.status-normal { color: #4CAF50; }
.status-warning { color: #FF9800; }
.status-critical { color: #F44336; }
```

### Step 5: Add JavaScript Controller

**Copy and paste this JavaScript into the "JavaScript" tab:**

```javascript
self.onInit = function() {
    self.ctx.data = {
        temperature: 0,
        rssi: 0,
        heap: 0,
        uptime: 0
    };
    
    self.updateDisplay();
};

self.onDataUpdated = function() {
    var data = self.ctx.defaultSubscription.data;
    
    if (data && data.length > 0) {
        for (var i = 0; i < data.length; i++) {
            var dataKey = data[i].dataKey;
            if (dataKey && data[i].data.length > 0) {
                var value = data[i].data[data[i].data.length - 1][1];
                
                switch(dataKey.name) {
                    case 'temperature':
                        self.ctx.data.temperature = parseFloat(value);
                        break;
                    case 'rssi':
                        self.ctx.data.rssi = parseInt(value);
                        break;
                    case 'heap':
                        self.ctx.data.heap = parseInt(value);
                        break;
                    case 'uptime':
                        self.ctx.data.uptime = parseInt(value);
                        break;
                }
            }
        }
        
        self.updateDisplay();
        
        var now = new Date();
        self.ctx.$container.find('.timestamp').text('Last: ' + now.toLocaleTimeString());
    }
};

self.updateDisplay = function() {
    var temp = self.ctx.data.temperature;
    var rssi = self.ctx.data.rssi;
    var heap = self.ctx.data.heap;
    var uptime = self.ctx.data.uptime;
    
    // Update Temperature
    self.ctx.$container.find('.temperature-value .value').text(temp.toFixed(1));
    
    var percentage = Math.min(Math.max(temp / 60, 0), 1);
    var angle = -90 + (percentage * 180);
    self.ctx.$container.find('.gauge-needle').attr('transform', 'rotate(' + angle + ' 100 90)');
    
    var circumference = Math.PI * 70;
    var progress = percentage * circumference;
    self.ctx.$container.find('.gauge-active').attr('stroke-dasharray', progress + ',' + circumference);
    
    var statusText, color;
    if (temp < 30) {
        statusText = 'Normal';
        color = '#4CAF50';
    } else if (temp < 45) {
        statusText = 'Warm';
        color = '#FF9800';
    } else {
        statusText = 'Hot';
        color = '#F44336';
    }
    
    self.ctx.$container.find('.temperature-status').text(statusText);
    self.ctx.$container.find('.gauge-active').attr('stroke', color);
    
    // Update RSSI
    self.ctx.$container.find('.rssi-value .value').text(rssi);
    
    var bars = 0;
    var quality = 'Poor';
    
    if (rssi >= -30) {
        bars = 5;
        quality = 'Excellent';
    } else if (rssi >= -50) {
        bars = 4;
        quality = 'Very Good';
    } else if (rssi >= -60) {
        bars = 3;
        quality = 'Good';
    } else if (rssi >= -70) {
        bars = 2;
        quality = 'Fair';
    } else if (rssi >= -80) {
        bars = 1;
        quality = 'Poor';
    }
    
    self.ctx.$container.find('.signal-bar').each(function(index) {
        if (index < bars) {
            $(this).addClass('active');
        } else {
            $(this).removeClass('active');
        }
    });
    
    self.ctx.$container.find('.rssi-quality').text(quality);
    
    // Update Memory
    var heapKB = Math.round(heap / 1024);
    self.ctx.$container.find('.memory-value .value').text(heapKB.toLocaleString());
    
    var totalMemoryKB = 320;
    var memoryPercentage = Math.min((heapKB / totalMemoryKB) * 100, 100);
    
    self.ctx.$container.find('.progress-fill').css('width', memoryPercentage + '%');
    
    var memStatus;
    if (memoryPercentage > 70) {
        memStatus = 'Excellent';
    } else if (memoryPercentage > 40) {
        memStatus = 'Good';
    } else {
        memStatus = 'Low';
    }
    
    self.ctx.$container.find('.memory-status').text(memStatus);
    
    // Update Uptime
    var days = Math.floor(uptime / 86400);
    var hours = Math.floor((uptime % 86400) / 3600);
    var minutes = Math.floor((uptime % 3600) / 60);
    
    var timeSegments = self.ctx.$container.find('.time-segment');
    $(timeSegments[0]).find('.time-value').text(days);
    $(timeSegments[1]).find('.time-value').text(hours);
    $(timeSegments[2]).find('.time-value').text(minutes);
    
    var stability;
    if (uptime < 300) {
        stability = 'Starting';
    } else if (uptime < 3600) {
        stability = 'Warming Up';
    } else if (uptime < 86400) {
        stability = 'Stable';
    } else {
        stability = 'Rock Solid';
    }
    
    self.ctx.$container.find('.uptime-stability').text(stability);
};

self.onDestroy = function() {
    // Cleanup
};
```

### Step 6: Configure Widget Settings

1. **Widget Size**:
   - **Size X**: `8`
   - **Size Y**: `6`

2. **Data Sources**:
   - **Entity**: Select your ESP32 device
   - **Data Keys**: Add the following keys:
     - `temperature` (type: timeseries, label: "Temperature")
     - `rssi` (type: timeseries, label: "RSSI")  
     - `heap` (type: timeseries, label: "Free Memory")
     - `uptime` (type: timeseries, label: "Uptime")

### Step 7: Save and Test

1. **Save Widget**: Click **"Save"** to create the widget
2. **Test Widget**: Add it to a dashboard to verify functionality

---

## 🎯 Adding to Dashboard

### Method 1: From Dashboard Edit Mode

1. **Open Dashboard** in edit mode
2. **Click "Add widget"**
3. **Find Widget**: Navigate to **"ESP32 IoT Widgets"** bundle
4. **Select**: **"ESP32-S3 System Status"**
5. **Configure**:
   - **Entity**: Select your ESP32 device
   - **Data keys**: Should auto-populate with temperature, rssi, heap, uptime
6. **Apply Changes**

### Method 2: From Widget Library

1. **Widget Library** → **"ESP32 IoT Widgets"**
2. **Open**: **"ESP32-S3 System Status"** widget
3. **Click**: **"Add to dashboard"**
4. **Select Dashboard**: Choose target dashboard
5. **Configure data source**

---

## ✅ Verification Checklist

**Widget Creation:**
- ✅ HTML template copied correctly
- ✅ CSS styling applied
- ✅ JavaScript controller functional
- ✅ Widget size set to 8x6
- ✅ All data keys configured

**Dashboard Integration:**
- ✅ Widget appears on dashboard
- ✅ Temperature gauge shows correct values and needle movement
- ✅ RSSI bars reflect signal strength
- ✅ Memory progress bar displays heap usage
- ✅ Uptime shows days/hours/minutes
- ✅ Timestamp updates every 5 seconds

**Visual Verification:**
- ✅ Glass-morphism background with blur effect
- ✅ Gradient blue-to-purple theme
- ✅ Responsive layout on mobile devices
- ✅ Smooth animations and transitions
- ✅ Color-coded status indicators

---

## 🔧 Troubleshooting

### Common Issues

**❌ Widget shows "No data"**
- **Check**: ESP32 device is selected as data source
- **Verify**: Telemetry keys match exactly: `temperature`, `rssi`, `heap`, `uptime`
- **Test**: View device's "Latest telemetry" to confirm data is being received

**❌ Styling looks broken**
- **Solution**: Clear browser cache (Ctrl+F5)
- **Verify**: CSS was pasted completely without truncation
- **Check**: Browser supports CSS backdrop-filter (Chrome 88+, Firefox 87+, Safari 14+)

**❌ Animations not working**
- **Verify**: JavaScript controller copied completely
- **Check**: Browser console for errors (F12 → Console tab)
- **Test**: Refresh dashboard page

**❌ Temperature gauge not moving**
- **Check**: Temperature data is numeric (not string)
- **Verify**: SVG elements are present in HTML
- **Debug**: Add console.log in updateDisplay function

### Performance Tips

- **Single Widget**: Use this instead of multiple individual widgets for better performance
- **Real-time**: Set dashboard time window to "Last 1 hour" for optimal responsiveness
- **Mobile**: Widget automatically adapts to smaller screens

---

## 🎨 Customization Options

### Color Themes

**Change gradient background** (in CSS):
```css
/* Default: Blue to Purple */
background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);

/* Green Theme */
background: linear-gradient(135deg, #4CAF50 0%, #2E7D32 100%);

/* Orange Theme */
background: linear-gradient(135deg, #FF9800 0%, #E65100 100%);

/* Dark Theme */
background: linear-gradient(135deg, #2c3e50 0%, #34495e 100%);
```

### Temperature Thresholds

**Modify temperature zones** (in JavaScript):
```javascript
// Current: Normal < 30°C, Warm 30-45°C, Hot > 45°C
// Custom thresholds:
if (temp < 25) {
    statusText = 'Cool';
    color = '#2196F3';
} else if (temp < 35) {
    statusText = 'Normal';  
    color = '#4CAF50';
} else if (temp < 50) {
    statusText = 'Warm';
    color = '#FF9800';
} else {
    statusText = 'Critical';
    color = '#F44336';
}
```

### Memory Calculation

**Adjust total memory** (in JavaScript):
```javascript
// Current: 320KB total
var totalMemoryKB = 320;

// For different ESP32 variants:
var totalMemoryKB = 520; // ESP32-S3 with more PSRAM
```

---

## 🎉 Success!

You've successfully created a professional ESP32-S3 system status widget with:

- **🌡️ Animated temperature gauge** with real-time needle movement
- **📶 5-bar signal strength** indicator with quality labels
- **💾 Memory progress bar** with gradient colors
- **⏱️ Multi-segment uptime** display (days/hours/minutes)
- **🎨 Glass-morphism design** with professional styling
- **📱 Responsive layout** for all device sizes

The widget will automatically update every 5 seconds with your ESP32's telemetry data, providing a comprehensive system monitoring solution.