#ifndef BLERC_H
#define BLERC_H

#include <Arduino.h>
#include <Arduino_JSON.h>

#ifndef IR_PIN
#define IR_PIN 23
#endif

#ifndef HTTP_PORT
#define HTTP_PORT 80
#endif

#ifndef WS_PORT
#define WS_PORT 81
#endif

#define DNS_PORT 53

#include <WebServer.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <iterator>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

#include <IRrecv.h>
#include <Preferences.h>

#include "Files.h"
#include "Keys.h"

#include "WebService.h"
#include "WSEvent.h"
#include "HTTPEvent.h"
#include "IRService.h"
#include "Bluetooth.h"

class BLERC
{
public:
    BLERC();
    static BLERC &instance()
    {
        static BLERC instance;
        return instance;
    }
    void setup();
    void loop();
};

#endif // BLERC_H
