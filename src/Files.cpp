#include "Files.h"

void Files::init() {
    initSpiffs();
}

void Files::initSpiffs() {
  SPIFFS.begin(true);
  Serial.print(SPIFFS.usedBytes());
  Serial.print("/");
  Serial.print(SPIFFS.totalBytes());
  Serial.println(" SPIFFS Bytes used");
}

FS Files::getFs() {
    return SPIFFS;
}

String Files::readFile(const char* path) {
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
        return String(content);
    }
    return "";
}