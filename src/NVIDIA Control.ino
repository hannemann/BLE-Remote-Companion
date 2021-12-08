//Your wifi connection
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
/*Change the port to a random (free) port for security.*/
const int webSocketPort = 2339;

//Websocket Initialization
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsServer.h>

WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(webSocketPort);

//JSON Parsing and Key Conversion
#include <Arduino_JSON.h>
//int playerpause = 2;
//int oldplayerpause
/* Assume media is playing to begin with */
bool playing = true;
uint16_t counter = 1;

/*  Struct to Hold Each Element of the JsonToUSB_HID_Map */
struct JSONMethodToCecType {
  char JSONMethod[24];
  char JSONAction[14];
  bool KeyboardAction;
  bool LongPress;
  uint8_t USBHID;
};

struct IPAddressFail {
  int FailedAttempts;
  time_t lastFailure;
};

/* Try to block users who are trying to brute force a connection */
IPAddressFail IPAddressFailures[254];

/*  JsonToUSB_HID_Map */
const JSONMethodToCecType JSONMethodToCec[] = {
  {"Input.Select",                  "",                 1, 0, 0x28},
  {"Input.SelectLongpress",         "",                 1, 1, 0x28},
  {"Input.Left",                    "",                 1, 0, 0x50},
  {"Input.Right",                   "",                 1, 0, 0x4F},
  {"Input.Up",                      "",                 1, 0, 0x52},
  {"Input.Down",                    "",                 1, 0, 0x51},
  {"Input.ShowOSD",                 "",                 1, 0, 0x10},
  {"Input.Info",                    "",                 0, 0, 0},
  {"Input.Back",                    "",                 1, 0, 0x29},
  {"Input.Home",                    "",                 1, 0, 0x29},

  {"Input.SendText",                "",                 0, 0, 0},
  {"VideoLibrary.Scan",             "",                 0, 0, 0},
  {"Input.ContextMenu",             "",                 1, 0, 0x65},
  {"Input.ExecuteAction",           "volumeup",         0, 0, 0xE9},
  {"Input.ExecuteAction",           "volumedown",       0, 0, 0xEA},
  {"Input.ExecuteAction",           "mute",             0, 0, 0xE2},
  {"Input.ExecuteAction",           "appswitch",        0, 0, 0xBB},
  {"Input.ExecuteAction",           "play",             0, 0, 0xCD},
  {"Input.ExecuteAction",           "stop",             0, 0, 0xB7},
  {"Input.ExecuteAction",           "stepback",         0, 0, 0xB4},

  {"Input.ExecuteAction",           "stepforward",      0, 0, 0xB3},
  {"Input.ExecuteAction",           "skipprevious",     0, 0, 0xB7},
  {"Input.ExecuteAction",           "skipnext",         0, 0, 0xB5}
};

//Keyboard Initialize
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

struct inputConsumer_t
{
  uint16_t ConsumerControl;                          // Value = 0 to 572
};

struct inputKeyboard_t
{
  uint8_t  KB_KeyboardKeyboardLeftControl  : 1;       // Usage 0x000700E0: Keyboard Left Control, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardLeftShift  : 1;         // Usage 0x000700E1: Keyboard Left Shift, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardLeftAlt    : 1;           // Usage 0x000700E2: Keyboard Left Alt, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardLeftGui    : 1;           // Usage 0x000700E3: Keyboard Left GUI, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightControl : 1;      // Usage 0x000700E4: Keyboard Right Control, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightShift   : 1;        // Usage 0x000700E5: Keyboard Right Shift, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightAlt   : 1;          // Usage 0x000700E6: Keyboard Right Alt, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightGui   : 1;          // Usage 0x000700E7: Keyboard Right GUI, Value = 0 to 1
  uint8_t  Key;                                 // Value = 0 to 101
};

// Report IDs:
#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02

const uint8_t keyboardHidDescriptor[] = {
  USAGE_PAGE(1),      0x01,          // USAGE_PAGE (Generic Desktop Ctrls)
  USAGE(1),           0x06,          // USAGE (Keyboard)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  // ------------------------------------------------- Keyboard
  REPORT_ID(1),       KEYBOARD_ID,   //   REPORT_ID (1)
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0xE0,          //   USAGE_MINIMUM (0xE0)
  USAGE_MAXIMUM(1),   0xE7,          //   USAGE_MAXIMUM (0xE7)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   Logical Maximum (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x08,          //   REPORT_COUNT (8)
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 1 byte (Reserved)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE (8)
  HIDINPUT(1),        0x01,          //   INPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x05,          //   REPORT_COUNT (5) ; 5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  USAGE_PAGE(1),      0x08,          //   USAGE_PAGE (LEDs)
  USAGE_MINIMUM(1),   0x01,          //   USAGE_MINIMUM (0x01) ; Num Lock
  USAGE_MAXIMUM(1),   0x05,          //   USAGE_MAXIMUM (0x05) ; Kana
  HIDOUTPUT(1),       0x02,          //   OUTPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 3 bits (Padding)
  REPORT_SIZE(1),     0x03,          //   REPORT_SIZE (3)
  HIDOUTPUT(1),       0x01,          //   OUTPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x06,          //   REPORT_COUNT (6) ; 6 bytes (Keys)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE(8)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM(0)
  LOGICAL_MAXIMUM(1), 0x65,          //   LOGICAL_MAXIMUM(0x65) ; 101 keys
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0)
  USAGE_MAXIMUM(1),   0x65,          //   USAGE_MAXIMUM (0x65)
  HIDINPUT(1),        0x00,          //   INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0),                 // END_COLLECTION
  // ------------------------------------------------- Media Keys
  USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
  USAGE(1),           0x01,          // USAGE (Consumer Control)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  REPORT_ID(1),       MEDIA_KEYS_ID, //   REPORT_ID (3)
  USAGE_PAGE(1),      0x0C,          //   USAGE_PAGE (Consumer)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x10,          //   REPORT_COUNT (16)
  USAGE(1),           0xB5,          //   USAGE (Scan Next Track)     ; bit 0: 1
  USAGE(1),           0xB6,          //   USAGE (Scan Previous Track) ; bit 1: 2
  USAGE(1),           0xB7,          //   USAGE (Stop)                ; bit 2: 4
  USAGE(1),           0xCD,          //   USAGE (Play/Pause)          ; bit 3: 8
  USAGE(1),           0xE2,          //   USAGE (Mute)                ; bit 4: 16
  USAGE(1),           0xE9,          //   USAGE (Volume Increment)    ; bit 5: 32
  USAGE(1),           0xEA,          //   USAGE (Volume Decrement)    ; bit 6: 64
  USAGE(2),           0x23, 0x02,    //   Usage (WWW Home)            ; bit 7: 128
  USAGE(2),           0x94, 0x01,    //   Usage (My Computer) ; bit 0: 1
  USAGE(2),           0x92, 0x01,    //   Usage (Calculator)  ; bit 1: 2
  USAGE(2),           0x2A, 0x02,    //   Usage (WWW fav)     ; bit 2: 4
  USAGE(2),           0x21, 0x02,    //   Usage (WWW search)  ; bit 3: 8
  USAGE(2),           0x26, 0x02,    //   Usage (WWW stop)    ; bit 4: 16
  USAGE(2),           0x24, 0x02,    //   Usage (WWW back)    ; bit 5: 32
  USAGE(2),           0x83, 0x01,    //   Usage (Media sel)   ; bit 6: 64
  USAGE(2),           0x8A, 0x01,    //   Usage (Mail)        ; bit 7: 128
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0)                  // END_COLLECTION
};

// static inputConsumer_t consumer_Report{};
static inputKeyboard_t keyboard_report{}; // sent to PC
BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
BLECharacteristic* inputMedia;
BLECharacteristic* outputMedia;
bool BLEconnected = false;

void keyboard_control(uint8_t Key, const char* JSONMethod, bool longpress) {
  Serial.print("Sending key code :");
  Serial.println(Key);
  inputKeyboard_t keyboard{};
  if (strcmp(JSONMethod, "Input.Home") == 0) {
    keyboard.KB_KeyboardKeyboardLeftControl = 1;
  }
  keyboard.Key = Key;
  input->setValue((uint8_t*)&keyboard, sizeof(keyboard));
  input->notify();
  if (longpress) {
    for(int i = 0; i < 30; i++) {
      delay(10);
      input->setValue((uint8_t*)&keyboard, sizeof(keyboard));
      input->notify();
    }
  }
  input->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
  input->notify();
  delay(10);
}

void media_control(uint8_t Key) {
  Serial.print("Sending media code :");
  Serial.println(Key);
  inputConsumer_t keyboard{};
  keyboard.ConsumerControl = Key;
  inputMedia->setValue((uint8_t*)&keyboard, sizeof(keyboard));
  inputMedia->notify();
  inputMedia->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
  inputMedia->notify();
  delay(10);
}

void processUSBHID(JSONVar JSONMethod, const char* JSONAction) {
  int JSONMethodToCecLength = 23;
  Serial.printf("Incoming JSON Request: %s / %s\n", (const char *)JSONMethod, (const char *)JSONAction);
  // Serial.printf("Found %d possible candidates\n", (int)JSONMethodToCecLength);
  for (int i = 0; i < JSONMethodToCecLength; i++) {
    // Serial.printf("Candidate: %s / %s\n", JSONMethodToCec[i].JSONMethod, JSONMethodToCec[i].JSONAction);
    if ((strcmp(JSONMethodToCec[i].JSONMethod, (const char *)JSONMethod) == 0) && (strcmp(JSONMethodToCec[i].JSONAction, JSONAction) == 0)) {
      Serial.println("JSON valid");
      if (JSONMethodToCec[i].KeyboardAction == 1) {
        keyboard_control(JSONMethodToCec[i].USBHID, (const char *)JSONMethod, JSONMethodToCec[i].LongPress);
      } else {
        media_control(JSONMethodToCec[i].USBHID);
      }
      return;
    }
  }
  Serial.println("JSON invalid");
}

class BLECallback : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      BLEconnected = true;
      Serial.println("Bluetooth Connected");
      BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      desc->setNotifications(true);

      BLE2902* descv = (BLE2902*)inputMedia->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      descv->setNotifications(true);
    }

    void onDisconnect(BLEServer* pServer) {
      BLEconnected = false;
      Serial.println("BLE Disonnected");
      BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      desc->setNotifications(false);

      BLE2902* descv = (BLE2902*)inputMedia->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      descv->setNotifications(false);
    }
};

void keyTaskServer(void*) {
  BLEDevice::init("NVIDIA Control Device");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLECallback());

  hid = new BLEHIDDevice(pServer);
  inputMedia = hid->inputReport(1); // <-- input REPORTID from report map
  outputMedia = hid->outputReport(1); // <-- output REPORTID from report map

  input = hid->inputReport(2); // <-- input REPORTID from report map
  output = hid->outputReport(2); // <-- output REPORTID from report map

  std::string name = "ElectronicCats";
  hid->manufacturer()->setValue(name);

  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00, 0x02);

  hid->reportMap((uint8_t*)keyboardHidDescriptor, sizeof(keyboardHidDescriptor));
  hid->startServices();

  BLESecurity *pSecurity = new BLESecurity();
  //  pSecurity->setKeySize();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->start();
  hid->setBatteryLevel(100);
  yield();
  delay(portMAX_DELAY);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("WEBSOCKET: [%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("WEBSOCKET: [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        /* A simple system to block IP Addresses that fail connecting 5 times for a 5 minute period */
        int IPAddressPosition = ip[3]-1;
        if ((IPAddressPosition>=0) && (IPAddressPosition<253)) {
          int FailedAttempts = IPAddressFailures[IPAddressPosition].FailedAttempts;
          if (FailedAttempts<5) {
            if (strcmp((const char *)payload, "/jsonrpc") != 0) {
              Serial.println("WEBSOCKET: Client has not connected to the correct path, disconnecting...");
              Serial.printf("IP Address %d.%d.%d.%d has failed connecting\n",ip[0], ip[1], ip[2], ip[3]);
  
              IPAddressFailures[IPAddressPosition].lastFailure = time(nullptr);
              IPAddressFailures[IPAddressPosition].FailedAttempts=FailedAttempts+1;
              if ((FailedAttempts+1)==5) {
                Serial.printf("IP Address %d.%d.%d.%d has failed connecting 5 times, it will now be blocked for 300 seconds\n",ip[0], ip[1], ip[2], ip[3]);
              } 
              webSocket.disconnect(num);
            }        
          } else {
            Serial.println("Disconnecting client due to being banned");
            if ((IPAddressFailures[IPAddressPosition].lastFailure+300)<time(nullptr)) {
              Serial.println("It has now been over five minutes, disabling client block...");
              IPAddressFailures[IPAddressPosition].FailedAttempts = 0;
            }
            webSocket.disconnect(num);
          }
        }
      }
      break;
    case WStype_TEXT:
      {
        unsigned long startTime;
        startTime = millis();

        Serial.printf("WEBSOCKET: [%u] Payload: %s\n", num, payload);
        Serial.printf("Counter: %d\n",counter);
        
        counter = counter + 1;
        /*  I want to avoid filling up stack space, and this is only needed to test latency */
        if (counter==10000) {
          counter=1;
        }
        JSONVar myObject = JSON.parse((const char *)payload);

        if (strcmp(myObject["method"], "Player.GetActivePlayers") == 0) {
          webSocket.sendTXT(num, "{\"id\": 1, \"jsonrpc\": \"2.0\", \"result\": [ { \"playerid\": 1, \"type\": \"video\" } ]}");
          return;
        } else {
          webSocket.sendTXT(num, "{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":\"OK\"}");
        }

        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        if (BLEconnected == false) {
          Serial.println("Bluetooth not connected");
          return;
        }
        if (!myObject.hasOwnProperty("method")) {
          Serial.println("JSON parse cannot find method");
          return;
        }
        Serial.println("Correctly parsed JSON");
        if (strcmp(myObject["method"], "Input.ExecuteAction") == 0) {
          if (!((myObject.hasOwnProperty("params")) && (myObject["params"].hasOwnProperty("action")))) {
            Serial.println("JSON parse cannot find params or action and is required for Input.ExecuteAction");
            return;
          }
          processUSBHID(myObject["method"], myObject["params"]["action"]);
        } else {
          processUSBHID(myObject["method"], "");
        }
        Serial.printf("Function time was %d\n", (int)(millis() - startTime));
        if (strcmp(myObject["method"], "Player.Stop")==0) {
          /* As the current paying media is stopped, assuming next media state is playing */
          playing = true;
        }
        break;
      }
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
      break;
  }

}

void setup() {
  Serial.begin(115200);
  /* Populate the list of struct array with IP addresses for a /24 subnet this will need to be modified if you are on something else */
  for (int i=0;i<253;i++) {
    IPAddressFailures[i].FailedAttempts = 0;
  }
  
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("WEBSOCKET: [SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(ssid, password);

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("Wifi Connected");
  xTaskCreate(keyTaskServer, "server", 20000, NULL, 5, NULL);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
