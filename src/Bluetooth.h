#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include <Arduino_JSON.h>
#include "Keyboard.h"
#include "Keys.h"

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

class Bluetooth
{
    public:
    Bluetooth();
    void send(JSONVar jsonBody);
    static bool BLEconnected;
    static BLEHIDDevice* hid;
    static BLECharacteristic* input;
    static BLECharacteristic* output;
    static BLECharacteristic* inputMedia;
    static BLECharacteristic* outputMedia;
    private:
    inputKeyboard_t getKeyboard(JSONMethodToCecType key);
    void sendKey(JSONMethodToCecType key);
    inputKeyboard_t keyboard_report{};
};

class BLECallback : public BLEServerCallbacks {
    public:
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

extern Bluetooth bluetooth;

#endif // BLUETOOTH_H