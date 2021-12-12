#include "Bluetooth.h"

Bluetooth::Bluetooth() {};

bool Bluetooth::BLEconnected = false;
BLEHIDDevice* Bluetooth::hid;
BLECharacteristic* Bluetooth::input;
BLECharacteristic* Bluetooth::output;
BLECharacteristic* Bluetooth::inputMedia;
BLECharacteristic* Bluetooth::outputMedia;

void Bluetooth::keydown(JSONMethodToCecType key, bool longpress) {
  Serial.printf("Sending key code: %d %s\n", key.USBHID, longpress ? "longpress" : "");
  inputKeyboard_t keyboard{};
  if (key.LeftCTRL) {
    keyboard.KB_KeyboardKeyboardLeftControl = 1;
  }
  keyboard.Key = key.USBHID;
  input->setValue((uint8_t*)&keyboard, sizeof(keyboard));
  input->notify();
  if (longpress || key.LongPress) {
    for(int i = 0; i < 50; i++) {
      delay(10);
      input->notify();
    }
  }
}

void Bluetooth::keyup(JSONMethodToCecType key) {
    input->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
    input->notify();
    delay(10);
}

void Bluetooth::mediadown(JSONMethodToCecType key, bool longpress) {
    Serial.printf("Sending key code: %d %s\n", key.USBHID, longpress ? "longpress" : "");
    uint8_t value[2];
    value[0] = key.USBHID;
    value[1] = key.USBHID >> 8;
    inputMedia->setValue(value, 2);
    inputMedia->notify();
    if (longpress || key.LongPress) {
        for(int i = 0; i < 50; i++) {
            delay(10);
            inputMedia->notify();
        }
    }
}

void Bluetooth::mediaup(JSONMethodToCecType key) {
    inputMedia->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
    inputMedia->notify();
    delay(10);
}

void Bluetooth::press(JSONVar jsonBody) {
  int16_t idx = getKeyIndex(jsonBody);
  if (idx > -1) {
    JSONMethodToCecType key = JSONMethodToCec[idx];
    bool longpress = jsonBody["params"].hasOwnProperty("longpress");
    key.KeyboardAction == 1 ? keydown(key, longpress) : mediadown(key, longpress);
    key.KeyboardAction == 1 ? keyup(key) : mediaup(key);
  }
}

void Bluetooth::down(JSONVar jsonBody) {
  int16_t idx = getKeyIndex(jsonBody);
  if (idx > -1) {
    JSONMethodToCecType key = JSONMethodToCec[idx];
    key.KeyboardAction == 1 ? keydown(key, false) : mediadown(key, false);
  }
}

void Bluetooth::up(JSONVar jsonBody) {
  int16_t idx = getKeyIndex(jsonBody);
  if (idx > -1) {
    JSONMethodToCecType key = JSONMethodToCec[idx];
    key.KeyboardAction == 1 ? keyup(key) : mediaup(key);
  }
}

int16_t Bluetooth::getKeyIndex(JSONVar jsonBody) {
  Serial.printf("Key Request: %s %s\n", (const char *)jsonBody["method"], (const char *)jsonBody["params"]["key"]);
  for (int i = 0; i < JSONMethodToCecLength; i++) {
    if ((strcmp(JSONMethodToCec[i].JSONMethod, (const char *)jsonBody["params"]["key"]) == 0)) {
        return i;
    }
  }
  Serial.println("key invalid");
  return -1;
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
