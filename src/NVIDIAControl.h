#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsServer.h>
#include <Arduino_JSON.h>
#include <Keys.h>
#include <Keyboard.h>
#include <Bluetooth.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const int webSocketPort = WS_PORT;

uint16_t counter = 1;

struct IPAddressFail {
int FailedAttempts;
time_t lastFailure;
};

WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(webSocketPort);

/* Try to block users who are trying to brute force a connection */
IPAddressFail IPAddressFailures[254];

