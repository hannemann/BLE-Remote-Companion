
struct JSONMethodToCecType {
  char JSONMethod[30];
  char label[16];
  bool KeyboardAction; // HID Keyboard and Keypad Page (0x07) otherwise HID Consumer Page (0x0c)
  bool LongPress;
  bool LeftCTRL;
  uint16_t USBHID;
};

  // Keyname, Keyboard Page, Longpress, Left CTRL, Keycode
const JSONMethodToCecType JSONMethodToCec[] = {
  {"KEYCODE_ENTER",                 "Enter", 1, 0, 0, 0x28},
  {"KEYCODE_ENTER_LONGPRESS",       "", 1, 1, 0, 0x28},
  {"KEYCODE_DPAD_RIGHT",            "Right", 1, 0, 0, 0x4F},
  {"KEYCODE_DPAD_LEFT",             "Left", 1, 0, 0, 0x50},
  {"KEYCODE_DPAD_DOWN",             "Down", 1, 0, 0, 0x51},
  {"KEYCODE_DPAD_UP",               "Up", 1, 0, 0, 0x52},
  {"KEYCODE_ESCAPE",                "Escape", 1, 0, 0, 0x29},
  {"KEYCODE_HOME",                  "Home", 0, 0, 0, 0x0223},
  {"KEYCODE_VOLUME_UP",             "", 0, 0, 0, 0xE9},
  {"KEYCODE_VOLUME_DOWN",           "", 0, 0, 0, 0xEA},

  {"KEYCODE_VOLUME_MUTE",           "", 0, 0, 0, 0xE2},
  {"KEYCODE_MEDIA_PLAY_PAUSE",      "Play", 0, 0, 0, 0xCD},
  {"KEYCODE_MEDIA_FAST_FORWARD",    "", 0, 1, 0, 0xB3},
  {"KEYCODE_MEDIA_REWIND",          "", 0, 1, 0, 0xB4},
  {"KEYCODE_MEDIA_NEXT",            "", 0, 0, 0, 0xB5},
  {"KEYCODE_MEDIA_PREVIOUS",        "", 0, 0, 0, 0xB6},
  {"KEYCODE_MEDIA_STOP",            "Stop", 0, 0, 0, 0xB7},
  {"KEYCODE_POWER",                 "Power", 0, 0, 0, 0x34},
  {"KEYCODE_MENU",                  "Menu", 0, 0, 0, 0x40},
  {"KEYCODE_MENU_LONGPRESS",        "", 0, 1, 0, 0x40},

  {"KEYCODE_SEARCH",                "", 0, 0, 0, 0x0221},
  {"KEYCODE_SELECT_TASK",           "", 0, 0, 0, 0x01a2},
  {"KEYCODE_INFO",                  "", 0, 0, 0, 0x01bd},   // App Switcher :) Yay!
  {"KEYCODE_PAGE_UP",               "", 1, 0, 0, 0x004b},
  {"KEYCODE_PAGE_DOWN",             "", 1, 0, 0, 0x004e},
};

const int JSONMethodToCecLength = 25;