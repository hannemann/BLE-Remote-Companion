#include "Bluetooth.h"

Bluetooth::Bluetooth() {};

bool Bluetooth::BLEconnected = false;
BLEHIDDevice* Bluetooth::hid;
BLECharacteristic* Bluetooth::input;
BLECharacteristic* Bluetooth::output;
BLECharacteristic* Bluetooth::inputMedia;
BLECharacteristic* Bluetooth::outputMedia;

inputKeyboard_t Bluetooth::getKeyboard(JSONMethodToCecType key) {
  inputKeyboard_t keyboard{};
  if (key.LeftCTRL) {
    keyboard.KB_KeyboardKeyboardLeftControl = 1;
  }
  keyboard.Key = key.USBHID;
  return keyboard;
}

void Bluetooth::sendKey(JSONMethodToCecType key) {
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

void Bluetooth::send(JSONVar jsonBody) {
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
