#include "Files.h"

FS Files::getFs() {
    return SPIFFS;
}

String Files::readFile(const char* path) {
    SPIFFS.begin(true);
    File file = SPIFFS.open(path);
    if (file) {
        char content[file.size()] = {'\0'};
        uint32_t i = 0;
        while (file.available()) {
            content[i] = file.read();
            i++;
        }
        content[i] = '\0';
        file.close();
        SPIFFS.end();
        return String(content);
    }
    SPIFFS.end();
    return "";
}