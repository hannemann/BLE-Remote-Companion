#include "Bluetooth.h"

Bluetooth::Bluetooth() {};

bool Bluetooth::BLEconnected = false;
BLEHIDDevice* Bluetooth::hid;
BLECharacteristic* Bluetooth::input;
BLECharacteristic* Bluetooth::output;
BLECharacteristic* Bluetooth::inputMedia;
BLECharacteristic* Bluetooth::outputMedia;

void Bluetooth::keydown(int16_t key, bool longpress) {
  Serial.printf("Sending key code: %d %s\n", key, longpress ? "longpress" : "");
  inputKeyboard_t keyboard{};
  // if (key.LeftCTRL) {
  //   keyboard.KB_KeyboardKeyboardLeftControl = 1;
  // }
  keyboard.Key = key;
  input->setValue((uint8_t*)&keyboard, sizeof(keyboard));
  input->notify();
  if (longpress) {
    for(int i = 0; i < 50; i++) {
      delay(10);
      input->notify();
    }
  }
}

void Bluetooth::keyup() {
    input->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
    input->notify();
    delay(10);
}

void Bluetooth::mediadown(int16_t key, bool longpress) {
    Serial.printf("Sending media code: %d %s\n", key, longpress ? "longpress" : "");
    uint8_t value[2];
    value[0] = key;
    value[1] = key >> 8;
    inputMedia->setValue(value, 2);
    inputMedia->notify();
    if (longpress) {
        for(int i = 0; i < 50; i++) {
            delay(10);
            inputMedia->notify();
        }
    }
}

void Bluetooth::mediaup() {
    inputMedia->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
    inputMedia->notify();
    delay(10);
}

void Bluetooth::press(JSONVar jsonBody) {
  int8_t typeId = atoi(jsonBody["params"]["type"]);
  int8_t idx = atoi(jsonBody["params"]["key"]);
  int16_t key = HIDUsageKeys::getKey(typeId, idx);
  if (key > 0) {
    bool longpress = jsonBody["params"].hasOwnProperty("longpress");
    typeId == TYPE_KEYBOARD ? keydown(key, longpress) : mediadown(key, longpress);
    typeId == TYPE_KEYBOARD ? keyup() : mediaup();
  }
}

void Bluetooth::down(JSONVar jsonBody) {
  int8_t typeId = atoi(jsonBody["params"]["type"]);
  int8_t idx = atoi(jsonBody["params"]["key"]);
  int16_t key = HIDUsageKeys::getKey(typeId, idx);
  if (key > 0) {
    typeId == TYPE_KEYBOARD ? keydown(key, false) : mediadown(key, false);
  }
}

void Bluetooth::up(JSONVar jsonBody) {
  int8_t typeId = atoi(jsonBody["params"]["type"]);
  int8_t idx = atoi(jsonBody["params"]["key"]);
  int16_t key = HIDUsageKeys::getKey(typeId, idx);
  if (key > 0) {
    typeId == TYPE_KEYBOARD ? keyup() : mediaup();
  }
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

void keyTaskServer(void*) {
  BLEDevice::init("BLIRC Keyboard Emulator");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLECallback());

  Bluetooth::hid = new BLEHIDDevice(pServer);
  Bluetooth::inputMedia = Bluetooth::hid->inputReport(1); // <-- input REPORTID from report map
  Bluetooth::outputMedia = Bluetooth::hid->outputReport(1); // <-- output REPORTID from report map

  Bluetooth::input = Bluetooth::hid->inputReport(2); // <-- input REPORTID from report map
  Bluetooth::output = Bluetooth::hid->outputReport(2); // <-- output REPORTID from report map

  std::string name = "Hannemann";
  Bluetooth::hid->manufacturer()->setValue(name);

  Bluetooth::hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  Bluetooth::hid->hidInfo(0x00, 0x02);

  Bluetooth::hid->reportMap((uint8_t*)HidDescriptor, sizeof(HidDescriptor));
  Bluetooth::hid->startServices();

  BLESecurity *pSecurity = new BLESecurity();
  //  pSecurity->setKeySize();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(Bluetooth::hid->hidService()->getUUID());
  pAdvertising->start();
  Bluetooth::hid->setBatteryLevel(100);
  yield();
  delay(portMAX_DELAY);
}

Bluetooth bluetooth = Bluetooth();
