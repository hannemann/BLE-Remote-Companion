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
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_UP"), "&#58132;", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_LEFT"), "&#58130;", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_ENTER"), "OK", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_RIGHT"), "&#58131;", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKeyIndex(TYPE_KEYBOARD, "KEYCODE_DPAD_DOWN"), "&#58133;", 0},
    {TYPE_NONE, -1, "", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteColors[] {
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_RED"), "&#128997;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_GREEN"), "&#129001;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_YELLOW"), "&#129000;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_PROG_BLUE"), "&#128998;", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteMedia[] {
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_PREVIOUS"), "&#58136;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_REWIND"), "&#58134;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_FAST_FORWARD"), "&#58135;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_NEXT"), "&#58137;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_RECORD"), "&#58140;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_STOP"), "&#58139;", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKeyIndex(TYPE_CONSUMER, "KEYCODE_MEDIA_PLAY_PAUSE"), "&#58131;&#58138;", 0}
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

int16_t HIDUsageKeys::getKey(uint8_t type, const char* code) {
    switch(type) {
        case TYPE_KEYBOARD:
            return instance().hidKeyboard(code);
        case TYPE_CONSUMER:
            return instance().hidConsumer(code);
        case TYPE_APP_LAUNCHER:
            return instance().hidAppLauncher(code);
        case TYPE_APP_CONTROL:
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
                buffer[i] = layoutRemoteColors  [i];
            }
            break;
        case LAYOUT_REMOTE_MEDIA:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteMedia  [i];
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
    }
    return size;
}