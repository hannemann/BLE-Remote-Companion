#include "Bluetooth.h"

Bluetooth::Bluetooth() {};

bool Bluetooth::BLEconnected = false;
BLEHIDDevice* Bluetooth::hid;
BLECharacteristic* Bluetooth::input;
BLECharacteristic* Bluetooth::output;
BLECharacteristic* Bluetooth::inputMedia;
BLECharacteristic* Bluetooth::outputMedia;
BLECharacteristic *Bluetooth::inputMouse;
BLECharacteristic *Bluetooth::outputMouse;
BLEServer *Bluetooth::pServer;
BLEAdvertising *Bluetooth::pAdvertising;
BLESecurity *Bluetooth::pSecurity;
uint16_t Bluetooth::connId = 0;
esp_ble_gatts_cb_param_t *Bluetooth::bdParams;
Bluetooth bluetooth;
const char *Bluetooth::STATUS_CONNECTED = "connected";
const char *Bluetooth::STATUS_DISCONNECTED = "disconnected";

void Bluetooth::create()
{
  Logger::instance().println("Creating Bluetooth instance...");
  bluetooth = Bluetooth();
}

void Bluetooth::keydown(int16_t key, bool longpress) {
  Logger::instance().printf("Sending key code: %d %s\n", key, longpress ? "longpress" : "");
  inputKeyboard_t keyboard{};
  // if (key.LeftCTRL) {
  //   keyboard.KB_KeyboardKeyboardLeftControl = 1;
  // }
  keyboard.Key = key;
  input->setValue((uint8_t*)&keyboard, sizeof(keyboard));
  input->notify();
  if (longpress) {
    delay(500);
  }
}

void Bluetooth::keyup() {
    input->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
    input->notify();
    delay(10);
}

void Bluetooth::mediadown(int16_t key, bool longpress) {
  Logger::instance().printf("Sending media code: %d %s\n", key, longpress ? "longpress" : "");
  uint8_t value[2];
  value[0] = key;
  value[1] = key >> 8;
  inputMedia->setValue(value, 2);
  inputMedia->notify();
  if (longpress)
  {
    delay(500);
  }
}

void Bluetooth::mediaup() {
    inputMedia->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
    inputMedia->notify();
    delay(10);
}

void Bluetooth::keypress(JSONVar &params)
{
    keydown(params);
    keyup(params);
}

void Bluetooth::keydown(JSONVar &params)
{
    if (params.hasOwnProperty("type") && params.hasOwnProperty("code"))
    {
        const char *type = params["type"];
        const char *code = params["code"];
        int16_t key = HIDUsageKeys::getKey(type, code);
        if (key > 0)
        {
            bool longpress = params.hasOwnProperty("longpress") && bool(params["longpress"]);
            strcmp(type, "KEYBOARD") == 0 ? keydown(key, longpress) : mediadown(key, longpress);
        }
    }
}

void Bluetooth::keyup(JSONVar &params)
{
    if (params.hasOwnProperty("type"))
    {
        const char *type = params["type"];
        strcmp(type, "KEYBOARD") == 0 ? keyup() : mediaup();
    }
}

void Bluetooth::mouseClick(uint8_t b)
{
  ESP_LOGD("Click: %d\n", b);
  _mouseButtons = b;
  mouseMove(0, 0, 0, 0);
  _mouseButtons = 0;
  mouseMove(0, 0, 0, 0);
}

void Bluetooth::mouseButtons(uint8_t b)
{
  if (b != _mouseButtons)
  {
    _mouseButtons = b;
    mouseMove(0, 0, 0, 0);
  }
}

void Bluetooth::mouseDown(uint8_t b)
{
  mouseButtons(_mouseButtons | b);
}

void Bluetooth::mouseUp(uint8_t b)
{
  mouseButtons(_mouseButtons & ~b);
}

void Bluetooth::mouseMove(signed char x, signed char y, signed char wheel, signed char hWheel)
{
  if (BLEconnected)
  {
    ESP_LOGD("Bluetooth", "x: %d, y: %d, Buttons: %d\n", (uint8_t)x, (uint8_t)y, _mouseButtons);
    uint8_t m[5];
    m[0] = _mouseButtons;
    m[1] = x;
    m[2] = y;
    m[3] = wheel;
    m[4] = hWheel;
    inputMouse->setValue(m, 5);
    inputMouse->notify();
  }
}

void Bluetooth::disconnect()
{
    esp_ble_gap_disconnect(bdParams->connect.remote_bda);
    // esp_ble_remove_bond_device(bdParams->connect.remote_bda);
    esp_ble_gap_update_whitelist(false, bdParams->connect.remote_bda);
    pServer->disconnect(bdParams->connect.conn_id);
    pServer->removePeerDevice(bdParams->connect.conn_id, false);
    int dev_num = esp_ble_get_bond_device_num();

    esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
    esp_ble_get_bond_device_list(&dev_num, dev_list);
    for (int i = 0; i < dev_num; i++)
    {
        esp_ble_remove_bond_device(dev_list[i].bd_addr);
    }

    free(dev_list);
}

void BLECallback::onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
{
    Bluetooth::BLEconnected = true;
    Logger::instance().println("Bluetooth Connected");
    BLE2902* desc = (BLE2902*)Bluetooth::input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);

    desc = (BLE2902 *)Bluetooth::inputMedia->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);

    desc = (BLE2902 *)Bluetooth::inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);

    Bluetooth::connId = pServer->getConnId();
    Bluetooth::bdParams = param;
    JSONVar status;
    status["bleStatus"] = Bluetooth::STATUS_CONNECTED;
    WSEvent::instance().broadcastTXT(JSON.stringify(status).c_str());
}

void BLECallback::onDisconnect(BLEServer* pServer) {
    Bluetooth::BLEconnected = false;
    Logger::instance().println("Bluetooth Disonnected");
    BLE2902* desc = (BLE2902*)Bluetooth::input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);

    desc = (BLE2902 *)Bluetooth::inputMedia->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);

    desc = (BLE2902 *)Bluetooth::inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);

    Bluetooth::pAdvertising->start();
    Bluetooth::connId = 0;
    JSONVar status;
    status["bleStatus"] = Bluetooth::STATUS_DISCONNECTED;
    WSEvent::instance().broadcastTXT(JSON.stringify(status).c_str());
}

void keyTaskServer(void*) {
    BLEDevice::init("BLE Remote Companion");
    Bluetooth::pServer = BLEDevice::createServer();
    Bluetooth::pServer->setCallbacks(new BLECallback());

    Bluetooth::hid = new BLEHIDDevice(Bluetooth::pServer);

    Bluetooth::input = Bluetooth::hid->inputReport(0x01);   // <-- input REPORTID from report map
    Bluetooth::output = Bluetooth::hid->outputReport(0x01); // <-- output REPORTID from report map

    Bluetooth::inputMedia = Bluetooth::hid->inputReport(0x02); // <-- input REPORTID from report map

    Bluetooth::inputMouse = Bluetooth::hid->inputReport(0x03); // <-- input REPORTID from report map

    std::string name = "Example Inc. (TM)";
    Bluetooth::hid->manufacturer()->setValue(name);

    Bluetooth::hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    Bluetooth::hid->hidInfo(0x00, 0x02);

    Bluetooth::hid->reportMap((uint8_t *)HidDescriptor, sizeof(HidDescriptor));
    Bluetooth::hid->startServices();

    // @see https://github.com/espressif/esp-idf/blob/6fe853a2c73437f74c0e6e79f9b15db68b231d32/examples/bluetooth/bluedroid/ble/gatt_security_server/tutorial/Gatt_Security_Server_Example_Walkthrough.md
    // Bluetooth::pSecurity = new BLESecurity();
    // we want the client to enter a pin
    // Bluetooth::pSecurity->setStaticPIN(123456);
    // setStaticPin also enables secure connection an encryption
    // since this does not work atm (the client wants us to enter the key... ??)
    // we disable it again by setting the capability to none
    // Bluetooth::pSecurity->setCapability(ESP_IO_CAP_NONE);
    // Bluetooth::pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);

    // Just Works mode....
    Bluetooth::pSecurity = new BLESecurity();
    Bluetooth::pSecurity->setCapability(ESP_IO_CAP_NONE);
    Bluetooth::pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    Bluetooth::pAdvertising = Bluetooth::pServer->getAdvertising();
    Bluetooth::pAdvertising->setAppearance(HID_KEYBOARD);
    Bluetooth::pAdvertising->addServiceUUID(Bluetooth::hid->hidService()->getUUID());
    Bluetooth::pAdvertising->start();
    Bluetooth::hid->setBatteryLevel(100);
    Logger::instance().println("Waiting for Bluetooth connection...");
    yield();
    delay(portMAX_DELAY);
}
