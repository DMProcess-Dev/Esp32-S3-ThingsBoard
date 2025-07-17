#pragma once

/**
 * @file ca_certificate.h
 * @brief CA Certificate for ThingsBoard/Mosquitto SSL/TLS connections
 * 
 * @warning DEMO PURPOSES ONLY - NOT FOR PRODUCTION USE
 * 
 * In production environments, certificates should be:
 * - Provisioned during manufacturing
 * - Loaded from secure flash partition
 * - Updated via OTA with proper validation
 * - Never hardcoded in source code
 * 
 * This certificate is used for development and demonstration only.
 */

/**
 * @brief ThingsBoard/Mosquitto CA Certificate (PEM format)
 * 
 * This certificate is used to validate the MQTT broker's SSL/TLS certificate.
 * Generated for demonstration purposes with validity until 2026-07-15.
 * 
 * @note This is a development certificate and should be replaced in production
 */
extern const char* const DEMO_CA_CERTIFICATE;

#define DEMO_CA_CERTIFICATE_PEM \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDizCCAnOgAwIBAgIUbQWnmmWcibAjdXSGTkCNHFlSp6AwDQYJKoZIhvcNAQEL\n" \
    "BQAwVTELMAkGA1UEBhMCVVMxDjAMBgNVBAgMBVN0YXRlMQ0wCwYDVQQHDARDaXR5\n" \
    "MRUwEwYDVQQKDAxPcmdhbml6YXRpb24xEDAOBgNVBAMMB01RVFQtQ0EwHhcNMjUw\n" \
    "NzE1MjE0NzUwWhcNMjYwNzE1MjE0NzUwWjBVMQswCQYDVQQGEwJVUzEOMAwGA1UE\n" \
    "CAwFU3RhdGUxDTALBgNVBAcMBENpdHkxFTATBgNVBAoMDE9yZ2FuaXphdGlvbjEQ\n" \
    "MA4GA1UEAwwHTVFUVC1DQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
    "AIzn6YZ9XmNq6ziQe9s0gEzvy/ulHgtE9dzlQTXbuloI8o5EDX4TDIuF7ijgGxIj\n" \
    "PBugxtjBA2cmw8RwlLVWiNqBwVaoxE1a30OL6errTJLzIwGbVz4I7N7afvfSvT5O\n" \
    "F4cx5UGOIC2pZlNxNpqpMcpZryPt6pmVBRtR2q66TrkbOPLTTij2UUcvzQJHDDNx\n" \
    "01SbFBvaFWTM7NpI3beeXVNpQ+A7o2lhWbMYO468eJA5PuX615mrp+hbXB7wiGN/\n" \
    "wuLl8rcBMA15JUDiUntfdCFVSszJyw2e6AQnUCFf5kaihy3Kf/Eh61ACOGEBxoPn\n" \
    "dDyvHBRlumLJZTZWka4UOFkCAwEAAaNTMFEwHQYDVR0OBBYEFFmLyOCgVpnH6oMJ\n" \
    "sL2ESvFpUeZCMB8GA1UdIwQYMBaAFFmLyOCgVpnH6oMJsL2ESvFpUeZCMA8GA1Ud\n" \
    "EwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAC8tXrFXnptWfaavQPsakQ8w\n" \
    "spkHvCV1t0YBT2w/fCwS/XH84pOqxj67qeDs4cj9dKnbf1bG6PUmuH/Fi/lK5HEW\n" \
    "pU1kHbj3hV9qHERp5dtxBVNqYIMAmoSrspI72fHGNNFCYebdsuhcnXLI4UGRlmt1\n" \
    "0SU/CFdM/S86aArlb48DCebhTY1WnNPD7oDmlURv6JBmnm2KqgN6KVEjp3cQBGMh\n" \
    "btTQPvvU6WGTiFv74WHbvfQxz/hX2mVkJDomjU79EeejUXzkTClB2PTyRTQaIvWH\n" \
    "EtepHpNKigkheeDUZQbe5mGyE9JSbWltJqSIJw+FFJvv20tyDnP3V1kdNjjaG7s=\n" \
    "-----END CERTIFICATE-----\n"
