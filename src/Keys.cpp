#include "Keys.h"

const HID_USAGE_KEY HIDUsageKeys::layoutEmpty[] {
    {TYPE_NONE, -1, "", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteMinimal[] {
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_POWER"), "Power", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MENU"), "Menu", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_ESCAPE"), "Back", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_UP"), "Up", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_HOME"), "Home", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_LEFT"), "Left", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_ENTER"), "OK", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_RIGHT"), "Right", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_PLAY_PAUSE"), "Play", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_DOWN"), "Down", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_STOP"), "Stop", 0},
    {TYPE_APP_LAUNCHER, HIDUsageKeys::getKeyIndex(TYPE_APP_LAUNCHER, "AL Calculator"), "Calc", 0},
    {TYPE_APP_LAUNCHER, HIDUsageKeys::getKeyIndex(TYPE_APP_LAUNCHER, "AL File Browser"), "Files", 0},
    {TYPE_APP_CONTROL, HIDUsageKeys::getKeyIndex(TYPE_APP_CONTROL, "AC Exit"), "Exit", 0},
    {TYPE_APP_LAUNCHER, HIDUsageKeys::getKeyIndex(TYPE_APP_LAUNCHER, "AL Select Task / Application"), "Switcher", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteNumbers[] {
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_1"), "1", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_2"), "2", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_3"), "3", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_4"), "4", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_5"), "5", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_6"), "6", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_7"), "7", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_8"), "8", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_9"), "9", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_0"), "0", 0},
    {TYPE_NONE, -1, "", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteDPad[] {
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_VOLUME_UP"), "&plus;", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_UP"), "&utrif;", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_PAGE_UP"), "&plus;", 0},

    {TYPE_NONE, -1, "Vol", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_LEFT"), "&ltrif;", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_ENTER"), "&#x1f197", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_RIGHT"), "&rtrif;", 0},
    {TYPE_NONE, -1, "CH", 0},

    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_VOLUME_DOWN"), "&minus;", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_DOWN"), "&dtrif;", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_PAGE_DOWN"), "&minus;", 0},

    {TYPE_NONE, -1, "", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_VOLUME_MUTE"), "&#x1f507;", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_LAST_CHANNEL"), "&#x1f504;", 0},
    {TYPE_NONE, -1, "", 0},
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteColors[] {
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_RED"), "&#128997;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_GREEN"), "&#129001;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_YELLOW"), "&#129000;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_BLUE"), "&#128998;", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteMedia[] {
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_PREVIOUS"), "&#x23EE;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_REWIND"), "&#x23EA;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_FAST_FORWARD"), "&#x23E9;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_NEXT"), "&#x23ED;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_RECORD"), "&#x23FA;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_STOP"), "&#x23f9;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_PLAY_PAUSE"), "&#x23EF;", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteFunctional[] {
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_ESCAPE"), "&#x21b0;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_POWER"), "PWR", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MENU"), "M", 0},
};

const HID_USAGE_KEY HIDUsageKeys::layoutKeyboardNumbers[] {
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_1"), "1", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_2"), "2", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_3"), "3", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_4"), "4", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_5"), "5", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_6"), "6", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_7"), "7", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_8"), "8", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_9"), "9", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_0"), "0", 0},
};
const HID_USAGE_KEY HIDUsageKeys::layoutKeyboardRow1[] {
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_Q"), "Q", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_W"), "W", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_E"), "E", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_R"), "R", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_T"), "T", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_Z"), "Z", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_U"), "U", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_I"), "I", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_O"), "O", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_P"), "P", 0},
};
const HID_USAGE_KEY HIDUsageKeys::layoutKeyboardRow2[] {
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_A"), "A", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_S"), "S", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_D"), "D", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_F"), "F", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_G"), "G", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_H"), "H", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_J"), "J", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_K"), "K", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_L"), "L", 0},
};
const HID_USAGE_KEY HIDUsageKeys::layoutKeyboardRow3[] {
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_Y"), "Y", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_X"), "X", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_C"), "C", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_V"), "V", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_B"), "B", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_N"), "N", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_M"), "M", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DEL"), "&#x232b", 0},
};
const HID_USAGE_KEY HIDUsageKeys::layoutKeyboardRow4[] {
    {TYPE_NONE, -1, "&nbsp;", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_SPACE"), "&nbsp", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_ENTER"), "&#x23ce", 0},
};

int16_t HIDUsageKeys::hidKeyboard(uint16_t id) {
    if (id < sizeof HIDKeyboard) {
        return HIDKeyboard[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidKeyboard(const char* code) {
    for (int16_t i=0; i < sizeof HIDKeyboard/sizeof HIDKeyboard[0]; i++) {
        if (strcmp(HIDKeyboard[i].name, code) == 0) {
            return HIDKeyboard[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidKeyboardIndex(const char* code) {
    for (int16_t i=0; i < sizeof HIDKeyboard/sizeof HIDKeyboard[0]; i++) {
        if (strcmp(HIDKeyboard[i].name, code) == 0) {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidKeyboardName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDKeyboard / sizeof HIDKeyboard[0]; i++)
    {
        return HIDKeyboard[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::hidConsumer(uint16_t id) {
    if (id < sizeof HIDConsumer) {
        return HIDConsumer[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidConsumer(const char* code) {
    for (int16_t i=0; i < sizeof HIDConsumer/sizeof HIDConsumer[0]; i++) {
        if (strcmp(HIDConsumer[i].name, code) == 0) {
            return HIDConsumer[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidConsumerIndex(const char* code) {
    for (int16_t i=0; i < sizeof HIDConsumer/sizeof HIDConsumer[0]; i++) {
        if (strcmp(HIDConsumer[i].name, code) == 0) {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidConsumerName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDKeyboard / sizeof HIDKeyboard[0]; i++)
    {
        return HIDConsumer[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::hidAppLauncher(uint16_t id) {
    if (id < sizeof HIDAppLauncher) {
        return HIDAppLauncher[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppLauncher(const char* code) {
    for (int16_t i=0; i < sizeof HIDAppLauncher/sizeof HIDAppLauncher[0]; i++) {
        if (strcmp(HIDAppLauncher[i].name, code) == 0) {
            return HIDAppLauncher[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppLauncherIndex(const char* code) {
    for (int16_t i=0; i < sizeof HIDAppLauncher/sizeof HIDAppLauncher[0]; i++) {
        if (strcmp(HIDAppLauncher[i].name, code) == 0) {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidAppLauncherName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDKeyboard / sizeof HIDKeyboard[0]; i++)
    {
        return HIDAppLauncher[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::hidAppControl(uint16_t id) {
    if (id < sizeof HIDAppControl) {
        return HIDAppControl[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppControl(const char* code) {
    for (int16_t i=0; i < sizeof HIDAppControl/sizeof HIDAppControl[0]; i++) {
        if (strcmp(HIDAppControl[i].name, code) == 0) {
            return HIDAppControl[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppControlIndex(const char* code) {
    for (int16_t i=0; i < sizeof HIDAppControl/sizeof HIDAppControl[0]; i++) {
        if (strcmp(HIDAppControl[i].name, code) == 0) {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidAppControlName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDKeyboard / sizeof HIDKeyboard[0]; i++)
    {
        return HIDAppControl[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::getKey(uint8_t type, uint16_t id) {
    switch(type) {
        case TYPE_KEYBOARD:
            return instance().hidKeyboard(id);
        case TYPE_CONSUMER:
            return instance().hidConsumer(id);
        case TYPE_APP_LAUNCHER:
            return instance().hidAppLauncher(id);
        case TYPE_APP_CONTROL:
            return instance().hidAppControl(id);
    }
    return -1;
}

int16_t HIDUsageKeys::getKey(const char *type, const char *code)
{
    if (strcmp(type, "KEYBOARD") == 0)
    {
        return instance().hidKeyboard(code);
    }
    else if (strcmp(type, "CONSUMER") == 0)
    {
        return instance().hidConsumer(code);
    }
    else if (strcmp(type, "APP_LAUNCHER") == 0)
    {
        return instance().hidAppLauncher(code);
    }
    else if (strcmp(type, "APP_CONTROL") == 0)
    {
        return instance().hidAppControl(code);
    }
    return -1;
}

int16_t HIDUsageKeys::getKeyIndex(uint8_t type, const char* code) {
    switch(type) {
        case TYPE_KEYBOARD:
            return instance().hidKeyboardIndex(code);
        case TYPE_CONSUMER:
            return instance().hidConsumerIndex(code);
        case TYPE_APP_LAUNCHER:
            return instance().hidAppLauncherIndex(code);
        case TYPE_APP_CONTROL:
            return instance().hidAppControlIndex(code);
    }
    return -1;
}

const char *HIDUsageKeys::getKeyName(uint8_t type, int16_t id)
{
    switch (type)
    {
    case TYPE_KEYBOARD:
        return instance().hidKeyboardName(id);
    case TYPE_CONSUMER:
        return instance().hidConsumerName(id);
    case TYPE_APP_LAUNCHER:
        return instance().hidAppLauncherName(id);
    case TYPE_APP_CONTROL:
        return instance().hidAppControlName(id);
    }
    return "unknown";
}

const char *HIDUsageKeys::getKeyType(uint8_t type)
{
    switch (type)
    {
    case TYPE_KEYBOARD:
        return "TYPE_KEYBOARD";
    case TYPE_CONSUMER:
        return "TYPE_CONSUMER";
    case TYPE_APP_LAUNCHER:
        return "TYPE_APP_LAUNCHER";
    case TYPE_APP_CONTROL:
        return "TYPE_APP_CONTROL";
    }
    return "unknown";
}

void HIDUsageKeys::getLayout(uint8_t id, HID_USAGE_KEY *buffer) {
    switch (id) {
        case LAYOUT_REMOTE_MINIMAL:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteMinimal[i];
            }
            break;
        case LAYOUT_REMOTE_DPAD:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteDPad[i];
            }
            break;
        case LAYOUT_REMOTE_NUMBERS:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteNumbers[i];
            }
            break;
        case LAYOUT_REMOTE_COLORS:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteColors[i];
            }
            break;
        case LAYOUT_REMOTE_MEDIA:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteMedia[i];
            }
            break;
        case LAYOUT_REMOTE_FUNCTIONAL:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteFunctional[i];
            }
            break;
        case LAYOUT_KEYBOARD_NUMBERS:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutKeyboardNumbers[i];
            }
            break;
        case LAYOUT_KEYBOARD_ROW1:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutKeyboardRow1[i];
            }
            break;
        case LAYOUT_KEYBOARD_ROW2:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutKeyboardRow2[i];
            }
            break;
        case LAYOUT_KEYBOARD_ROW3:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutKeyboardRow3[i];
            }
            break;
        case LAYOUT_KEYBOARD_ROW4:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutKeyboardRow4[i];
            }
            break;
    }
}

const uint8_t HIDUsageKeys::getLayoutSize(uint8_t id) {
    uint8_t size = 0;
    switch (id) {
        case LAYOUT_REMOTE_MINIMAL:
            for (uint8_t i = 0; i < sizeof layoutRemoteMinimal / sizeof layoutRemoteMinimal[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_REMOTE_DPAD:
            for (uint8_t i = 0; i < sizeof layoutRemoteDPad / sizeof layoutRemoteDPad[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_REMOTE_NUMBERS:
            for (uint8_t i = 0; i < sizeof layoutRemoteNumbers / sizeof layoutRemoteNumbers[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_REMOTE_COLORS:
            for (uint8_t i = 0; i < sizeof layoutRemoteColors / sizeof layoutRemoteColors[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_REMOTE_MEDIA:
            for (uint8_t i = 0; i < sizeof layoutRemoteMedia / sizeof layoutRemoteMedia[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_REMOTE_FUNCTIONAL:
            for (uint8_t i = 0; i < sizeof layoutRemoteFunctional / sizeof layoutRemoteFunctional[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_KEYBOARD_NUMBERS:
            for (uint8_t i = 0; i < sizeof layoutKeyboardNumbers / sizeof layoutKeyboardNumbers[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_KEYBOARD_ROW1:
            for (uint8_t i = 0; i < sizeof layoutKeyboardRow1 / sizeof layoutKeyboardRow1[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_KEYBOARD_ROW2:
            for (uint8_t i = 0; i < sizeof layoutKeyboardRow2 / sizeof layoutKeyboardRow2[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_KEYBOARD_ROW3:
            for (uint8_t i = 0; i < sizeof layoutKeyboardRow3 / sizeof layoutKeyboardRow3[0]; i++) {
                size++;
            }
            break;
        case LAYOUT_KEYBOARD_ROW4:
            for (uint8_t i = 0; i < sizeof layoutKeyboardRow4 / sizeof layoutKeyboardRow4[0]; i++) {
                size++;
            }
            break;
    }
    return size;
}