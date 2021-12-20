#ifndef HTTPEVENT_H
#define HTTPEVENT_H

#include "BLIRC.h"

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
    private:
        uint16_t port = HTTP_PORT;
        static void home();
        static void learn();
        static void keyboard();
        static void fourOFour();
        static String header();
        static String footer();
        static String nav();
        static String numbers();
        static String dpad();
        static String media();
        static String colors();
        static String functional();
        static String keyboardRows();
        static String button(HID_USAGE_KEY key);
        static JSONVar buttonJSON(HID_USAGE_KEY key);

};

#endif // HTTPEVENT_H