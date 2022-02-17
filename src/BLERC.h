#ifndef BLERC_H
#define BLERC_H

#include <Arduino.h>
#include <Arduino_JSON.h>

#ifndef DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP
#define DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP 1
#endif

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

#if WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32_ETH
#include <ETH.h>
#define WEBSOCKETS_ETH 1
#define ETH_CLK_MODE ETH_CLOCK_GPIO0_IN //  ETH_CLOCK_GPIO17_OUT
// Pin# of the enable signal for the external crystal oscillator (-1 to disable for internal APLL source)
#define ETH_POWER_PIN 16
// Type of the Ethernet PHY (LAN8720 or TLK110)
#define ETH_TYPE ETH_PHY_LAN8720
// I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_ADDR 1
// Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDC_PIN 23
// Pin# of the I²C IO signal for the Ethernet PHY
#define ETH_MDIO_PIN 18
#endif

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

#include "Logger.h"
#include "Files.h"
#include "Keys.h"

#include "WebService.h"
#include "WSEvent.h"
#include "HAClient.h"
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
    static uint8_t kb_layout;
    static bool ha_api_enable;
    static bool ha_send_assigned;
    static String ha_ip;
    static uint16_t ha_port;
    static String ha_token;
    static bool ir_ign_unknown;
    static bool ws_br_enable;
    static bool ws_br_send_assigned;
    static String configJSON;
    static String remoteMappingsJSON;
    static JSONVar remoteMappings;
    static bool addRemoteMapping(JSONVar &params);
    static bool removeRemoteMapping(JSONVar &params);
    static bool deleteRemoteMappings();
    static void saveConfig(JSONVar &params);

private:
    void readConfig();
    void readMappings();
};

#endif // BLERC_H
