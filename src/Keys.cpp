#include "Keys.h"

const HID_USAGE_KEY HIDUsageKeys::layoutEmpty[] {
    {TYPE_NONE, -1, "", 0}
};

const HID_USAGE_KEY HIDUsageKeys::layoutRemoteMinimal[] {
    {TYPE_CONSUMER, HIDUsageKeys::getKey(TYPE_CONSUMER, "KEYCODE_POWER"), "Power", 0},
    {TYPE_NONE, -1, "", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKey(TYPE_CONSUMER, "KEYCODE_MENU"), "Menu", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKey(TYPE_KEYBOARD, "KEYCODE_ESCAPE"), "Back", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKey(TYPE_KEYBOARD, "KEYCODE_DPAD_UP"), "Up", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKey(TYPE_CONSUMER, "KEYCODE_HOME"), "Home", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKey(TYPE_KEYBOARD, "KEYCODE_DPAD_LEFT"), "Left", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKey(TYPE_KEYBOARD, "KEYCODE_ENTER"), "OK", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKey(TYPE_KEYBOARD, "KEYCODE_DPAD_RIGHT"), "Right", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKey(TYPE_CONSUMER, "KEYCODE_MEDIA_PLAY_PAUSE"), "Play", 0},
    {TYPE_KEYBOARD, HIDUsageKeys::getKey(TYPE_KEYBOARD, "KEYCODE_DPAD_DOWN"), "Down", 0},
    {TYPE_CONSUMER, HIDUsageKeys::getKey(TYPE_CONSUMER, "KEYCODE_MEDIA_STOP"), "Stop", 0},
    {TYPE_APP_LAUNCHER, HIDUsageKeys::getKey(TYPE_APP_LAUNCHER, "AL Calculator"), "Calc", 0},
    {TYPE_APP_LAUNCHER, HIDUsageKeys::getKey(TYPE_APP_LAUNCHER, "AL File Browser"), "Files", 0},
    {TYPE_APP_CONTROL, HIDUsageKeys::getKey(TYPE_APP_CONTROL, "AC Exit"), "Exit", 0},
    {TYPE_APP_LAUNCHER, HIDUsageKeys::getKey(TYPE_APP_LAUNCHER, "AL Select Task / Application"), "Switcher", 0}
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

void HIDUsageKeys::getLayout(uint8_t id, HID_USAGE_KEY *buffer) {
    switch (id) {
        case LAYOUT_REMOTE_MINIMAL:
            for (uint8_t i = 0; i < getLayoutSize(id); i++) {
                buffer[i] = layoutRemoteMinimal[i];
            }
            break;
    }
}

const uint8_t HIDUsageKeys::getLayoutSize(uint8_t id) {
    switch (id) {
        case LAYOUT_REMOTE_MINIMAL:
            uint8_t size = 0;
            for (uint8_t i = 0; i < sizeof layoutRemoteMinimal / sizeof layoutRemoteMinimal[0]; i++) {
                size++;
            }
            return size;
    }
    return 0;
}