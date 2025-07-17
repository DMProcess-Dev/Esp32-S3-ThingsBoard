# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### ✨ Features
- **(security)**: Implement enterprise-grade SSL/TLS certificate management with automatic ThingsBoard/Mosquitto CA initialization
- **(mqtt)**: Add secure MQTTS connections with full certificate validation on port 8883
- **(security)**: Multi-namespace certificate storage with fallback support (security/wifi_creds namespaces)
- **(security)**: Enhanced SSL error handling and comprehensive certificate validation logging
- **(telemetry)**: Integrate ESP32-S3 built-in temperature sensor with ±1°C accuracy (-10°C to 80°C range)
- **(mqtt, security)**: Implement SSL certificate verification for secure MQTTS connections
- **(ui, led)**: Improve LED feedback and provisioning UI ([e8b9baa](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/e8b9baa))
- **(chore)**: Add remote mqtt broker and update firmware ([4296abc](https://github.com/DMProcess-Dev/Esp32-S3-ThingsBoard/commit/4296abc))
- **(provisioning)**: Implement web-based Wi-Fi provisioning with network scanning ([9e2e485](https://github.com/OmerFarukOruc/Esp32-S3-ThingsBoard/commit/9e2e485))

### 🐛 Bug Fixes
- **(security)**: Remove hardcoded credentials from production firmware for enhanced security posture
- **(ssl)**: Fix SSL certificate loading sequence with proper fallback mechanism
- **(mqtt)**: Resolve MQTTS connection failures through proper certificate validation
- **(provisioning)**: Address all review comments for provisioning flow ([60eeb77](https://github.com/OmerFarukOruc/Esp32-S3-ThingsBoard/commit/60eeb77))

### 🔧 Miscellaneous
- Add initial project structure and documentation ([370acd3](https://github.com/OmerFarukOruc/Esp32-S3-ThingsBoard/commit/370acd394be21369640221aa0c89e4cec04b43e3))
- **(setup)**: Verify ESP-IDF toolchain and board communication ([45ddc4a](https://github.com/OmerFarukOruc/Esp32-S3-ThingsBoard/commit/45ddc4a77962aaf59282e0196cda7dfc4cdfb71b))
