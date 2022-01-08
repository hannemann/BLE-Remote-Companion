#include "HTTPEvent.h"
#include "page/index.h"
#include "page/captive.h"
#include "page/reboot.h"

void HTTPEvent::init() {
    Serial.println("Init HTTP server...");
    WebServer server(HTTP_PORT);
    Serial.println("HTTP Server initialized.");
}

void HTTPEvent::run()
{
    if (!WebService::captiveMode)
    {
        Serial.println("Init routes...");
        server.on("/", HTTP_GET, home);
        server.on("/ota", HTTP_POST, ota, update);
    }
    else
    {
        Serial.println("Init captive routes...");
        server.on("/", captivePortal);
        server.on("/generate_204", captivePortal); //Android captive portal. Maybe not needed. Might be handled by notFound handler.
        server.on("/fwlink", captivePortal);       //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
        server.on("/wifi-setup", HTTP_POST, captiveSave);
    }
    server.onNotFound(fourOFour);
    Serial.println("HTTP Routes added.");
    server.begin();
    Serial.printf("Webserver started listening on port %d\n", port);
}

void HTTPEvent::loop()
{
    server.handleClient();
}

void HTTPEvent::home() {
    Serial.printf("GET /\n");
    instance().server.sendHeader("Content-Encoding", "gzip");
    instance().server.send_P(200, "text/html", indexHTML, indexHTML_L);
    Serial.println(ESP.getFreeHeap());
}

void HTTPEvent::ota()
{
    instance().server.sendHeader("Connection", "close");
    if (Update.hasError())
    {
        if (Update.canRollBack())
        {
            Update.rollBack();
        }
        instance().server.send(500, "text/plain", Update.errorString());
    }
    else
    {
        instance().server.send(200, "text/plain", "OK");
        delay(1000);
        ESP.restart();
    }
}

void HTTPEvent::update()
{
    HTTPUpload &upload = instance().server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        { //start with max available size
            Update.printError(Serial);
        }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
            Update.printError(Serial);
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (Update.end(true))
        { //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        }
        else
        {
            Update.printError(Serial);
        }
    }
}

void HTTPEvent::fourOFour() {
  instance().server.send(404, "text/plain", "404 Not found");
}

void HTTPEvent::captivePortal()
{
    Serial.printf("GET / CaptivePortal\n");
    instance().server.send_P(200, "text/html", captiveHTML);
}

void HTTPEvent::captiveSave()
{
    String ssid;
    String psk;
    if (instance().server.hasArg("ssid") && instance().server.hasArg("psk"))
    {
        for (uint8_t i = 0; i < instance().server.args(); i++)
        {
            if (instance().server.argName(i) == "ssid")
            {
                ssid = instance().server.arg(i);
            }
            if (instance().server.argName(i) == "psk")
            {
                psk = instance().server.arg(i);
            }
        }
        WebService::instance().saveCredentials(ssid.c_str(), psk.c_str());
    }
    // WebService::instance().saveCredentials();
    instance()
        .server.send_P(200, "text/html", rebootHTML);
    ESP.restart();
}
