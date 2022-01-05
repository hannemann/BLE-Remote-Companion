#ifndef BLERC_H
#define BLERC_H

#include <Arduino.h>
#include <Arduino_JSON.h>

#if (DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP > 0)
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#endif

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
#include <WebSocketsClient.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Update.h>
#include <iterator>

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_BACK 8
#define MOUSE_FORWARD 16
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE) // For compatibility with the Mouse library

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

#include <IRrecv.h>
#include <IRutils.h>
#include <Preferences.h>

#include "Files.h"
#include "Keys.h"

#include "WebService.h"
#include "WSEvent.h"
#include "WSClient.h"
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
    static Preferences preferences;
    static String room;
    static String configJSON;

private:
    void readConfig();
};

#endif // BLERC_H
