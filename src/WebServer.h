#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include "WSEvent.h"
#include "HTTPEvent.h"

#ifndef WIFI_H
#define WIFI_H

class WebServer {
    public:
        WebServer();
        static WebServer& instance()
        {
            static WebServer instance;
            return instance;
        }
        void init();
        void run();
        void loop();
};

#endif // WIFI_H