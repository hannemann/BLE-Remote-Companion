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
        static void keyboard();
        static void fourOFour();
        static String header();
        static String footer();
        static String nav();
        static JSONVar numbers();
        static JSONVar dpad();
        static JSONVar media();
        static JSONVar colors();
        static JSONVar functional();
        static JSONVar keyboardRows();
        static String button(HID_USAGE_KEY key);
        static JSONVar buttonJSON(HID_USAGE_KEY key);

};

#endif // HTTPEVENT_H