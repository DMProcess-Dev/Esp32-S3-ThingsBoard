# Conventional Commits v1.0.0 Specification

## Overview
A specification for adding human and machine readable meaning to commit messages.

## Commit Message Structure

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

## Type Definitions

### Primary Types
- `feat`: A new feature for the user
- `fix`: A bug fix for the user
- `docs`: Documentation only changes
- `style`: Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
- `refactor`: A code change that neither fixes a bug nor adds a feature
- `perf`: A code change that improves performance
- `test`: Adding missing tests or correcting existing tests
- `build`: Changes that affect the build system or external dependencies
- `ci`: Changes to CI configuration files and scripts
- `chore`: Other changes that don't modify src or test files
- `revert`: Reverts a previous commit

### ESP32-Specific Scopes
- `wifi`: WiFi functionality
- `ble`: Bluetooth functionality
- `gpio`: GPIO operations
- `spi`: SPI communication
- `i2c`: I2C communication
- `uart`: UART communication
- `nvs`: Non-volatile storage
- `ota`: Over-the-air updates
- `power`: Power management
- `task`: FreeRTOS task management
- `timer`: Hardware timer operations
- `adc`: Analog-to-digital converter
- `dac`: Digital-to-analog converter
- `pwm`: Pulse-width modulation
- `rtc`: Real-time clock
- `watchdog`: Watchdog timer
- `partition`: Flash partition management
- `spiffs`: SPIFFS filesystem
- `fatfs`: FAT filesystem
- `mqtt`: MQTT communication
- `http`: HTTP client/server
- `websocket`: WebSocket communication
- `json`: JSON parsing/generation
- `crypto`: Cryptographic operations
- `security`: Security-related changes

## Rules

### Mandatory Elements
1. **Type**: Must be present
2. **Description**: Must be present and concise
3. **Colon and space**: Must follow type/scope

### Optional Elements
1. **Scope**: Provides additional contextual information
2. **Body**: Provides detailed explanation
3. **Footer**: Contains breaking change information or references

### Breaking Changes
- Use `BREAKING CHANGE:` in footer
- Use `!` after type/scope for immediate visibility

## Examples

### Basic Examples
```
feat(wifi): add WPA3 authentication support
fix(gpio): resolve interrupt handling race condition
docs: update API documentation for SPI driver
style(task): format FreeRTOS task creation code
refactor(nvs): simplify key-value storage implementation
perf(adc): optimise sampling rate for continuous mode
test(uart): add unit tests for baud rate configuration
build: update ESP-IDF to v5.4.1
ci: add memory usage analysis to GitHub Actions
chore(spiffs): update filesystem configuration
```

### With Body
```
feat(mqtt): implement secure connection handling

Add TLS support for MQTT connections with certificate validation.
Includes automatic reconnection logic and connection status monitoring.
```

### With Footer
```
fix(ota): resolve partition verification failure

Fixes issue where OTA updates would fail during partition verification
on devices with custom partition tables.

Closes #123
```

### Breaking Changes
```
feat(api)!: remove deprecated GPIO functions

BREAKING CHANGE: gpio_set_direction() has been removed in favour of gpio_config()
```

### Multiple Paragraphs and Footers
```
feat(ble): implement GATT server functionality

Add complete GATT server implementation with characteristic read/write support.
Includes automatic service discovery and connection management.

Performance improvements for characteristic notifications reduce latency by 40%.

Reviewed-by: John Doe
Refs: #456
```

## Validation Rules

### Description Requirements
- Use imperative mood ("add" not "added" or "adds")
- No capital letter at start
- No dot (.) at end
- Maximum 50 characters for first line

### Body Requirements
- Separate from description with blank line
- Wrap at 72 characters
- Explain what and why, not how

### Footer Requirements
- Separate from body with blank line
- Reference issues and breaking changes
- Use standard tokens: `BREAKING CHANGE:`, `Closes:`, `Fixes:`, `Refs:`

## CI/CD Integration

### GitHub Actions Validation
```yaml
- name: Validate Commit Message
  uses: wagoid/commitlint-github-action@v4
  with:
    configFile: '.commitlintrc.js'
```

### Pre-commit Hook
```bash
#!/bin/sh
commit_regex='^(feat|fix|docs|style|refactor|perf|test|build|ci|chore|revert)(\(.+\))?: .{1,50}'
if ! grep -qE "$commit_regex" "$1"; then
    echo "Invalid commit message format"
    exit 1
fi
```

## Benefits

### For ESP32 Development
- **Automated Changelog**: Generate release notes automatically
- **Semantic Versioning**: Trigger version bumps based on commit types
- **Build Automation**: Trigger different CI/CD pipelines based on scope
- **Documentation**: Maintain clear project history
- **Code Review**: Improve review process with structured commits

### Team Collaboration
- **Consistency**: Standardised commit format across team
- **Clarity**: Clear intent of each change
- **Traceability**: Easy to track feature development
- **Maintenance**: Simplified debugging and rollback processes

## Common Mistakes

### Incorrect Format
```
// Wrong
Add WiFi support
Fixed bug in GPIO
Updated docs

// Correct
feat(wifi): add WPA3 authentication support
fix(gpio): resolve interrupt handling race condition
docs: update API documentation for SPI driver
```

### Poor Descriptions
```
// Wrong
fix: bug fix
feat: new feature
docs: update

// Correct
fix(uart): resolve baud rate calculation error
feat(ble): implement GATT server functionality
docs(api): add examples for GPIO configuration
```

### Missing Context
```
// Wrong
feat: add function
fix: issue

// Correct
feat(power): add deep sleep mode configuration
fix(timer): resolve overflow in microsecond calculation
```

## Implementation Checklist

- [ ] Install commitlint and conventional commit tools
- [ ] Configure pre-commit hooks
- [ ] Set up GitHub Actions validation
- [ ] Create team guidelines document
- [ ] Train team on specification
- [ ] Implement automated changelog generation
- [ ] Configure semantic release automation