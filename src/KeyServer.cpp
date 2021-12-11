#include "KeyServer.h"
#include "BLIRC.h"

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

  Bluetooth::hid->reportMap((uint8_t*)keyboardHidDescriptor, sizeof(keyboardHidDescriptor));
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