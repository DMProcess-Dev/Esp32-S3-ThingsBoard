# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### ✨ Features
- **(thingsboard)**: add dashboard widgets and configuration ([5fcccf7](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/5fcccf7))
- **(thingsboard)**: Implement ThingsBoard integration with access token authentication ([b7bbff3](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/b7bbff3))
- **(security)**: Implement comprehensive certificate management system with multi-tier provisioning (manufacturing, OTA, config endpoint, development) ([c10b81f](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/c10b81f))
- **(security)**: Add certificate validation with CRC32 integrity checks and metadata tracking ([c10b81f](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/c10b81f))
- **(security)**: Implement certificate rotation support without firmware updates ([c10b81f](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/c10b81f))
- **(led)**: Add LED color management system with named constants and proper brightness scaling ([c10b81f](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/c10b81f))
- **(core)**: Enhance error messages with actionable recovery steps for certificate issues ([c10b81f](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/c10b81f))
- **(security)**: Implement smart boot logic with credential detection for automatic provisioning bypass ([385248d](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/385248d))
- **(security)**: Add certificate optimization to prevent unnecessary NVS writes on subsequent boots ([385248d](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/385248d))
- **(security)**: Implement enterprise-grade SSL/TLS certificate management with automatic ThingsBoard/Mosquitto CA initialization ([385248d](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/385248d))
- **(mqtt)**: Add secure MQTTS connections with full certificate validation on port 8883 ([b7a712f](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/b7a712f))
- **(security)**: Multi-namespace certificate storage with fallback support (security/wifi_creds namespaces) ([385248d](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/385248d))
- **(security)**: Enhanced SSL error handling and comprehensive certificate validation logging ([fed9c00](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/fed9c00))
- **(telemetry)**: Integrate ESP32-S3 built-in temperature sensor with ±1°C accuracy (-10°C to 80°C range) ([1b25279](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/1b25279))
- **(mqtt, security)**: Implement SSL certificate verification for secure MQTTS connections ([fed9c00](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/fed9c00))
- **(ui, led)**: Implement ARGB LED status indicator system with configurable brightness control ([1b25279](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/1b25279))
- **(provisioning)**: Add configurable MQTT server settings in provisioning UI ([1b25279](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/1b25279))
- **(ui)**: Add real-time connection status notifications via polling API ([1b25279](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/1b25279))
- **(ui)**: Implement responsive two-column layout with dark mode support ([1b25279](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/1b25279))
- **(provisioning)**: Improve Wi-Fi network list with cache prevention headers ([1b25279](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/1b25279))
- **(ui, led)**: Improve LED feedback and provisioning UI ([e8b9baa](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/e8b9baa))
- **(chore)**: Add remote mqtt broker and update firmware ([e8b9baa](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/e8b9baa))
- **(provisioning)**: Implement web-based Wi-Fi provisioning with network scanning ([1b25279](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/1b25279))

### 🐛 Bug Fixes
- **(wifi)**: Fix Wi-Fi initialization sequence for direct connection mode in smart boot ([385248d](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/385248d))
- **(security)**: Remove hardcoded credentials from production firmware for enhanced security posture ([385248d](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/385248d))
- **(ssl)**: Fix SSL certificate loading sequence with proper fallback mechanism ([fed9c00](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/fed9c00))
- **(mqtt)**: Resolve MQTTS connection failures through proper certificate validation ([fed9c00](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/fed9c00))
- **(provisioning)**: Address all review comments for provisioning flow ([27d81da](https://github.com/OmerFarukOruc/Esp32-S3-ThingsBoard/commit/27d81da))

### 🔧 Miscellaneous
- **(docs)**: Update comprehensive documentation with SSL/TLS architecture and FAQ sections ([385248d](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/385248d))
- Add initial project structure and documentation ([370acd3](https://github.com/OmerFarukOruc/Esp32-S3-ThingsBoard/commit/370acd394be21369640221aa0c89e4cec04b43e3))
- **(setup)**: Verify ESP-IDF toolchain and board communication ([45ddc4a](https://github.com/OmerFarukOruc/Esp32-S3-ThingsBoard/commit/45ddc4a77962aaf59282e0196cda7dfc4cdfb71b))