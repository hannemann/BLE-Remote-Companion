#include <NVIDIAControl.h>

inputKeyboard_t getKeyboard(JSONMethodToCecType key) {
  inputKeyboard_t keyboard{};
  if (key.LeftCTRL) {
    keyboard.KB_KeyboardKeyboardLeftControl = 1;
  }
  keyboard.Key = key.USBHID;
  return keyboard;
}

void sendKey(JSONMethodToCecType key) {
  BLECharacteristic* bc = key.KeyboardAction == 1 ? input : inputMedia;
  Serial.printf("Sending %s code : %d\n", key.KeyboardAction == 1 ? "key" : "media", key.USBHID);
  inputKeyboard_t keyboard = getKeyboard(key);
  bc->setValue((uint8_t*)&keyboard, sizeof(keyboard));
  bc->notify();
  if (key.LongPress) {
    Serial.println("Longpress");
    for(int i = 0; i < 50; i++) {
      delay(10);
      bc->setValue((uint8_t*)&keyboard, sizeof(keyboard));
      bc->notify();
    }
  }
  bc->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
  bc->notify();
  delay(10);
}

void processRequest(JSONVar jsonBody) {
  Serial.printf("Incoming JSON Request: %s\n", (const char *)jsonBody["method"]);
  for (int i = 0; i < JSONMethodToCecLength; i++) {
    if ((strcmp(JSONMethodToCec[i].JSONMethod, (const char *)jsonBody["method"]) == 0)) {
      Serial.println("JSON valid");
      sendKey(JSONMethodToCec[i]);
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
        JSONVar jsonBody = JSON.parse((const char *)payload);

        if (strcmp(jsonBody["method"], "Player.GetActivePlayers") == 0) {
          webSocket.sendTXT(num, "{\"id\": 1, \"jsonrpc\": \"2.0\", \"result\": [ { \"playerid\": 1, \"type\": \"video\" } ]}");
          return;
        } else {
          webSocket.sendTXT(num, "{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":\"OK\"}");
        }

        if (JSON.typeof(jsonBody) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        if (BLEconnected == false) {
          Serial.println("Bluetooth not connected");
          return;
        }
        if (!jsonBody.hasOwnProperty("method")) {
          Serial.println("JSON parse cannot find method");
          return;
        }
        Serial.println("Correctly parsed JSON");
        processRequest(jsonBody);
        Serial.printf("Function time was %d\n", (int)(millis() - startTime));
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

  wifi.addAP(ssid, password);

  while (wifi.run() != WL_CONNECTED) {
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
