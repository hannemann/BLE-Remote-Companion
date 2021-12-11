#include "HTTPEvent.h"
#include "NVC.h"

void HTTPEvent::init() {
    Serial.println("Init HTTP server");
    WiFiServer server(port);
}

void HTTPEvent::run() {
    server.begin();
    Serial.printf("Webserver started listening on port %d\n", port);
}

void HTTPEvent::loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Incoming HTTP Request.");
        String header;
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        client.println("<!DOCTYPE html><html><head>");
                        client.println("<meta charset=\"utf-8\">");
                        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<title>BLE-LIRC</title>");
                        client.println("<link rel=\"icon\" href=\"data:,\"></head><body>");
                        // BODY
                        client.println("<h1>BLE-LIRC</h1>");
                        // END
                        client.println("</body></html>");
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        header = "";
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}