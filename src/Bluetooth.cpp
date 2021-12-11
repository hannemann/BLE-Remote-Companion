#include "Bluetooth.h"

Bluetooth::Bluetooth() {};

bool Bluetooth::BLEconnected = false;
BLEHIDDevice* Bluetooth::hid;
BLECharacteristic* Bluetooth::input;
BLECharacteristic* Bluetooth::output;
BLECharacteristic* Bluetooth::inputMedia;
BLECharacteristic* Bluetooth::outputMedia;

void Bluetooth::sendKey(JSONMethodToCecType key) {
  Serial.print("Sending key code :");
  Serial.println(key.USBHID);
  Serial.printf("Sending media code: %d\n", key.USBHID);
  inputKeyboard_t keyboard{};
  if (key.LeftCTRL) {
    keyboard.KB_KeyboardKeyboardLeftControl = 1;
  }
  keyboard.Key = key.USBHID;
  input->setValue((uint8_t*)&keyboard, sizeof(keyboard));
  input->notify();
  if (key.LongPress) {
    Serial.println("Longpress");
    for(int i = 0; i < 50; i++) {
      delay(10);
      input->setValue((uint8_t*)&keyboard, sizeof(keyboard));
      input->notify();
    }
  }
  input->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
  input->notify();
  delay(10);
}

void Bluetooth::sendMedia(JSONMethodToCecType key) {
    Serial.printf("Sending media code: %d\n", key.USBHID);    
    uint8_t value[2];
    value[0] = key.USBHID;
    value[1] = key.USBHID >> 8;
    inputMedia->setValue(value, 2);
    inputMedia->notify();
    if (key.LongPress) {
        Serial.println("Longpress");
            for(int i = 0; i < 50; i++) {
            delay(10);
            inputMedia->setValue(value, 2);
            inputMedia->notify();
        }
    }
    inputMedia->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
    inputMedia->notify();
    delay(10);
}

void Bluetooth::send(JSONVar jsonBody) {
  Serial.printf("Incoming JSON Request: %s\n", (const char *)jsonBody["method"]);
  for (int i = 0; i < JSONMethodToCecLength; i++) {
    if ((strcmp(JSONMethodToCec[i].JSONMethod, (const char *)jsonBody["method"]) == 0)) {
      Serial.println("JSON valid");
      JSONMethodToCecType key = JSONMethodToCec[i];
      key.KeyboardAction == 1 ? sendKey(key) : sendMedia(key);
      return;
    }
  }
  Serial.println("JSON invalid");
}


void BLECallback::onConnect(BLEServer* pServer) {
    Bluetooth::BLEconnected = true;
    Serial.println("Bluetooth Connected");
    BLE2902* desc = (BLE2902*)Bluetooth::input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);

    BLE2902* descv = (BLE2902*)Bluetooth::inputMedia->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    descv->setNotifications(true);
}

void BLECallback::onDisconnect(BLEServer* pServer) {
    Bluetooth::BLEconnected = false;
    Serial.println("BLE Disonnected");
    BLE2902* desc = (BLE2902*)Bluetooth::input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);

    BLE2902* descv = (BLE2902*)Bluetooth::inputMedia->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    descv->setNotifications(false);
}

Bluetooth bluetooth = Bluetooth();
