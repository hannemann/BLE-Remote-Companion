
struct JSONMethodToCecType {
  char JSONMethod[30];
  char JSONAction[14];
  bool KeyboardAction;
  bool LongPress;
  uint8_t USBHID;
};

const JSONMethodToCecType JSONMethodToCec[] = {
  {"KEYCODE_ENTER",                 "",                 1, 0, 0x28},
  {"Input.SelectLongpress",         "",                 1, 1, 0x28},
  {"KEYCODE_DPAD_RIGHT",            "",                 1, 0, 0x4F},
  {"KEYCODE_DPAD_LEFT",             "",                 1, 0, 0x50},
  {"KEYCODE_DPAD_DOWN",             "",                 1, 0, 0x51},
  {"KEYCODE_DPAD_UP",               "",                 1, 0, 0x52},
  {"KEYCODE_ESCAPE",                "",                 1, 0, 0x29},
  {"KEYCODE_HOME",                  "",                 1, 0, 0x29},
  {"KEYCODE_MENU",                  "",                 1, 0, 0x65},
  {"KEYCODE_VOLUME_UP",             "",                 0, 0, 0xE9},

  {"KEYCODE_VOLUME_DOWN",           "",                 0, 0, 0xEA},
  {"KEYCODE_VOLUME_MUTE",           "",                 0, 0, 0xE2},
  {"KEYCODE_MEDIA_PLAY_PAUSE",      "",                 0, 0, 0xCD},
  {"KEYCODE_MEDIA_FAST_FORWARD",    "",                 0, 1, 0xB3},
  {"KEYCODE_MEDIA_REWIND",          "",                 0, 1, 0xB4},
  {"KEYCODE_MEDIA_NEXT",            "",                 0, 0, 0xB5},
  {"KEYCODE_MEDIA_PREVIOUS",        "",                 0, 0, 0xB6},
  {"KEYCODE_MEDIA_STOP",            "",                 0, 0, 0xB7},
  {"KEYCODE_POWER",                 "",                 0, 0, 0x34},
  {"KEYCODE_MENU",                  "",                 0, 0, 0x40},

  {"Input.ExecuteAction",           "menulongpress",    0, 1, 0x40}
};

int JSONMethodToCecLength = 21;