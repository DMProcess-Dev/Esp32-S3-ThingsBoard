idf_component_register(SRCS "app_main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES mqtt esp_http_server json nvs_flash esp_netif esp_wifi esp_event)
