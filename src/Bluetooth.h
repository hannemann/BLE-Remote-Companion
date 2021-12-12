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
    void press(JSONVar jsonBody);
    void down(JSONVar jsonBody);
    void up(JSONVar jsonBody);
    static bool BLEconnected;
    static BLEHIDDevice* hid;
    static BLECharacteristic* input;
    static BLECharacteristic* output;
    static BLECharacteristic* inputMedia;
    static BLECharacteristic* outputMedia;
    private:
    int16_t getKeyIndex(JSONVar jsonBody);
    void keydown(JSONMethodToCecType key);
    void keyup(JSONMethodToCecType key);
    void mediadown(JSONMethodToCecType key);
    void mediaup(JSONMethodToCecType key);
    inputKeyboard_t keyboard_report{};
};

class BLECallback : public BLEServerCallbacks {
    public:
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

extern Bluetooth bluetooth;

#endif // BLUETOOTH_H