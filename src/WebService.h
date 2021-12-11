#include <WiFi.h>
#include "WSEvent.h"
#include "HTTPEvent.h"

#ifndef WIFI_H
#define WIFI_H

class WebService {
    public:
        WebService();
        static WebService& instance()
        {
            static WebService instance;
            return instance;
        }
        void init();
        void run();
        void loop();
};

#endif // WIFI_H