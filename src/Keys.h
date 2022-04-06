#ifndef KEYS_H
#define KEYS_H

#include <Arduino.h>

#define DPAD_CENTER 0x0041
#define DPAD_RIGHT 0x004f
#define DPAD_LEFT 0x0050
#define DPAD_DOWN 0x0051
#define DPAD_UP 0x0052
#define TOGGLE_MOUSE 0x0001
struct inputConsumer_t
{
  uint16_t ConsumerControl; // Value = 0 to 572
};

struct inputKeyboard_t
{
  uint8_t KB_KeyboardKeyboardLeftControl : 1;  // Usage 0x000700E0: Keyboard Left Control, Value = 0 to 1
  uint8_t KB_KeyboardKeyboardLeftShift : 1;    // Usage 0x000700E1: Keyboard Left Shift, Value = 0 to 1
  uint8_t KB_KeyboardKeyboardLeftAlt : 1;      // Usage 0x000700E2: Keyboard Left Alt, Value = 0 to 1
  uint8_t KB_KeyboardKeyboardLeftGui : 1;      // Usage 0x000700E3: Keyboard Left GUI, Value = 0 to 1
  uint8_t KB_KeyboardKeyboardRightControl : 1; // Usage 0x000700E4: Keyboard Right Control, Value = 0 to 1
  uint8_t KB_KeyboardKeyboardRightShift : 1;   // Usage 0x000700E5: Keyboard Right Shift, Value = 0 to 1
  uint8_t KB_KeyboardKeyboardRightAlt : 1;     // Usage 0x000700E6: Keyboard Right Alt, Value = 0 to 1
  uint8_t KB_KeyboardKeyboardRightGui : 1;     // Usage 0x000700E7: Keyboard Right GUI, Value = 0 to 1
  uint8_t Key;                                 // Value = 0 to 101
};

enum USBHIDType
{
  TYPE_NONE,
  TYPE_KEYBOARD,
  TYPE_CONSUMER,
  TYPE_APP_LAUNCHER,
  TYPE_APP_CONTROL,
  TYPE_INTERNAL
};

typedef struct
{
  char name[45];
  uint16_t USBHID;
} HID_USAGE;

typedef struct
{
  uint8_t type;
  int16_t index;
  char label[20];
  uint8_t longpress : 1;
} HID_USAGE_KEY;

class HIDUsageKeys
{
public:
  HIDUsageKeys(){};
  static HIDUsageKeys &instance()
  {
    static HIDUsageKeys instance;
    return instance;
  }
  int16_t hidKeyboard(uint16_t id);
  int16_t hidKeyboard(const char *code);
  int16_t hidKeyboardIndex(const char *code);
  const char *hidKeyboardName(uint16_t id);
  int16_t hidAppLauncher(uint16_t id);
  int16_t hidAppLauncher(const char *code);
  int16_t hidAppLauncherIndex(const char *code);
  const char *hidAppLauncherName(uint16_t id);
  int16_t hidConsumer(uint16_t id);
  int16_t hidConsumer(const char *code);
  int16_t hidConsumerIndex(const char *code);
  const char *hidConsumerName(uint16_t id);
  int16_t hidAppControl(uint16_t id);
  int16_t hidAppControl(const char *code);
  int16_t hidAppControlIndex(const char *code);
  const char *hidAppControlName(uint16_t id);
  int16_t hidInternal(uint16_t id);
  int16_t hidInternal(const char *code);
  int16_t hidInternalIndex(const char *code);
  const char *hidInternalName(uint16_t id);
  static int16_t getKey(uint8_t type, uint16_t id);
  static int16_t getKey(const char *type, const char *code);
  static int16_t getKeyIndex(uint8_t type, const char *code);
  static const char *getKeyName(uint8_t type, int16_t id);
  static const char *getKeyType(uint8_t type);
  static const uint8_t getKeyTypeId(const char *type);
};

const HID_USAGE InternalUsage[1] = {
    {"TOGGLE_MOUSE", 0x0001}};

const HID_USAGE HIDKeyboard[122] = {
    {"KEYCODE_A", 0x0004},
    {"KEYCODE_B", 0x0005},
    {"KEYCODE_C", 0x0006},
    {"KEYCODE_D", 0x0007},
    {"KEYCODE_E", 0x0008},
    {"KEYCODE_F", 0x0009},
    {"KEYCODE_G", 0x000a},
    {"KEYCODE_H", 0x000b},
    {"KEYCODE_I", 0x000c},
    {"KEYCODE_J", 0x000d},
    {"KEYCODE_K", 0x000e},
    {"KEYCODE_L", 0x000f},
    {"KEYCODE_M", 0x0010},
    {"KEYCODE_N", 0x0011},
    {"KEYCODE_O", 0x0012},
    {"KEYCODE_P", 0x0013},
    {"KEYCODE_Q", 0x0014},
    {"KEYCODE_R", 0x0015},
    {"KEYCODE_S", 0x0016},
    {"KEYCODE_T", 0x0017},
    {"KEYCODE_U", 0x0018},
    {"KEYCODE_V", 0x0019},
    {"KEYCODE_W", 0x001a},
    {"KEYCODE_X", 0x001b},
    {"KEYCODE_Y", 0x001c},
    {"KEYCODE_Z", 0x001d},
    {"KEYCODE_1", 0x001e},
    {"KEYCODE_2", 0x001f},
    {"KEYCODE_3", 0x0020},
    {"KEYCODE_4", 0x0021},
    {"KEYCODE_5", 0x0022},
    {"KEYCODE_6", 0x0023},
    {"KEYCODE_7", 0x0024},
    {"KEYCODE_8", 0x0025},
    {"KEYCODE_9", 0x0026},
    {"KEYCODE_0", 0x0027},
    {"KEYCODE_ENTER", 0x0028},
    {"KEYCODE_ESCAPE", 0x0029},
    {"KEYCODE_DEL", 0x002a},
    {"KEYCODE_TAB", 0x002b},
    {"KEYCODE_SPACE", 0x002c},
    {"KEYCODE_MINUS", 0x002d},
    {"KEYCODE_EQUALS", 0x002e},
    {"KEYCODE_LEFT_BRACKET", 0x002f},
    {"KEYCODE_RIGHT_BRACKET", 0x0030},
    {"KEYCODE_BACKSLASH", 0x0031},
    {"KEYCODE_SEMICOLON", 0x0033},
    {"KEYCODE_APOSTROPHE", 0x0034},
    {"KEYCODE_GRAVE", 0x0035},
    {"KEYCODE_COMMA", 0x0036},
    {"KEYCODE_PERIOD", 0x0037},
    {"KEYCODE_SLASH", 0x0038},
    {"KEYCODE_CAPS_LOCK", 0x0039},
    {"KEYCODE_F1", 0x003a},
    {"KEYCODE_F2", 0x003b},
    {"KEYCODE_F3", 0x003c},
    {"KEYCODE_F4", 0x003d},
    {"KEYCODE_F5", 0x003e},
    {"KEYCODE_F6", 0x003f},
    {"KEYCODE_F7", 0x0040},
    {"KEYCODE_F8", 0x0041},
    {"KEYCODE_F9", 0x0042},
    {"KEYCODE_F10", 0x0043},
    {"KEYCODE_F11", 0x0044},
    {"KEYCODE_F12", 0x0045},
    {"KEYCODE_SYSRQ", 0x0046},
    {"KEYCODE_SCROLL_LOCK", 0x0047},
    {"KEYCODE_BREAK", 0x0048},
    {"KEYCODE_INSERT", 0x0049},
    {"KEYCODE_MOVE_HOME", 0x004a},
    {"KEYCODE_PAGE_UP", 0x004b},
    {"KEYCODE_FORWARD_DEL", 0x004c},
    {"KEYCODE_MOVE_END", 0x004d},
    {"KEYCODE_PAGE_DOWN", 0x004e},
    {"KEYCODE_DPAD_RIGHT", 0x004f},
    {"KEYCODE_DPAD_LEFT", 0x0050},
    {"KEYCODE_DPAD_DOWN", 0x0051},
    {"KEYCODE_DPAD_UP", 0x0052},
    {"KEYCODE_NUM_LOCK", 0x0053},
    {"KEYCODE_NUMPAD_DIVIDE", 0x0054},
    {"KEYCODE_NUMPAD_MULTIPLY", 0x0055},
    {"KEYCODE_NUMPAD_SUBTRACT", 0x0056},
    {"KEYCODE_NUMPAD_ADD", 0x0057},
    {"KEYCODE_NUMPAD_ENTER", 0x0058},
    {"KEYCODE_NUMPAD_1", 0x0059},
    {"KEYCODE_NUMPAD_2", 0x005a},
    {"KEYCODE_NUMPAD_3", 0x005b},
    {"KEYCODE_NUMPAD_4", 0x005c},
    {"KEYCODE_NUMPAD_5", 0x005d},
    {"KEYCODE_NUMPAD_6", 0x005e},
    {"KEYCODE_NUMPAD_7", 0x005f},
    {"KEYCODE_NUMPAD_8", 0x0060},
    {"KEYCODE_NUMPAD_9", 0x0061},
    {"KEYCODE_NUMPAD_0", 0x0062},
    {"KEYCODE_NUMPAD_DOT", 0x0063},
    {"KEYCODE_MENU", 0x0065},
    {"KEYCODE_POWER", 0x0066},
    {"KEYCODE_NUMPAD_EQUALS", 0x0067},
    {"KEYCODE_MEDIA_STOP", 0x0078},
    {"KEYCODE_VOLUME_MUTE", 0x007f},
    {"KEYCODE_VOLUME_UP", 0x0080},
    {"KEYCODE_VOLUME_DOWN", 0x0081},
    {"KEYCODE_NUMPAD_COMMA", 0x0085},
    {"KEYCODE_NUMPAD_LEFT_PAREN", 0x00b6},
    {"KEYCODE_NUMPAD_RIGHT_PAREN", 0x00b7},
    {"KEYCODE_CTRL_LEFT", 0x00e0},
    {"KEYCODE_SHIFT_LEFT", 0x00e1},
    {"KEYCODE_ALT_LEFT", 0x00e2},
    {"KEYCODE_META_LEFT", 0x00e3},
    {"KEYCODE_CTRL_RIGHT", 0x00e4},
    {"KEYCODE_SHIFT_RIGHT", 0x00e5},
    {"KEYCODE_ALT_RIGHT", 0x00e6},
    {"KEYCODE_META_RIGHT", 0x00e7},
    {"KEYCODE_MEDIA_PLAY_PAUSE", 0x00e8},
    {"KEYCODE_MEDIA_PREVIOUS", 0x00ea},
    {"KEYCODE_MEDIA_NEXT", 0x00eb},
    {"KEYCODE_MEDIA_EJECT", 0x00ec},
    {"KEYCODE_EXPLORER", 0x00f0},
    {"KEYCODE_BACK", 0x00f1},
    {"KEYCODE_FORWARD", 0x00f2},
    {"KEYCODE_CALCULATOR", 0x00fb},
    /* New */
    {"KEYCODE_INTL_BACKSLASH", 0x0032}};

const HID_USAGE HIDConsumer[49] = {
    {"KEYCODE_POWER", 0x0034},
    {"KEYCODE_MENU", 0x0040},
    {"KEYCODE_WINDOW", 0x0067},
    {"KEYCODE_PROG_RED", 0x0069},
    {"KEYCODE_PROG_GREEN", 0x006a},
    {"KEYCODE_PROG_BLUE", 0x006b}, // 5
    {"KEYCODE_PROG_YELLOW", 0x006c},
    {"KEYCODE_LAST_CHANNEL", 0x0083},
    {"KEYCODE_TV", 0x0089},
    {"KEYCODE_EXPLORER", 0x008a},
    {"KEYCODE_CALL", 0x008c}, // 10
    {"KEYCODE_GUIDE", 0x008d},
    {"KEYCODE_DVR", 0x009a},
    {"KEYCODE_CAPTIONS", 0x0061},
    {"KEYCODE_CHANNEL_UP", 0x009c},
    {"KEYCODE_CHANNEL_DOWN", 0x009d}, // 15
    {"KEYCODE_MEDIA_PLAY", 0x00b0},
    {"KEYCODE_BREAK", 0x00b1},
    {"KEYCODE_MEDIA_RECORD", 0x00b2},
    {"KEYCODE_MEDIA_FAST_FORWARD", 0x00b3},
    {"KEYCODE_MEDIA_REWIND", 0x00b4}, // 20
    {"KEYCODE_MEDIA_NEXT", 0x00b5},
    {"KEYCODE_MEDIA_PREVIOUS", 0x00b6},
    {"KEYCODE_MEDIA_STOP", 0x00b7},
    {"KEYCODE_MEDIA_EJECT", 0x00b8},
    {"KEYCODE_MEDIA_PLAY_PAUSE", 0x00cd}, // 25
    {"KEYCODE_VOLUME_MUTE", 0x00e2},
    {"KEYCODE_VOLUME_UP", 0x00e9},
    {"KEYCODE_VOLUME_DOWN", 0x00ea},
    {"KEYCODE_MEDIA_AUDIO_TRACK", 0x0173},
    {"KEYCODE_BOOKMARK", 0x0182}, // 30
    {"KEYCODE_MUSIC", 0x0183},
    {"KEYCODE_ENVELOPE", 0x018a},
    {"KEYCODE_CONTACTS", 0x018d},
    {"KEYCODE_CALENDAR", 0x018e},
    {"KEYCODE_CALCULATOR", 0x0192}, // 35
    {"KEYCODE_EXPLORER", 0x0196},
    {"KEYCODE_POWER", 0x019e},
    {"KEYCODE_HEADSETHOOK", 0x01b6},
    {"KEYCODE_MUSIC", 0x01b7},
    {"KEYCODE_SEARCH", 0x0221}, // 40
    {"KEYCODE_HOME", 0x0223},
    {"KEYCODE_BACK", 0x0224},
    {"KEYCODE_FORWARD", 0x0225},
    {"KEYCODE_MEDIA_STOP", 0x0226},
    {"KEYCODE_BOOKMARK", 0x022a}, // 45
    {"KEYCODE_PAGE_UP", 0x0233},
    {"KEYCODE_PAGE_DOWN", 0x0234},
    {"KEYCODE_DPAD_CENTER", 0x0041}};

const HID_USAGE HIDAppLauncher[70] = {
    "AL Launch Button Config. Tool", 0x0181,
    "AL Programmable Button Config.", 0x0182,
    "AL Consumer Control Config.", 0x0183,
    "AL Word Processor", 0x0184,
    "AL Text Editor", 0x0185,
    "AL Spreadsheet", 0x0186,
    "AL Graphics Editor", 0x0187,
    "AL Presentation App", 0x0188,
    "AL Database App", 0x0189,
    "AL Email Reader", 0x018a,
    "AL Newsreader", 0x018b,
    "AL Voicemail", 0x018c,
    "AL Contacts / Address Book", 0x018d,
    "AL Calendar / Schedule", 0x018e,
    "AL Task / Project Manager", 0x018f,
    "AL Log / Journal / Timecard", 0x0190,
    "AL Checkbook / Finance", 0x0191,
    "AL Calculator", 0x0192,
    "AL A/V Capture / Playback", 0x0193,
    "AL Local Machine Browser", 0x0194,
    "AL LAN/WAN Browser", 0x0195,
    "AL Internet Browser", 0x0196,
    "AL Remote Networking/ISP Connect", 0x0197,
    "AL Network Conference", 0x0198,
    "AL Network Chat", 0x0199,
    "AL Telephony / Dialer", 0x019a,
    "AL Logon", 0x019b,
    "AL Logoff", 0x019c,
    "AL Logon / Logoff", 0x019d,
    "AL Terminal Lock / Screensaver", 0x019e,
    "AL Control Panel", 0x019f,
    "AL Command Line Processor / Run", 0x01a0,
    "AL Process / Task Manager", 0x01a1,
    "AL Select Task / Application", 0x01a2,
    "AL Next Task / Application", 0x01a3,
    "AL Previous Task / Application", 0x01a4,
    "AL Preemptive Halt Task / App.", 0x01a5,
    "AL Integrated Help Center", 0x01a6,
    "AL Documents", 0x01a7,
    "AL Thesaurus", 0x01a8,
    "AL Dictionary", 0x01a9,
    "AL Desktop", 0x01aa,
    "AL Spell Check", 0x01ab,
    "AL Grammar Check", 0x01ac,
    "AL Wireless Status", 0x01ad,
    "AL Keyboard Layout", 0x01ae,
    "AL Virus Protection", 0x01af,
    "AL Encryption", 0x01b0,
    "AL Screen Saver", 0x01b1,
    "AL Alarms", 0x01b2,
    "AL Clock", 0x01b3,
    "AL File Browser", 0x01b4,
    "AL Power Status", 0x01b5,
    "AL Image Browser", 0x01b6,
    "AL Audio Browser", 0x01b7,
    "AL Movie Browser", 0x01b8,
    "AL Digital Rights Manager", 0x01b9,
    "AL Digital Wallet", 0x01ba,
    "AL Instant Messaging", 0x01bc,
    "AL OEM Features / Tips Browser", 0x01bd,
    "AL OEM Help", 0x01be,
    "AL Online Community", 0x01bf,
    "AL Entertainment Content Browser", 0x01c0,
    "AL Online Shopping Browser", 0x01c1,
    "AL SmartCard Information / Help", 0x01c2,
    "AL Market / Finance Browser", 0x01c3,
    "AL Customized Corp. News Browser", 0x01c4,
    "AL Online Activity Browser", 0x01c5,
    "AL Research / Search Browser", 0x01c6,
    "AL Audio Player", 0x01c7};

const HID_USAGE HIDAppControl[138] PROGMEM = {
    "AC New", 0x0201,
    "AC Open", 0x0202,
    "AC Close", 0x0203,
    "AC Exit", 0x0204,
    "AC Maximize", 0x0205,
    "AC Minimize", 0x0206,
    "AC Save", 0x0207,
    "AC Print", 0x0208,
    "AC Properties", 0x0209,
    "AC Undo", 0x021a,
    "AC Copy", 0x021b,
    "AC Cut", 0x021c,
    "AC Paste", 0x021d,
    "AC Select All", 0x021e,
    "AC Find", 0x021f,
    "AC Find and Replace", 0x0220,
    "AC Search", 0x0221,
    "AC Go To", 0x0222,
    "AC Home", 0x0223,
    "AC Back", 0x0224,
    "AC Forward", 0x0225,
    "AC Stop", 0x0226,
    "AC Refresh", 0x0227,
    "AC Previous Link", 0x0228,
    "AC Next Link", 0x0229,
    "AC Bookmarks", 0x022a,
    "AC History", 0x022b,
    "AC Subscriptions", 0x022c,
    "AC Zoom In", 0x022d,
    "AC Zoom Out", 0x022e,
    "AC Zoom", 0x022f,
    "AC Full Screen View", 0x0230,
    "AC Normal View", 0x0231,
    "AC View Toggle", 0x0232,
    "AC Scroll Up", 0x0233,
    "AC Scroll Down", 0x0234,
    "AC Pan Left", 0x0236,
    "AC Pan Right", 0x0237,
    "AC New Window", 0x0239,
    "AC Tile Horizontally", 0x023a,
    "AC Tile Vertically", 0x023b,
    "AC Format", 0x023c,
    "AC Edit", 0x023d,
    "AC Bold", 0x023e,
    "AC Italics", 0x023f,
    "AC Underline", 0x0240,
    "AC Strikethrough", 0x0241,
    "AC Subscript", 0x0242,
    "AC Superscript", 0x0243,
    "AC All Caps", 0x0244,
    "AC Rotate", 0x0245,
    "AC Resize", 0x0246,
    "AC Flip horizontal", 0x0247,
    "AC Flip Vertical", 0x0248,
    "AC Mirror Horizontal", 0x0249,
    "AC Mirror Vertical", 0x024a,
    "AC Font Select", 0x024b,
    "AC Font Color", 0x024c,
    "AC Font Size", 0x024d,
    "AC Justify Left", 0x024e,
    "AC Justify Center H", 0x024f,
    "AC Justify Right", 0x0250,
    "AC Justify Block H", 0x0251,
    "AC Justify Top", 0x0252,
    "AC Justify Center V", 0x0253,
    "AC Justify Bottom", 0x0254,
    "AC Justify Block V", 0x0255,
    "AC Indent Decrease", 0x0256,
    "AC Indent Increase", 0x0257,
    "AC Numbered List", 0x0258,
    "AC Restart Numbering", 0x0259,
    "AC Bulleted List", 0x025a,
    "AC Promote", 0x025b,
    "AC Demote", 0x025c,
    "AC Yes", 0x025d,
    "AC No", 0x025e,
    "AC Cancel", 0x025f,
    "AC Catalog", 0x0260,
    "AC Buy / Checkout", 0x0261,
    "AC Add to Cart", 0x0262,
    "AC Expand", 0x0263,
    "AC Expand All", 0x0264,
    "AC Collapse", 0x0265,
    "AC Collapse All", 0x0266,
    "AC Print Preview", 0x0267,
    "AC Paste Special", 0x0268,
    "AC Insert Mode", 0x0269,
    "AC Delete", 0x026a,
    "AC Lock", 0x026b,
    "AC Unlock", 0x026c,
    "AC Protect", 0x026d,
    "AC Unprotect", 0x026e,
    "AC Attach Comment", 0x026f,
    "AC Delete Comment", 0x0270,
    "AC View Comment", 0x0271,
    "AC Select Word", 0x0272,
    "AC Select Sentence", 0x0273,
    "AC Select Paragraph", 0x0274,
    "AC Select Column", 0x0275,
    "AC Select Row", 0x0276,
    "AC Select Table", 0x0277,
    "AC Select Object", 0x0278,
    "AC Redo / Repeat", 0x0279,
    "AC Sort", 0x027a,
    "AC Sort Ascending", 0x027b,
    "AC Sort Descending", 0x027c,
    "AC Filter", 0x027d,
    "AC Set Clock", 0x027e,
    "AC View Clock", 0x027f,
    "AC Select Time Zone", 0x0280,
    "AC Edit Time Zones", 0x0281,
    "AC Set Alarm", 0x0282,
    "AC Clear Alarm", 0x0283,
    "AC Snooze Alarm", 0x0284,
    "AC Reset Alarm", 0x0285,
    "AC Synchronize", 0x0286,
    "AC Send/Receive", 0x0287,
    "AC Send To", 0x0288,
    "AC Reply", 0x0289,
    "AC Reply All", 0x028a,
    "AC Forward Msg", 0x028b,
    "AC Send", 0x028c,
    "AC Attach File", 0x028d,
    "AC Upload", 0x028e,
    "AC Download (Save Target As)", 0x028f,
    "AC Set Borders", 0x0290,
    "AC Insert Row", 0x0291,
    "AC Insert Column", 0x0292,
    "AC Insert File", 0x0293,
    "AC Insert Picture", 0x0294,
    "AC Insert Object", 0x0295,
    "AC Insert Symbol", 0x0296,
    "AC Save and Close", 0x0297,
    "AC Rename", 0x0298,
    "AC Merge", 0x0299,
    "AC Split", 0x029a,
    "AC Distribute Horizontally", 0x029b,
    "AC Distribute Vertically", 0x029c};

#endif // KEYS_H