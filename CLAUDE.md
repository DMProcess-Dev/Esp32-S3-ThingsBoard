# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a production-quality ESP32-S3 firmware project demonstrating ThingsBoard IoT platform integration with sophisticated web-based Wi-Fi provisioning. The firmware implements a complete event-driven provisioning system with MQTT telemetry transmission, ARGB LED status indication, and robust error handling.

## CRITICAL DEVELOPMENT PHILOSOPHY

### MANDATORY WORKFLOW: Research → Plan → Implement
**NEVER JUMP STRAIGHT TO CODING!** Always follow this sequence:

1. **Research**: Explore codebase, understand existing patterns, USE CONTEXT7 MCP to fetch ESP-IDF documentation
2. **Plan**: Create detailed implementation plan including memory usage, task priorities, verify with user
3. **Implement**: Execute plan with validation checkpoints

When implementing any feature, say: "Let me research the codebase and fetch ESP-IDF documentation using Context7 before implementing."

### Context7 MCP Integration Requirements
**ALWAYS use Context7 MCP when you need to:**
- Verify ESP-IDF API usage
- Check FreeRTOS task configuration
- Reference ThingsBoard integration patterns
- Validate peripheral driver implementations
- Confirm memory management best practices

### Quality Standards: ZERO TOLERANCE
**ALL hook issues are BLOCKING - EVERYTHING must be ✅ GREEN!**

#### Mandatory Checks:
- `idf.py build` - Must compile without errors or warnings
- `clang-tidy` - ESP-IDF clang-tidy must pass completely
- GitHub Actions CI/CD - All workflows must pass
- Memory analysis - No stack overflow or heap issues
- Static analysis - All ESP-IDF static checks pass

No errors. No formatting issues. No linting problems. Fix ALL issues before continuing.

## Development Environment

### Platform Configuration
- **OS**: Fedora Linux
- **Framework**: ESP-IDF v5.4.1
- **RTOS**: FreeRTOS
- **Methodology**: TDD without unit testing initially
- **Philosophy**: Simple, efficient, no overengineering, no code comments

### Build and Flash Commands
```bash
# Set target chip (required for new builds)
idf.py set-target esp32s3

# Build and flash to device (replace /dev/ttyUSB0 with your serial port)
idf.py -p /dev/ttyUSB0 flash

# Monitor serial output
idf.py -p /dev/ttyUSB0 monitor

# Full build and flash with monitoring
idf.py -p /dev/ttyUSB0 flash monitor

# Clean build
idf.py clean

# Configuration menu
idf.py menuconfig
```

## Architecture Overview

### Core Components
- **Event-Driven Architecture**: FreeRTOS task management with event loops
- **Provisioning System**: AP mode with HTTP server and dynamic Wi-Fi scanning
- **MQTT Telemetry**: Structured JSON payload transmission every 5 seconds
- **ARGB LED Status**: Sophisticated visual feedback system
- **NVS Storage**: Persistent Wi-Fi and MQTT credential management
- **Temperature Sensor**: Built-in ESP32-S3 temperature monitoring
- **Memory Management**: Heap monitoring and stack overflow protection

### Key Files
- `main/app_main.c`: Main application entry point with event-driven logic
- `main/certificate_manager.h`: Certificate management API and type definitions
- `main/certificate_manager.c`: Secure certificate provisioning and validation implementation
- `main/provisioning_page.h`: HTML content for provisioning web interface
- `main/ca_certificate.h`: Development certificate definitions (fallback only)
- `main/CMakeLists.txt`: Component configuration and dependencies
- `CMakeLists.txt`: Root project configuration
- `sdkconfig`: ESP-IDF configuration (auto-generated)

### Hardware Configuration

#### Target Device
- **Chip**: ESP32-S3
- **ARGB LED**: GPIO 48 with status indication system
- **Temperature Sensor**: Built-in ESP32-S3 sensor
- **Communication**: UART debugging, Wi-Fi connectivity

#### ARGB LED Status System
- **White**: Provisioning mode active
- **Blue**: Connecting to Wi-Fi network
- **Green**: Connected to Wi-Fi and MQTT
- **Red**: Error state (Wi-Fi/MQTT connection failed)
- **Brightness**: Configurable DEFAULT_LED_BRIGHTNESS (0-255, default 25)

## Provisioning Flow Architecture

### Complete Provisioning Sequence
1. **Boot Phase**: Device starts in AP mode (`ESP32-Provisioning`, password: `password`)
2. **Discovery Phase**: User connects to AP, navigates to `http://192.168.4.1`
3. **Scanning Phase**: Dynamic Wi-Fi network scanning with RSSI and security info
4. **Configuration Phase**: Wi-Fi credentials and MQTT broker configuration
5. **Storage Phase**: Credentials saved to NVS with atomic operations
6. **Connection Phase**: Wi-Fi connection with visual LED feedback
7. **SSL/TLS Phase**: CA certificate loaded and validated for secure connections
8. **Telemetry Phase**: MQTTS client connects with SSL/TLS encryption and begins data transmission

### HTTP Server Endpoints
- `GET /`: Main provisioning page (HTML interface)
- `GET /api/wifi-scan`: Real-time Wi-Fi network scan results (JSON)
- `GET /api/status`: Connection status polling (JSON)
- `POST /connect`: Submit Wi-Fi and MQTT credentials
- `GET /favicon.ico`: Returns 204 No Content (prevents 404 errors)

### NVS Storage Schema
#### Primary Storage (`wifi_creds` namespace)
- **Keys**: `ssid`, `password`, `mqtt_host`, `mqtt_port`, `mqtt_user`, `mqtt_pass`
- **Operations**: Atomic read/write with error handling

#### Certificate Storage (`cert_mgr` namespace)
- **Keys**: `primary_cert`, `backup_cert`, `metadata` - Certificate manager storage
- **Purpose**: Multi-tier certificate provisioning and validation
- **Initialization**: Automatic certificate manager initialization on first boot
- **Metadata**: Certificate source, checksum, expiration, validation status

### Telemetry Data Structure
JSON payload transmitted every 5 seconds:
```json
{
  "temperature": 25.6,
  "rssi": -45,
  "heap": 245760,
  "uptime": 120
}
```

## CERTIFICATE MANAGEMENT ARCHITECTURE

### Advanced Certificate Manager System
The firmware implements a production-ready certificate management system with multi-tier provisioning:

#### Certificate Manager Initialization
```c
// Certificate manager configuration in app_main()
cert_manager_config_t cert_config = CERT_MANAGER_DEFAULT_CONFIG();
cert_config.nvs_namespace = "cert_mgr";
cert_config.allow_development_cert = true;  // Development fallback
esp_err_t cert_init_err = cert_manager_init(&cert_config);
```

#### Multi-Tier Certificate Provisioning
```c
typedef enum {
    CERT_SOURCE_MANUFACTURING = 0,  // Secure: Manufacturing injection
    CERT_SOURCE_OTA_UPDATE,         // Secure: OTA updates
    CERT_SOURCE_CONFIG_ENDPOINT,    // Secure: Encrypted endpoint
    CERT_SOURCE_DEVELOPMENT,        // Fallback: Development only
} cert_source_t;
```

#### Certificate Storage Strategy
- **Primary Storage**: `cert_mgr` namespace - Dedicated certificate manager storage
- **Metadata Storage**: Certificate source, checksum, expiration tracking
- **Backup Support**: Primary/backup certificate rotation
- **Format**: PEM-encoded X.509 certificates with validation
- **Integrity**: CRC32 checksum verification for corruption detection

#### Certificate Loading Priority
1. **Primary Certificate**: Load from certificate manager storage
2. **Backup Certificate**: Fallback to backup if primary fails
3. **Development Certificate**: Fallback to embedded certificate (development only)
4. **Validation**: Full certificate format and integrity verification

### Certificate Manager API
```c
// Core certificate manager functions
esp_err_t cert_manager_init(const cert_manager_config_t* config);
esp_err_t cert_manager_store(const char* cert_pem, cert_source_t source);
esp_err_t cert_manager_load(char* cert_buffer, size_t buffer_size, size_t* actual_size);
esp_err_t cert_manager_validate(const char* cert_pem, cert_validation_result_t* result);
esp_err_t cert_manager_rotate(const char* new_cert_pem, cert_source_t source);
bool cert_manager_is_certificate_valid(void);
```

### MQTTS Connection Security
```c
// SSL/TLS configuration using certificate manager
esp_err_t cert_err = cert_manager_load(ca_cert, cert_len, &cert_len);
esp_mqtt_client_config_t mqtt_cfg = {
    .broker.address.uri = uri,
    .broker.verification.certificate = ca_cert,
    .broker.verification.use_global_ca_store = false,
};
```

#### Security Features
- **Port**: Standard MQTTS port 8883 for encrypted connections
- **Protocol**: TLS 1.2+ with certificate validation
- **CA Verification**: Full certificate authority validation
- **Error Handling**: Enhanced SSL error detection and logging
- **Certificate Metadata**: Source tracking and validation logging

#### Certificate Validation Process
1. **Certificate Load**: Load via certificate manager with metadata
2. **Format Validation**: Verify PEM format and structure
3. **Integrity Check**: CRC32 checksum validation
4. **Expiration Check**: Validate certificate is not expired
5. **Connection Security**: Establish encrypted MQTTS connection
6. **Error Recovery**: Comprehensive error handling with recovery steps

### Enhanced Security Error Handling
```c
if (cert_err != ESP_OK) {
    ESP_LOGE(TAG, "CRITICAL: Failed to load CA certificate from certificate manager!");
    ESP_LOGE(TAG, "Recovery options:");
    ESP_LOGE(TAG, "1. Run 'idf.py erase-flash' and reflash firmware to reinitialize certificates");
    ESP_LOGE(TAG, "2. Provision certificate via secure endpoint using cert_manager_store()");
    ESP_LOGE(TAG, "3. Check certificate expiration and validity");
    ESP_LOGE(TAG, "4. Verify NVS partition is not corrupted");
    ESP_LOGE(TAG, "5. Consult troubleshooting guide in project documentation");
}
```

### Certificate Manager Functions
- **`cert_manager_init()`**: Initialize certificate management system
- **`cert_manager_store()`**: Store certificate with source tracking
- **`cert_manager_load()`**: Load certificate with automatic validation
- **`cert_manager_validate()`**: Comprehensive certificate validation
- **`cert_manager_rotate()`**: Secure certificate rotation
- **`cert_manager_get_metadata()`**: Certificate source and validation info

### Production Security Standards
- ✅ **Multi-Tier Provisioning**: Manufacturing, OTA, config endpoint, development
- ✅ **Certificate Validation**: Format, integrity, expiration checking
- ✅ **Metadata Tracking**: Source, checksum, timestamps
- ✅ **Certificate Rotation**: Secure updates without firmware changes
- ✅ **Error Recovery**: Comprehensive recovery procedures
- ✅ **Development Fallback**: Secure fallback for development environments
- ✅ **Memory Management**: Secure certificate storage and cleanup

## LOGGING ARCHITECTURE

### ESP32 Logging Levels
```
ESP_LOG_ERROR   - Critical failures only
ESP_LOG_WARN    - Important warnings
ESP_LOG_INFO    - System state changes
ESP_LOG_DEBUG   - Development debugging
ESP_LOG_VERBOSE - Detailed tracing
```

### Logging Standards
- Use ESP_LOG macros consistently
- Include task name in log messages
- Log memory usage at key points
- Include timestamps for timing analysis
- Tag-based logging for different modules

### Example Logging Pattern
```c
static const char* TAG = "MODULE_NAME";

ESP_LOGI(TAG, "Module initialized, heap: %d bytes", esp_get_free_heap_size());
ESP_LOGD(TAG, "Processing task started on core %d", xPortGetCoreID());
```

## MEMORY MANAGEMENT

### Memory Monitoring Requirements
- Check heap size at system initialization (Current: ~323KB free)
- Monitor stack usage for all FreeRTOS tasks
- Track memory leaks during development
- Validate memory usage before commits
- Monitor certificate storage impact (~2KB per certificate)

### Performance Checkpoints
- Memory usage validation at 85% threshold
- Stack overflow detection enabled
- Heap corruption detection active
- Task watchdog monitoring enabled
- Certificate storage validation (max 2048 bytes per cert)

### Current Memory Profile
- **Free Heap**: 323,604 bytes at startup
- **Certificate Storage**: ~2KB in NVS `cert_mgr` namespace (including metadata)
- **Certificate Manager**: ~8KB code size with comprehensive validation
- **MQTT Client**: SSL/TLS context memory allocation
- **Temperature Sensor**: Built-in ESP32-S3 sensor integration

## BRANCHING MODEL: GITHUB FLOW

### Workflow Steps
1. **Create Feature Branch**: `<type>/<short-description>` (e.g., `feat/wpa3-support`)
2. **Implement and Commit**: Follow conventional commits specification
3. **Open Pull Request**: Trigger all CI checks
4. **Review and Approve**: Code quality and CI verification
5. **Squash and Merge**: Single clean commit to master
6. **Delete Feature Branch**: Cleanup after merge

### Commit Message Format
Follow Conventional Commits v1.0.0 with ESP32-specific scopes:
- `feat(wifi)`: Wi-Fi functionality
- `feat(security)`: SSL/TLS and security features
- `fix(mqtt)`: MQTT fixes
- `fix(ssl)`: SSL/TLS certificate issues
- `docs`: Documentation changes
- `chore(nvs)`: NVS storage changes
- `perf(telemetry)`: Performance improvements
- `refactor(led)`: Code restructuring

See `CONVENTIONAL_COMMITS.md` for complete specification.

## AUTOMATED CHANGELOG WORKFLOW

### Commit & Changelog Process
1. **Implementation**: Complete coding task
2. **Verification**: Run all mandatory checks
3. **Approval Request**: Present changes and commit message
4. **Atomic Commit**: Generate changelog, stage changes, commit, update with GitHub link

### Changelog Entry Format
`- <description> ([<short-sha>](<github-commit-url>))`

## TDD APPROACH (WITHOUT UNIT TESTING INITIALLY)

### Hardware-Focused Testing
1. **Define Behavior**: What should the module do?
2. **Write Minimal Code**: Implement just enough to compile
3. **Test on Hardware**: Verify actual behavior matches expected
4. **Refactor**: Improve without changing behavior
5. **Repeat**: Build incrementally

### Testing Focus Areas
- Real device testing over simulation
- Performance measurement on target hardware
- Memory usage validation
- Power consumption monitoring

## COMMUNICATION PROTOCOLS

### Progress Updates
```
✓ Added WiFi connection handling
✗ Found issue with task watchdog - investigating
⚠ Memory usage at 85% - needs optimisation
```

### Problem-Solving Protocol
When stuck or confused:
1. **Stop** - Don't spiral into complex solutions
2. **Fetch Documentation** - Use Context7 MCP for ESP-IDF/ThingsBoard docs
3. **Ultrathink** - Say "I need to ultrathink through this embedded challenge"
4. **Step back** - Re-read requirements
5. **Simplify** - Simple solution is usually correct
6. **Ask** - Present options for user decision

### Context7 MCP Usage Patterns
**Hardware Issues**: Fetch ESP32 Technical Reference Manual
**Software Issues**: Fetch ESP-IDF API documentation
**Integration Issues**: Fetch ThingsBoard API documentation

## VALIDATION CHECKPOINTS

### Mandatory Validation Points
- After implementing complete feature
- Before starting new major component
- When memory usage seems high
- Before declaring "done"
- After any FreeRTOS task creation

### Verification Steps
1. Flash firmware and monitor serial output
2. Connect to `ESP32-Provisioning` Wi-Fi network
3. Navigate to `http://192.168.4.1` for provisioning
4. Verify ARGB LED status transitions
5. Confirm Wi-Fi connection and MQTT connectivity
6. Monitor telemetry data transmission
7. Test error scenarios and recovery

## DEVELOPMENT NOTES

### Current Status
- Firmware always starts in provisioning mode for development
- ARGB LED provides comprehensive status feedback
- Dynamic Wi-Fi scanning with security information
- Persistent MQTT broker configuration
- Temperature sensor integration active
- Memory monitoring enabled

### Common Issues & Solutions
- **Build failures**: Ensure ESP-IDF v5.4.1 sourced correctly
- **Flash issues**: Check serial port permissions and cable
- **Memory issues**: Monitor heap usage and stack sizes
- **Wi-Fi connection**: Verify network credentials and availability
- **MQTT connection**: Test with public broker first

### Next Development Areas
- ThingsBoard device provisioning integration
- OTA firmware update capability
- Power management optimization
- Enhanced error recovery mechanisms

## REMINDER PROTOCOL

**If this file hasn't been referenced in 30+ minutes, RE-READ IT!**

This ensures consistent adherence to the strict development standards and quality requirements essential for production-quality ESP32 firmware development.