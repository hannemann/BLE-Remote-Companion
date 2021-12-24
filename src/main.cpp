#include "BLERC.h"

void setup() {
    BLERC::instance().setup();
}

void loop() {
    BLERC::instance().loop();
}
