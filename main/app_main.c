/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_http_server.h"
#include "provisioning_page.h"
#include "sys/param.h"
#include "cJSON.h"
#include "netdb.h"
#include "driver/temperature_sensor.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define ARGB_LED_GPIO 48
#define MAX_BRIGHTNESS 50 // 0-255

static led_strip_handle_t s_led_strip;

static void init_led(void)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = ARGB_LED_GPIO,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &s_led_strip));
    led_strip_clear(s_led_strip);
}

static void set_led_status(uint32_t red, uint32_t green, uint32_t blue)
{
    led_strip_set_pixel(s_led_strip, 0, (red * MAX_BRIGHTNESS) / 255, (green * MAX_BRIGHTNESS) / 255, (blue * MAX_BRIGHTNESS) / 255);
    led_strip_refresh(s_led_strip);
}

static const char *TAG = "PROVISIONING_EXAMPLE";


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
#define LED_GPIO 48

static void telemetry_task(void *pvParameters)
{
    esp_mqtt_client_handle_t client = (esp_mqtt_client_handle_t)pvParameters;

    // Initialize temperature sensor
    temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(20, 50);
    temperature_sensor_handle_t temp_handle = NULL;
    ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_handle));
    ESP_ERROR_CHECK(temperature_sensor_enable(temp_handle));

    while (1) {
        float temperature;
        wifi_ap_record_t ap_info;
        int32_t rssi = 0;
        uint32_t free_heap = esp_get_free_heap_size();
        int64_t uptime = esp_timer_get_time() / 1000000; // seconds

        // Get temperature
        ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_handle, &temperature));

        // Get RSSI
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            rssi = ap_info.rssi;
        }

        // Create JSON payload
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "temperature", temperature);
        cJSON_AddNumberToObject(root, "rssi", rssi);
        cJSON_AddNumberToObject(root, "heap", free_heap);
        cJSON_AddNumberToObject(root, "uptime", uptime);

        char *json_payload = cJSON_Print(root);
        esp_mqtt_client_publish(client, "v1/devices/me/telemetry", json_payload, 0, 1, 0);
        cJSON_Delete(root);
        free(json_payload);

        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 seconds
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        set_led_status(0, 255, 0); // Green for MQTT connected
        xTaskCreate(telemetry_task, "telemetry_task", 4096, client, 5, NULL);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        set_led_status(255, 0, 0); // Red for error
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        set_led_status(255, 0, 0); // Red for error
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    nvs_handle_t nvs_handle;
    ESP_ERROR_CHECK(nvs_open("wifi_creds", NVS_READONLY, &nvs_handle));

    char mqtt_host[64];
    char mqtt_port_str[8];
    char mqtt_user[32];
    char mqtt_pass[32];
    size_t len;

    len = sizeof(mqtt_host);
    ESP_ERROR_CHECK(nvs_get_str(nvs_handle, "mqtt_host", mqtt_host, &len));
    len = sizeof(mqtt_port_str);
    ESP_ERROR_CHECK(nvs_get_str(nvs_handle, "mqtt_port", mqtt_port_str, &len));
    len = sizeof(mqtt_user);
    nvs_get_str(nvs_handle, "mqtt_user", mqtt_user, &len);
    len = sizeof(mqtt_pass);
    nvs_get_str(nvs_handle, "mqtt_pass", mqtt_pass, &len);

    nvs_close(nvs_handle);

    char uri[128];
    snprintf(uri, sizeof(uri), "mqtt://%s:%s", mqtt_host, mqtt_port_str);

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = uri,
        .credentials.username = mqtt_user,
        .credentials.authentication.password = mqtt_pass,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

/* HTTP server handlers */
static esp_err_t wifi_scan_get_handler(httpd_req_t *req)
{
    wifi_scan_config_t scan_config = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0,
        .show_hidden = false
    };
    // Blocking scan
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);
    if (ap_count == 0) {
        ESP_LOGI(TAG, "No APs found");
        httpd_resp_send(req, "[]", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    wifi_ap_record_t *ap_info = (wifi_ap_record_t *)malloc(ap_count * sizeof(wifi_ap_record_t));
    if (ap_info == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for AP info");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_info));

    cJSON *root = cJSON_CreateArray();
    for (int i = 0; i < ap_count; i++) {
        cJSON *ap_json = cJSON_CreateObject();
        cJSON_AddStringToObject(ap_json, "ssid", (const char *)ap_info[i].ssid);
        cJSON_AddNumberToObject(ap_json, "rssi", ap_info[i].rssi);
        cJSON_AddNumberToObject(ap_json, "authmode", ap_info[i].authmode);
        cJSON_AddItemToArray(root, ap_json);
    }

    const char *json_string = cJSON_Print(root);
    if (json_string == NULL) {
        ESP_LOGE(TAG, "Failed to print cJSON to string");
        cJSON_Delete(root);
        free(ap_info);
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, json_string, strlen(json_string));

    cJSON_Delete(root);
    free(ap_info);
    free((void *)json_string);

    return ESP_OK;
}

typedef enum {
    STATUS_IDLE,
    STATUS_CONNECTING,
    STATUS_CONNECTED,
    STATUS_CONNECT_FAILED,
} connection_status_t;

static connection_status_t s_connection_status = STATUS_IDLE;

static esp_err_t status_get_handler(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "status", s_connection_status);
    const char *json_string = cJSON_Print(root);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, json_string, strlen(json_string));
    cJSON_Delete(root);
    free((void *)json_string);
    return ESP_OK;
}

static esp_err_t favicon_get_handler(httpd_req_t *req)
{
    httpd_resp_set_status(req, "204 No Content");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Cache-Control", "no-store");
    httpd_resp_send(req, provisioning_html, strlen(provisioning_html));
    return ESP_OK;
}

httpd_handle_t server = NULL;






static esp_err_t connect_post_handler(httpd_req_t *req)
{
    char buf[512];
    int ret, remaining = req->content_len;

    if (remaining >= sizeof(buf)) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Request too long");
        return ESP_FAIL;
    }

    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            return ESP_FAIL;
        }
        remaining -= ret;
    }
    buf[req->content_len] = '\0';

    char ssid[32] = {0};
    char password[64] = {0};
    char mqtt_host[64] = {0};
    char mqtt_port_str[8] = {0};
    char mqtt_user[32] = {0};
    char mqtt_pass[32] = {0};

    if (httpd_query_key_value(buf, "ssid", ssid, sizeof(ssid)) != ESP_OK ||
        httpd_query_key_value(buf, "password", password, sizeof(password)) != ESP_OK ||
        httpd_query_key_value(buf, "mqtt_host", mqtt_host, sizeof(mqtt_host)) != ESP_OK ||
        httpd_query_key_value(buf, "mqtt_port", mqtt_port_str, sizeof(mqtt_port_str)) != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing required fields");
        return ESP_FAIL;
    }

    // Optional fields
    httpd_query_key_value(buf, "mqtt_user", mqtt_user, sizeof(mqtt_user));
    httpd_query_key_value(buf, "mqtt_pass", mqtt_pass, sizeof(mqtt_pass));

    ESP_LOGI(TAG, "Received SSID: %s", ssid);
    ESP_LOGI(TAG, "Received MQTT Host: %s", mqtt_host);

    // Store credentials in NVS
    nvs_handle_t nvs_handle;
    ESP_ERROR_CHECK(nvs_open("wifi_creds", NVS_READWRITE, &nvs_handle));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "ssid", ssid));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "password", password));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "mqtt_host", mqtt_host));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "mqtt_port", mqtt_port_str));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "mqtt_user", mqtt_user));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "mqtt_pass", mqtt_pass));
    ESP_ERROR_CHECK(nvs_commit(nvs_handle));
    nvs_close(nvs_handle);

    // Configure Wi-Fi Station
    wifi_config_t wifi_config = {0};
    strlcpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strlcpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    s_connection_status = STATUS_CONNECTING;
    ESP_ERROR_CHECK(esp_wifi_connect());

    // Send response to client
    const char *resp_str = "Wi-Fi credentials received. Attempting to connect...";
    httpd_resp_send(req, resp_str, strlen(resp_str));

    return ESP_OK;
}


static httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        httpd_uri_t wifi_scan_uri = {
            .uri       = "/api/wifi-scan",
            .method    = HTTP_GET,
            .handler   = wifi_scan_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &wifi_scan_uri);

        httpd_uri_t favicon_uri = {
            .uri       = "/favicon.ico",
            .method    = HTTP_GET,
            .handler   = favicon_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &favicon_uri);

        httpd_uri_t status_uri = {
            .uri       = "/api/status",
            .method    = HTTP_GET,
            .handler   = status_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &status_uri);

        httpd_uri_t root = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &root);

        httpd_uri_t connect = {
            .uri       = "/connect",
            .method    = HTTP_POST,
            .handler   = connect_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &connect);
    }
    return server;
}

static void start_provisioning_server(void)
{
    ESP_LOGI(TAG, "Starting provisioning mode");
    set_led_status(50, 50, 50); // White for provisioning

    esp_netif_create_default_wifi_ap();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Clear any stored STA credentials
    wifi_config_t sta_config = {0};
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));

    wifi_config_t ap_config = {
        .ap = {
            .ssid = "ESP32-Provisioning",
            .ssid_len = strlen("ESP32-Provisioning"),
            .password = "password",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen((char *)ap_config.ap.password) == 0) {
        ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi AP started. SSID:%s password:%s",
             "ESP32-Provisioning", "password");

    start_webserver();
}



static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        set_led_status(0, 0, 255); // Blue for connecting
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected from Wi-Fi, trying to reconnect...");
        s_connection_status = STATUS_CONNECT_FAILED;
        set_led_status(255, 0, 0); // Red for error
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        s_connection_status = STATUS_CONNECTED;
        set_led_status(0, 255, 0); // Green for connected

        // Stop the provisioning AP and webserver
        esp_wifi_set_mode(WIFI_MODE_STA);
        if (server) {
            httpd_stop(server);
            server = NULL;
        }

        ESP_LOGI(TAG, "Checking internet connectivity...");
        const struct addrinfo hints = {
            .ai_family = AF_INET,
            .ai_socktype = SOCK_STREAM,
        };
        struct addrinfo *res;
        int err = getaddrinfo("www.google.com", "80", &hints, &res);
        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "Internet connectivity check failed. DNS lookup for google.com failed.");
            set_led_status(255, 0, 0); // Red for error
        } else {
            ESP_LOGI(TAG, "Internet connectivity check successful.");
            freeaddrinfo(res);
        }

        mqtt_app_start();
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("PROVISIONING_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("transport", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    init_led();

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    start_provisioning_server();
}
