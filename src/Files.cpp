#include "Files.h"

String Files::scripts = String();
String Files::css = String();

void Files::init() {
  SPIFFS.begin(true);
  Serial.print(SPIFFS.usedBytes());
  Serial.print("/");
  Serial.print(SPIFFS.totalBytes());
  Serial.println(" SPIFFS Bytes used");
  css = readFile("/styles.css");
  scripts = readFile("/scripts.js");
  SPIFFS.end();
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