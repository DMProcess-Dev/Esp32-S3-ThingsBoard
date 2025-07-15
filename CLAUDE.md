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
- `main/provisioning_page.h`: HTML content for provisioning web interface
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
- **Brightness**: Configurable MAX_BRIGHTNESS (0-255, default 50)

## Provisioning Flow Architecture

### Complete Provisioning Sequence
1. **Boot Phase**: Device starts in AP mode (`ESP32-Provisioning`, password: `password`)
2. **Discovery Phase**: User connects to AP, navigates to `http://192.168.4.1`
3. **Scanning Phase**: Dynamic Wi-Fi network scanning with RSSI and security info
4. **Configuration Phase**: Wi-Fi credentials and MQTT broker configuration
5. **Storage Phase**: Credentials saved to NVS with atomic operations
6. **Connection Phase**: Wi-Fi connection with visual LED feedback
7. **Telemetry Phase**: MQTT client connects and begins data transmission

### HTTP Server Endpoints
- `GET /`: Main provisioning page (HTML interface)
- `GET /api/wifi-scan`: Real-time Wi-Fi network scan results (JSON)
- `GET /api/status`: Connection status polling (JSON)
- `POST /connect`: Submit Wi-Fi and MQTT credentials
- `GET /favicon.ico`: Returns 204 No Content (prevents 404 errors)

### NVS Storage Schema
- **Namespace**: `wifi_creds`
- **Keys**: `ssid`, `password`, `mqtt_host`, `mqtt_port`, `mqtt_user`, `mqtt_pass`
- **Operations**: Atomic read/write with error handling

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
- Check heap size at system initialization
- Monitor stack usage for all FreeRTOS tasks
- Track memory leaks during development
- Validate memory usage before commits

### Performance Checkpoints
- Memory usage validation at 85% threshold
- Stack overflow detection enabled
- Heap corruption detection active
- Task watchdog monitoring enabled

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
- `fix(mqtt)`: MQTT fixes
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