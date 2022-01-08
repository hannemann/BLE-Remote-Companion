#ifndef HTTPEVENT_H
#define HTTPEVENT_H

#include "BLERC.h"

class HTTPEvent {
    public:
        HTTPEvent() {};
        static HTTPEvent& instance()
        {
            static HTTPEvent instance;
            return instance;
        }
        void init();
        void run();
        void loop();
        WebServer server;
        uint16_t port = HTTP_PORT;
        static void home();
        static void captivePortal();
        static void captiveSave();
        static void ota();
        static void update();
        static void learn();
        static void fourOFour();
};

#endif // HTTPEVENT_H