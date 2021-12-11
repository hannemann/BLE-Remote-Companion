#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsServer.h>
#include <Arduino_JSON.h>
#include "Bluetooth.h"
#include "KeyServer.h"
#include "WSEvent.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiMulti wifi;
