#pragma once

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @file certificate_manager.h
 * @brief Secure Certificate Management System for ESP32-S3
 * 
 * This module provides a comprehensive certificate management system with:
 * - Multi-tier certificate provisioning (secure → development fallback)
 * - Certificate validation and integrity checks
 * - Secure storage with corruption detection
 * - Certificate rotation without firmware updates
 * - Production-ready security standards
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Certificate provisioning source types (in order of security preference)
 */
typedef enum {
    CERT_SOURCE_MANUFACTURING = 0,  /**< Secure: Injected during manufacturing */
    CERT_SOURCE_OTA_UPDATE,         /**< Secure: Downloaded via OTA */
    CERT_SOURCE_CONFIG_ENDPOINT,    /**< Secure: Provisioned via encrypted endpoint */
    CERT_SOURCE_DEVELOPMENT,        /**< Insecure: Compile-time default (debug only) */
    CERT_SOURCE_NONE,               /**< No certificate source available */
    CERT_SOURCE_MAX
} cert_source_t;

/**
 * @brief Certificate validation result
 */
typedef enum {
    CERT_VALID = 0,                 /**< Certificate is valid */
    CERT_INVALID_FORMAT,            /**< Invalid PEM format */
    CERT_EXPIRED,                   /**< Certificate has expired */
    CERT_INTEGRITY_FAILED,          /**< Integrity check failed */
    CERT_NOT_FOUND,                 /**< Certificate not found */
    CERT_STORAGE_ERROR              /**< Storage operation failed */
} cert_validation_result_t;

/**
 * @brief Certificate metadata structure
 */
typedef struct {
    cert_source_t source;           /**< Certificate source type */
    uint32_t checksum;              /**< CRC32 checksum for integrity */
    uint64_t stored_time;           /**< Timestamp when stored */
    uint64_t expiry_time;           /**< Certificate expiry timestamp */
    size_t cert_size;               /**< Certificate size in bytes */
    bool is_valid;                  /**< Validation status */
} cert_metadata_t;

/**
 * @brief Certificate manager configuration
 */
typedef struct {
    bool allow_development_cert;    /**< Allow development certificate fallback */
    bool require_integrity_check;   /**< Require integrity validation */
    bool auto_rotate_expired;       /**< Auto-rotate expired certificates */
    const char* nvs_namespace;      /**< NVS namespace for storage */
} cert_manager_config_t;

/**
 * @brief Default certificate manager configuration
 */
#define CERT_MANAGER_DEFAULT_CONFIG() { \
    .allow_development_cert = true, \
    .require_integrity_check = true, \
    .auto_rotate_expired = false, \
    .nvs_namespace = "cert_mgr" \
}

/**
 * @brief Initialize certificate manager
 * 
 * @param config Configuration for certificate manager
 * @return esp_err_t ESP_OK on success
 */
esp_err_t cert_manager_init(const cert_manager_config_t* config);

/**
 * @brief Store certificate with metadata
 * 
 * @param cert_pem PEM-formatted certificate string
 * @param source Certificate source type
 * @return esp_err_t ESP_OK on success
 */
esp_err_t cert_manager_store(const char* cert_pem, cert_source_t source);

/**
 * @brief Load certificate with automatic validation
 * 
 * @param cert_buffer Buffer to store certificate
 * @param buffer_size Size of buffer
 * @param actual_size Actual certificate size (output)
 * @return esp_err_t ESP_OK on success
 */
esp_err_t cert_manager_load(char* cert_buffer, size_t buffer_size, size_t* actual_size);

/**
 * @brief Validate certificate and update metadata
 * 
 * @param cert_pem PEM-formatted certificate string
 * @param result Validation result (output)
 * @return esp_err_t ESP_OK on success
 */
esp_err_t cert_manager_validate(const char* cert_pem, cert_validation_result_t* result);

/**
 * @brief Get certificate metadata
 * 
 * @param metadata Certificate metadata (output)
 * @return esp_err_t ESP_OK on success
 */
esp_err_t cert_manager_get_metadata(cert_metadata_t* metadata);

/**
 * @brief Check if certificate exists and is valid
 * 
 * @return true if certificate exists and is valid
 */
bool cert_manager_is_certificate_valid(void);

/**
 * @brief Rotate certificate (backup current, store new)
 * 
 * @param new_cert_pem New PEM-formatted certificate
 * @param source Certificate source type
 * @return esp_err_t ESP_OK on success
 */
esp_err_t cert_manager_rotate(const char* new_cert_pem, cert_source_t source);

/**
 * @brief Get certificate source name (for logging)
 * 
 * @param source Certificate source type
 * @return const char* Source name string
 */
const char* cert_manager_get_source_name(cert_source_t source);

/**
 * @brief Deinitialize certificate manager
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t cert_manager_deinit(void);

#ifdef __cplusplus
}
#endif