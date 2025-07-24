# GitHub Copilot Instructions

This ESP32-S3 ThingsBoard integration project implements a production-quality IoT firmware with web-based Wi-Fi provisioning, MQTT telemetry, and comprehensive certificate management.

## Project Architecture

### Core Components
- **`main/app_main.c`**: Main application with event-driven state machine handling Wi-Fi provisioning → MQTT connection → telemetry transmission
- **`main/certificate_manager.{c,h}`**: Multi-tier certificate provisioning system (manufacturing → OTA → development fallback)
- **`main/provisioning_page.h`**: Embedded HTML/CSS/JS for captive portal provisioning interface
- **`thingsboard/`**: Dashboard widgets, MQTT broker config, and ThingsBoard integration assets

### Key Architectural Patterns

**Event-Driven State Management**: The firmware uses ESP-IDF's event system with color-coded LED feedback:
```c
// LED states indicate system status
LED_COLOR_WHITE     // Provisioning mode active
LED_COLOR_BLUE      // Connecting to Wi-Fi
LED_COLOR_GREEN     // Connected to ThingsBoard, sending telemetry
LED_COLOR_RED       // Connection error
```

**Smart Boot Logic**: 
- First boot → automatic provisioning mode (no stored credentials)
- Subsequent boots → direct Wi-Fi connection using stored NVS credentials
- Reset via `idf.py erase-flash` returns to provisioning mode

**Task Creation Pattern**: Critical tasks are created dynamically only after prerequisite connections:
```c
case MQTT_EVENT_CONNECTED:
    xTaskCreate(telemetry_task, "telemetry_task", 4096, client, 5, NULL);
```

## Development Workflow

### Essential Commands
```bash
# Build and flash (primary development cycle)
idf.py build flash monitor

# Reset to provisioning mode
idf.py erase-flash

# Component dependencies via managed_components/
idf.py build  # Auto-downloads from component registry
```

### Quality Gates (ALL must pass)
- **Build**: `idf.py build` - zero errors/warnings
- **Static Analysis**: ESP-IDF clang-tidy integration
- **Memory Safety**: FreeRTOS stack overflow detection enabled
- **CI/CD**: GitHub Actions workflows must pass

### Branch Strategy
- `main`: Stable releases
- `feat/insecure-mqtt`: Current development (MQTT port 1883)
- `feat/secure-mqtts`: Planned TLS/certificate implementation (port 8883)

## ThingsBoard Integration Specifics

### Telemetry Format
The firmware sends standardized JSON every 5 seconds to `v1/devices/me/telemetry`:
```json
{
  "temperature": 25.6,  // °C from ESP32-S3 internal sensor
  "rssi": -45,         // dBm Wi-Fi signal strength  
  "heap": 245760,      // bytes free memory
  "uptime": 120        // seconds since boot
}
```

### Configuration Defaults
- **Server**: `193.164.4.51:1883` (pre-configured)
- **Access Token**: `VbYfLIDth7lUgBs5nrzf` (demo token)
- **Protocol Detection**: Automatic MQTT (1883) vs MQTTS (8883) based on port

## Critical Implementation Details

### Memory Management
- **Telemetry Task Stack**: 4096 bytes (validated for JSON creation + MQTT)
- **HTTP Server**: 512-byte content buffer for POST requests
- **LED Strip**: Single ARGB pixel on GPIO 48 with brightness scaling

### Certificate Management System
Multi-tier security with graceful fallbacks:
1. Manufacturing-injected certificates (most secure)
2. OTA-provisioned certificates
3. Development certificates (compile-time, debug only)

### NVS Storage Schema
```c
// Wi-Fi credentials namespace
"wifi_creds" -> {ssid, password}

// MQTT configuration namespace  
"mqtt_config" -> {server, port, access_token}
```

### HTTP Provisioning Endpoints
- `GET /`: Serves embedded HTML provisioning page
- `POST /`: Handles form submission (Wi-Fi + ThingsBoard config)
- `GET /scan`: Returns available Wi-Fi networks as JSON

## Testing & Debugging

### Verification Steps
1. **LED Status**: Visual confirmation of connection states
2. **Serial Monitor**: `idf.py monitor` for real-time telemetry logs
3. **ThingsBoard Dashboard**: Verify data ingestion at `http://193.164.4.51:8080`
4. **Public MQTT Test**: Use `broker.hivemq.com` + MQTTX client for protocol validation

### Common Debugging Patterns
- **Provisioning Issues**: Check AP mode activation and HTTP server logs
- **MQTT Failures**: Verify access token in credentials tab + network connectivity
- **Certificate Errors**: Use certificate manager validation functions
- **Memory Issues**: Monitor heap reports in telemetry output

## Project-Specific Conventions

### Code Style
- **No Comments Philosophy**: Code should be self-documenting
- **ESP-IDF Logging**: Use `ESP_LOG*` macros with component tags
- **Error Handling**: Always check `ESP_ERROR_CHECK()` for critical operations

### Component Dependencies
Defined in `main/idf_component.yml` and `main/CMakeLists.txt`:
- `led_strip`: ARGB LED control
- `mqtt`: ThingsBoard communication
- `esp_http_server`: Provisioning interface
- `json`: Telemetry payload formatting

When adding features, follow the established pattern: research existing ESP-IDF examples → adapt to project architecture → validate with quality gates → test end-to-end with ThingsBoard integration.
