#include "certificate_manager.h"
#include "ca_certificate.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_crc.h"
#include "esp_timer.h"
#include <string.h>
#include <time.h>

static const char* TAG = "CERT_MANAGER";

// Configuration and state
static cert_manager_config_t s_config = {0};
static bool s_initialized = false;

// NVS keys
#define NVS_KEY_PRIMARY_CERT    "primary_cert"
#define NVS_KEY_BACKUP_CERT     "backup_cert"
#define NVS_KEY_METADATA        "metadata"

// Maximum certificate size (8KB should be sufficient for most certificates)
#define MAX_CERT_SIZE           8192

/**
 * @brief Certificate source name mapping
 */
static const char* cert_source_names[] = {
    "MANUFACTURING",
    "OTA_UPDATE",
    "CONFIG_ENDPOINT",
    "DEVELOPMENT",
    "NONE"
};

/**
 * @brief Calculate CRC32 checksum for certificate integrity
 */
static uint32_t calculate_cert_checksum(const char* cert_pem)
{
    if (!cert_pem) return 0;
    return esp_crc32_le(0, (const uint8_t*)cert_pem, strlen(cert_pem));
}

/**
 * @brief Validate PEM certificate format
 */
static bool is_valid_pem_format(const char* cert_pem)
{
    if (!cert_pem) return false;
    
    // Check for PEM header and footer
    if (!strstr(cert_pem, "-----BEGIN CERTIFICATE-----") ||
        !strstr(cert_pem, "-----END CERTIFICATE-----")) {
        return false;
    }
    
    // Basic length check
    size_t len = strlen(cert_pem);
    if (len < 200 || len > MAX_CERT_SIZE) {
        return false;
    }
    
    return true;
}

/**
 * @brief Extract certificate expiry time (simplified implementation)
 * Note: This is a basic implementation. In production, use a proper
 * certificate parsing library like mbedTLS X.509 parser.
 */
static uint64_t extract_cert_expiry(const char* cert_pem)
{
    // TODO: Implement proper certificate parsing
    // For now, return a future timestamp (certificate expires in 1 year)
    return esp_timer_get_time() + (365ULL * 24 * 60 * 60 * 1000000); // 1 year in microseconds
}

/**
 * @brief Store certificate metadata in NVS
 */
static esp_err_t store_cert_metadata(const cert_metadata_t* metadata)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(s_config.nvs_namespace, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS for metadata storage: %s", esp_err_to_name(err));
        return err;
    }
    
    err = nvs_set_blob(nvs_handle, NVS_KEY_METADATA, metadata, sizeof(cert_metadata_t));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to store certificate metadata: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Certificate metadata stored successfully");
        nvs_commit(nvs_handle);
    }
    
    nvs_close(nvs_handle);
    return err;
}

/**
 * @brief Load certificate metadata from NVS
 */
static esp_err_t load_cert_metadata(cert_metadata_t* metadata)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(s_config.nvs_namespace, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }
    
    size_t required_size = sizeof(cert_metadata_t);
    err = nvs_get_blob(nvs_handle, NVS_KEY_METADATA, metadata, &required_size);
    nvs_close(nvs_handle);
    
    return err;
}

/**
 * @brief Store certificate in NVS with key
 */
static esp_err_t store_cert_in_nvs(const char* key, const char* cert_pem)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(s_config.nvs_namespace, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS for certificate storage: %s", esp_err_to_name(err));
        return err;
    }
    
    err = nvs_set_str(nvs_handle, key, cert_pem);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to store certificate: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Certificate stored successfully with key: %s", key);
        nvs_commit(nvs_handle);
    }
    
    nvs_close(nvs_handle);
    return err;
}

/**
 * @brief Load certificate from NVS with key
 */
static esp_err_t load_cert_from_nvs(const char* key, char* cert_buffer, size_t buffer_size, size_t* actual_size)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(s_config.nvs_namespace, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }
    
    size_t required_size = 0;
    err = nvs_get_str(nvs_handle, key, NULL, &required_size);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }
    
    if (required_size > buffer_size) {
        nvs_close(nvs_handle);
        return ESP_ERR_INVALID_SIZE;
    }
    
    err = nvs_get_str(nvs_handle, key, cert_buffer, &required_size);
    if (actual_size) {
        *actual_size = required_size;
    }
    
    nvs_close(nvs_handle);
    return err;
}

// Public API implementation

esp_err_t cert_manager_init(const cert_manager_config_t* config)
{
    if (s_initialized) {
        ESP_LOGW(TAG, "Certificate manager already initialized");
        return ESP_OK;
    }
    
    if (!config) {
        ESP_LOGE(TAG, "Invalid configuration");
        return ESP_ERR_INVALID_ARG;
    }
    
    s_config = *config;
    s_initialized = true;
    
    ESP_LOGI(TAG, "Certificate manager initialized");
    ESP_LOGI(TAG, "  - Development cert allowed: %s", s_config.allow_development_cert ? "yes" : "no");
    ESP_LOGI(TAG, "  - Integrity check required: %s", s_config.require_integrity_check ? "yes" : "no");
    ESP_LOGI(TAG, "  - Auto-rotate expired: %s", s_config.auto_rotate_expired ? "yes" : "no");
    ESP_LOGI(TAG, "  - NVS namespace: %s", s_config.nvs_namespace);
    
    return ESP_OK;
}

esp_err_t cert_manager_store(const char* cert_pem, cert_source_t source)
{
    if (!s_initialized) {
        ESP_LOGE(TAG, "Certificate manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (!cert_pem || source >= CERT_SOURCE_MAX) {
        ESP_LOGE(TAG, "Invalid arguments");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Validate certificate format
    if (!is_valid_pem_format(cert_pem)) {
        ESP_LOGE(TAG, "Invalid PEM certificate format");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Create metadata
    cert_metadata_t metadata = {
        .source = source,
        .checksum = calculate_cert_checksum(cert_pem),
        .stored_time = esp_timer_get_time(),
        .expiry_time = extract_cert_expiry(cert_pem),
        .cert_size = strlen(cert_pem),
        .is_valid = true
    };
    
    // Store certificate and metadata
    esp_err_t err = store_cert_in_nvs(NVS_KEY_PRIMARY_CERT, cert_pem);
    if (err != ESP_OK) {
        return err;
    }
    
    err = store_cert_metadata(&metadata);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Failed to store metadata, but certificate stored successfully");
    }
    
    ESP_LOGI(TAG, "Certificate stored successfully (source: %s, size: %d bytes)", 
             cert_manager_get_source_name(source), metadata.cert_size);
    
    return ESP_OK;
}

esp_err_t cert_manager_load(char* cert_buffer, size_t buffer_size, size_t* actual_size)
{
    if (!s_initialized) {
        ESP_LOGE(TAG, "Certificate manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (!cert_buffer || buffer_size == 0) {
        ESP_LOGE(TAG, "Invalid buffer arguments");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Try to load primary certificate
    esp_err_t err = load_cert_from_nvs(NVS_KEY_PRIMARY_CERT, cert_buffer, buffer_size, actual_size);
    if (err == ESP_OK) {
        // Validate loaded certificate
        cert_validation_result_t validation_result;
        esp_err_t validation_err = cert_manager_validate(cert_buffer, &validation_result);
        
        if (validation_err == ESP_OK && validation_result == CERT_VALID) {
            ESP_LOGI(TAG, "Primary certificate loaded and validated successfully");
            return ESP_OK;
        } else {
            ESP_LOGW(TAG, "Primary certificate validation failed: %d", validation_result);
        }
    }
    
    // Try backup certificate
    err = load_cert_from_nvs(NVS_KEY_BACKUP_CERT, cert_buffer, buffer_size, actual_size);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Backup certificate loaded successfully");
        return ESP_OK;
    }
    
    // Fallback to development certificate if allowed
    if (s_config.allow_development_cert) {
        size_t dev_cert_len = strlen(DEMO_CA_CERTIFICATE_PEM);
        if (dev_cert_len < buffer_size) {
            strncpy(cert_buffer, DEMO_CA_CERTIFICATE_PEM, buffer_size - 1);
            cert_buffer[buffer_size - 1] = '\\0';
            if (actual_size) {
                *actual_size = dev_cert_len;
            }
            ESP_LOGW(TAG, "Using development certificate fallback");
            return ESP_OK;
        }
    }
    
    ESP_LOGE(TAG, "No valid certificate found");
    return ESP_ERR_NOT_FOUND;
}

esp_err_t cert_manager_validate(const char* cert_pem, cert_validation_result_t* result)
{
    if (!s_initialized) {
        ESP_LOGE(TAG, "Certificate manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (!cert_pem || !result) {
        ESP_LOGE(TAG, "Invalid arguments");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Check PEM format
    if (!is_valid_pem_format(cert_pem)) {
        *result = CERT_INVALID_FORMAT;
        return ESP_OK;
    }
    
    // Check integrity if required
    if (s_config.require_integrity_check) {
        cert_metadata_t metadata;
        esp_err_t err = load_cert_metadata(&metadata);
        if (err == ESP_OK) {
            uint32_t current_checksum = calculate_cert_checksum(cert_pem);
            if (current_checksum != metadata.checksum) {
                *result = CERT_INTEGRITY_FAILED;
                ESP_LOGW(TAG, "Certificate integrity check failed");
                return ESP_OK;
            }
        }
    }
    
    // Check expiry (simplified)
    uint64_t current_time = esp_timer_get_time();
    uint64_t expiry_time = extract_cert_expiry(cert_pem);
    if (current_time > expiry_time) {
        *result = CERT_EXPIRED;
        ESP_LOGW(TAG, "Certificate has expired");
        return ESP_OK;
    }
    
    *result = CERT_VALID;
    return ESP_OK;
}

esp_err_t cert_manager_get_metadata(cert_metadata_t* metadata)
{
    if (!s_initialized) {
        ESP_LOGE(TAG, "Certificate manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (!metadata) {
        ESP_LOGE(TAG, "Invalid metadata pointer");
        return ESP_ERR_INVALID_ARG;
    }
    
    return load_cert_metadata(metadata);
}

bool cert_manager_is_certificate_valid(void)
{
    if (!s_initialized) {
        return false;
    }
    
    static char cert_buffer[MAX_CERT_SIZE];
    size_t actual_size;
    
    esp_err_t err = cert_manager_load(cert_buffer, sizeof(cert_buffer), &actual_size);
    if (err != ESP_OK) {
        return false;
    }
    
    cert_validation_result_t result;
    err = cert_manager_validate(cert_buffer, &result);
    
    return (err == ESP_OK && result == CERT_VALID);
}

esp_err_t cert_manager_rotate(const char* new_cert_pem, cert_source_t source)
{
    if (!s_initialized) {
        ESP_LOGE(TAG, "Certificate manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (!new_cert_pem || source >= CERT_SOURCE_MAX) {
        ESP_LOGE(TAG, "Invalid arguments");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Validate new certificate
    cert_validation_result_t validation_result;
    esp_err_t err = cert_manager_validate(new_cert_pem, &validation_result);
    if (err != ESP_OK || validation_result != CERT_VALID) {
        ESP_LOGE(TAG, "New certificate validation failed: %d", validation_result);
        return ESP_ERR_INVALID_ARG;
    }
    
    // Backup current certificate
    static char current_cert[MAX_CERT_SIZE];
    size_t current_size;
    err = load_cert_from_nvs(NVS_KEY_PRIMARY_CERT, current_cert, sizeof(current_cert), &current_size);
    if (err == ESP_OK) {
        store_cert_in_nvs(NVS_KEY_BACKUP_CERT, current_cert);
        ESP_LOGI(TAG, "Current certificate backed up");
    }
    
    // Store new certificate
    err = cert_manager_store(new_cert_pem, source);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to store new certificate");
        return err;
    }
    
    ESP_LOGI(TAG, "Certificate rotated successfully (source: %s)", 
             cert_manager_get_source_name(source));
    
    return ESP_OK;
}

const char* cert_manager_get_source_name(cert_source_t source)
{
    if (source >= CERT_SOURCE_MAX) {
        return "UNKNOWN";
    }
    return cert_source_names[source];
}

esp_err_t cert_manager_deinit(void)
{
    if (!s_initialized) {
        ESP_LOGW(TAG, "Certificate manager not initialized");
        return ESP_OK;
    }
    
    s_initialized = false;
    memset(&s_config, 0, sizeof(s_config));
    
    ESP_LOGI(TAG, "Certificate manager deinitialized");
    return ESP_OK;
}