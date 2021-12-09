
struct JSONMethodToCecType {
  char JSONMethod[24];
  char JSONAction[14];
  bool KeyboardAction;
  bool LongPress;
  uint8_t USBHID;
};

const JSONMethodToCecType JSONMethodToCec[] = {
  {"Input.Select",                  "",                 1, 0, 0x28},
  {"Input.SelectLongpress",         "",                 1, 1, 0x28},
  {"Input.Left",                    "",                 1, 0, 0x50},
  {"Input.Right",                   "",                 1, 0, 0x4F},
  {"Input.Up",                      "",                 1, 0, 0x52},
  {"Input.Down",                    "",                 1, 0, 0x51},
  {"Input.ShowOSD",                 "",                 1, 0, 0x10},
  {"Input.Info",                    "",                 0, 0, 0},
  {"Input.Back",                    "",                 1, 0, 0x29},
  {"Input.Home",                    "",                 1, 0, 0x29},

  {"Input.SendText",                "",                 0, 0, 0},
  {"VideoLibrary.Scan",             "",                 0, 0, 0},
  {"Input.ContextMenu",             "",                 1, 0, 0x65},
  {"Input.ExecuteAction",           "volumeup",         0, 0, 0xE9},
  {"Input.ExecuteAction",           "volumedown",       0, 0, 0xEA},
  {"Input.ExecuteAction",           "mute",             0, 0, 0xE2},
  {"Input.ExecuteAction",           "appswitch",        0, 0, 0xBB},
  {"Input.ExecuteAction",           "play",             0, 0, 0xCD},
  {"Input.ExecuteAction",           "ffwd",             0, 1, 0xB3},
  {"Input.ExecuteAction",           "rew",              0, 1, 0xB4},

  {"Input.ExecuteAction",           "skipnext",         0, 0, 0xB5},
  {"Input.ExecuteAction",           "skipprevious",     0, 0, 0xB6},
  {"Input.ExecuteAction",           "stop",             0, 0, 0xB7},
  {"Input.ExecuteAction",           "sleep",            0, 0, 0x34},
  {"Input.ExecuteAction",           "menu",             0, 0, 0x40},
  {"Input.ExecuteAction",           "menulongpress",    0, 1, 0x40}
};

int JSONMethodToCecLength = 26;