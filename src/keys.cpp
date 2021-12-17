#include "Keys.h"
#include "BLIRC.h"

int16_t HIDUsageKeys::hidKeyBoard(uint16_t id) {
    if (id < sizeof HIDKeyboard) {
        return HIDKeyboard[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidKeyBoard(const char* code) {
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