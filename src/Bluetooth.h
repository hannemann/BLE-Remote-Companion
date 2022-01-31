#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "BLERC.h"

const uint8_t HidDescriptor[] = {

    /*** KEYBOARD REPORT ***/
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x85, 0x01, //   REPORT_ID (1)
    0x05, 0x07, //   USAGE_PAGE (Keyboard)
    0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)
    0x75, 0x01, //   REPORT_SIZE (1)
    0x95, 0x08, //   REPORT_COUNT (8)
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    0x95, 0x01, //   REPORT_COUNT (1)
    0x75, 0x08, //   REPORT_SIZE (8)
    0x25, 0x65, //   LOGICAL_MAXIMUM (101)
    0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, //   INPUT (Data,Ary,Abs)
    0xc0,       // END_COLLECTION

    /*** MEDIA REPORT ***/
    0x05, 0x0c,       // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,       // USAGE (Consumer Control)
    0xa1, 0x01,       // COLLECTION (Application)
    0x85, 0x02,       //   REPORT_ID (2)
    0x19, 0x00,       //   USAGE_MINIMUM (Unassigned)
    0x2a, 0x3c, 0x02, //   USAGE_MAXIMUM (AC Format)
    0x15, 0x00,       //   LOGICAL_MINIMUM (0)
    0x26, 0x3c, 0x02, //   LOGICAL_MAXIMUM (572)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x75, 0x10,       //   REPORT_SIZE (16)
    0x81, 0x00,       //   INPUT (Data,Var,Abs)
    0xc0,             // END_COLLECTION

    /*** MOUSE REPORT ***/
    USAGE_PAGE(1), 0x01,      // USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x02,           // USAGE (Mouse)
    COLLECTION(1), 0x01,      // COLLECTION (Application)
    USAGE(1), 0x01,           //   USAGE (Pointer)
    COLLECTION(1), 0x00,      //   COLLECTION (Physical)
    REPORT_ID(1), 0x03,       //     REPORT_ID (1)
                              // -------------- Buttons (Left, Right, Middle, Back, Forward)
    USAGE_PAGE(1), 0x09,      //     USAGE_PAGE (Button)
    USAGE_MINIMUM(1), 0x01,   //     USAGE_MINIMUM (Button 1)
    USAGE_MAXIMUM(1), 0x05,   //     USAGE_MAXIMUM (Button 5)
    LOGICAL_MINIMUM(1), 0x00, //     LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01, //     LOGICAL_MAXIMUM (1)
    REPORT_SIZE(1), 0x01,     //     REPORT_SIZE (1)
    REPORT_COUNT(1), 0x05,    //     REPORT_COUNT (5)
    HIDINPUT(1), 0x02,        //     INPUT (Data, Variable, Absolute) ;5 button bits
                              // -------------- Padding
    REPORT_SIZE(1), 0x03,     //     REPORT_SIZE (3)
    REPORT_COUNT(1), 0x01,    //     REPORT_COUNT (1)
    HIDINPUT(1), 0x03,        //     INPUT (Constant, Variable, Absolute) ;3 bit padding
                              // -------------- X/Y position, Wheel
    USAGE_PAGE(1), 0x01,      //     USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x30,           //     USAGE (X)
    USAGE(1), 0x31,           //     USAGE (Y)
    USAGE(1), 0x38,           //     USAGE (Wheel)
    LOGICAL_MINIMUM(1), 0x81, //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f, //     LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,     //     REPORT_SIZE (8)
    REPORT_COUNT(1), 0x03,    //     REPORT_COUNT (3)
    HIDINPUT(1), 0x06,        //     INPUT (Data, Variable, Relative) ;3 bytes (X,Y,Wheel)
                              // -------------- Horizontal wheel
    USAGE_PAGE(1), 0x0c,      //     USAGE PAGE (Consumer Devices)
    USAGE(2), 0x38, 0x02,     //     USAGE (AC Pan)
    LOGICAL_MINIMUM(1), 0x81, //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f, //     LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,     //     REPORT_SIZE (8)
    REPORT_COUNT(1), 0x01,    //     REPORT_COUNT (1)
    HIDINPUT(1), 0x06,        //     INPUT (Data, Var, Rel)
    END_COLLECTION(0),        //   END_COLLECTION
    END_COLLECTION(0)         // END_COLLECTION
};

class Bluetooth
{
public:
  Bluetooth();
  void keypress(JSONVar &params);
  void keydown(JSONVar &params);
  void keyup(JSONVar &params);
  static void create();
  static bool BLEconnected;
  static BLEHIDDevice *hid;
  static BLECharacteristic *input;
  static BLECharacteristic *output;
  static BLECharacteristic *inputMedia;
  static BLECharacteristic *outputMedia;
  static BLECharacteristic *inputMouse;
  static BLECharacteristic *outputMouse;
  static BLEServer *pServer;
  static BLEAdvertising *pAdvertising;
  static BLESecurity *pSecurity;
  static uint16_t connId;
  static esp_ble_gatts_cb_param_t *bdParams;
  void keydown(int16_t key, bool longpress);
  void keyup();
  void mediadown(int16_t key, bool longpress);
  void mediaup();
  void mouseDown(uint8_t b);
  void mouseUp(uint8_t b);
  void mouseClick(uint8_t b = MOUSE_LEFT);
  void mouseMove(signed char x, signed char y, signed char wheel = 0, signed char hWheel = 0);
  void disconnect();
  static const char *STATUS_CONNECTED;
  static const char *STATUS_DISCONNECTED;

private:
  uint8_t _mouseButtons = 0;
  void mouseButtons(uint8_t b);
  inputKeyboard_t keyboard_report{};
};

class BLECallback : public BLEServerCallbacks
{
public:
  void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param);
  void onDisconnect(BLEServer *pServer);
};

extern Bluetooth bluetooth;

void keyTaskServer(void*);

#endif // BLUETOOTH_H