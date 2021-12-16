#include <WiFi.h>
#include <WebServer.h>

#ifndef HTTPEVENT_H
#define HTTPEVENT_H

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
        static void fourOFour();
        static String header();
        static String footer();
        static String nav();
        static String buttons();

};

#endif // HTTPEVENT_H