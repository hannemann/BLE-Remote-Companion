#include "WSEvent.h"

WebSocketsServer WSEvent::webSocket = WebSocketsServer(webSocketPort);

IPAddressFail WSEvent::IPAddressFailures[254];

void WSEvent::init() {
    Serial.println("Init Websocket server...");
    initIPAddressFailures();
    webSocket.onEvent(webSocketEvent);
}

void WSEvent::run() {
    webSocket.begin();
    Serial.printf("Websocket started listening on port %d\n", WS_PORT);
}

void WSEvent::loop() {
    webSocket.loop();
}

/**
 * Populate the list of struct array with IP addresses
 * for a /24 subnet this will need to be modified if you are on something else
 */
void WSEvent::initIPAddressFailures() {
    for (int i=0; i<253; i++) {
        IPAddressFailures[i].FailedAttempts = 0;
    }
}

void WSEvent::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("WEBSOCKET: [%u] Disconnected!\n", num);
      // Serial.printf("Connected Clients: %d\n", webSocket.connectedClients());
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("WEBSOCKET: [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        /* A simple system to block IP Addresses that fail connecting 5 times for a 5 minute period */
        int IPAddressPosition = ip[3]-1;
        if ((IPAddressPosition>=0) && (IPAddressPosition<253)) {
          int FailedAttempts = IPAddressFailures[IPAddressPosition].FailedAttempts;
          if (FailedAttempts<5) {
            if (strcmp((const char *)payload, "/jsonrpc") != 0) {
              Serial.println("WEBSOCKET: Client has not connected to the correct path, disconnecting...");
              Serial.printf("IP Address %d.%d.%d.%d has failed connecting\n",ip[0], ip[1], ip[2], ip[3]);
  
              IPAddressFailures[IPAddressPosition].lastFailure = time(nullptr);
              IPAddressFailures[IPAddressPosition].FailedAttempts=FailedAttempts+1;
              if ((FailedAttempts+1)==5) {
                Serial.printf("IP Address %d.%d.%d.%d has failed connecting 5 times, it will now be blocked for 300 seconds\n",ip[0], ip[1], ip[2], ip[3]);
              } 
              webSocket.disconnect(num);
            }        
          } else {
            Serial.println("Disconnecting client due to being banned");
            if ((IPAddressFailures[IPAddressPosition].lastFailure+300)<time(nullptr)) {
              Serial.println("It has now been over five minutes, disabling client block...");
              IPAddressFailures[IPAddressPosition].FailedAttempts = 0;
            }
            webSocket.disconnect(num);
          }
        }
      }
      break;
    case WStype_TEXT:
      {
        unsigned long startTime;
        startTime = millis();

        Serial.printf("WEBSOCKET: [%u] Payload: %s\n", num, payload);
        JSONVar jsonBody = JSON.parse((const char *)payload);

        if (JSON.typeof(jsonBody) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        if (!jsonBody.hasOwnProperty("method")) {
          Serial.println("JSON parse cannot find method");
          return;
        }
        if (strcmp(jsonBody["method"], "clear") == 0) {
          IRService::instance().clearConfig();
          return;
        }
        if (strcmp(jsonBody["method"], "buttons") == 0) {
          JSONVar btns;
          JSONVar result;
          const char* request = jsonBody["type"];
          if (strcmp(request, "numbers") == 0) {
              btns = HTTPEvent::numbers();
          }
          if (strcmp(request, "functional") == 0) {
              btns = HTTPEvent::functional();
          }
          if (strcmp(request, "dpad") == 0) {
              btns = HTTPEvent::dpad();
          }
          if (strcmp(request, "media") == 0) {
              btns = HTTPEvent::media();
          }
          if (strcmp(request, "colors") == 0) {
              btns = HTTPEvent::colors();
          }
          if (strcmp(request, "keyboard") == 0) {
              HTTPEvent::keyboardRows();
              return;
          }
          result["buttons"] = btns;
          webSocket.sendTXT(num, JSON.stringify(result).c_str());
          return;
        }
        if (!jsonBody.hasOwnProperty("params") || !jsonBody["params"].hasOwnProperty("type") || !(jsonBody["params"].hasOwnProperty("key") || jsonBody["params"].hasOwnProperty("code")))
        {
          Serial.println("JSON parse cannot find key");
          return;
        }
        // if (Bluetooth::BLEconnected == false && strcmp(jsonBody["method"], "learn") != 0) {
        //   Serial.println("Bluetooth not connected");
        //   return;
        // }
        Serial.println("Correctly parsed JSON");
        if (strcmp(jsonBody["method"], "press") == 0) {
          if (jsonBody["params"].hasOwnProperty("code"))
          {
            bluetooth.pressByCode(jsonBody);
          }
          else
          {
            bluetooth.press(jsonBody);
          }
        }
        if (strcmp(jsonBody["method"], "keydown") == 0) {
          bluetooth.down(jsonBody);
        }
        if (strcmp(jsonBody["method"], "keyup") == 0) {
          bluetooth.up(jsonBody);
        }
        if (strcmp(jsonBody["method"], "learn") == 0) {
          IRService::instance().learn(jsonBody["params"]);
        }
        Serial.printf("Function time was %d\n", (int)(millis() - startTime));
        webSocket.sendTXT(num, "{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":\"OK\"}");
        break;
      }
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
      break;
  }

}
