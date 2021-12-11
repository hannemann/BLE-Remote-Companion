#include "BLIRC.h"

void setup() {
    BLIRC::instance().setup();
}

void loop() {
    BLIRC::instance().loop();
}
