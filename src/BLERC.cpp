#include "BLERC.h"

String BLERC::configJSON = "{}";
String BLERC::room = "";
bool BLERC::ha_api_enable = false;
bool BLERC::ha_send_assigned = false;
String BLERC::ha_ip = "";
uint16_t BLERC::ha_port = 8123;
String BLERC::ha_token = "";
Preferences BLERC::preferences = Preferences();

BLERC::BLERC(){};

void BLERC::setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.print("[SETUP] BOOT WAIT");
  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.print(".");
    Serial.flush();
    delay(1000);
  }
  Serial.println("");
  readConfig();
  WebService::instance().init();
  if (!WebService::captiveMode)
  {
    delay(1000);
    Bluetooth::create();
    xTaskCreate(keyTaskServer, "server", 3062, NULL, 5, NULL);
    IRService::instance().init().run();
  }
  WebService::instance().run();
  delay(2000);
  const char *line1 = PSTR("**** BLE REMOTE COMPANION V1 ****");
  const uint8_t length = 40;
  uint8_t pre = (length - strlen_P(line1)) / 2;
  Serial.printf("\n%*s%s\n\n", pre, "", line1);
  uint8_t tmp = strlen_P(" RAM SYSTEM  BASIC BYTES FREE");
  char buffer[tmp + 10];
  snprintf(buffer, tmp + 10, "%dK RAM SYSTEM %d BASIC BYTES FREE", (uint8_t)(floor(ESP.getHeapSize() / 1024)), ESP.getFreeHeap());
  uint8_t pre2 = (length - strlen(buffer)) / 2;
  Serial.printf("%*s%s\n\nReady.\n", pre2, "", buffer);
}

void BLERC::readConfig()
{
    if (BLERC::preferences.begin("blerc", true))
    {
        configJSON = BLERC::preferences.getString("config", "{}");
        BLERC::preferences.end();
        JSONVar config = JSON.parse(configJSON);
        Serial.println(configJSON);
        if (config.hasOwnProperty("config"))
        {
            JSONVar cfg = config["config"];
            if (cfg.hasOwnProperty("room") && cfg["room"] != "")
            {
                room = cfg["room"];
            }
            if (cfg.hasOwnProperty("ha_ip") && cfg["ha_ip"] != "")
            {
                ha_ip = cfg["ha_ip"];
            }
            if (cfg.hasOwnProperty("ha_token") && cfg["ha_token"] != "")
            {
                ha_token = cfg["ha_token"];
                cfg["ha_token"] = true;
            }
            if (cfg.hasOwnProperty("ha_port") && cfg["ha_port"] != "")
            {
                ha_port = (uint16_t) long(cfg["ha_port"]);
            }
            else
            {
                cfg["ha_port"] = ha_port;
            }
            if (cfg.hasOwnProperty("ha_api_enable") && cfg["ha_api_enable"] != "")
            {
                ha_api_enable = bool(cfg["ha_api_enable"]);
            }
            if (cfg.hasOwnProperty("ha_send_assigned") && cfg["ha_send_assigned"] != "")
            {
                ha_send_assigned = bool(cfg["ha_send_assigned"]);
            }
        }
        configJSON = JSON.stringify(config);
        Serial.printf("WSClient: %s %s:%d, %s\n", (ha_api_enable ? "on" : "off"), ha_ip.c_str(), ha_port, ha_token.c_str());
    }
}

void BLERC::saveConfig(JSONVar &configJSON)
{
    BLERC::preferences.begin("blerc", false);
    BLERC::preferences.putString("config", JSON.stringify(configJSON).c_str());
    BLERC::preferences.end();
    instance().readConfig();
}

void BLERC::loop()
{
  WebService::instance().loop();
  if (!WebService::captiveMode)
  {
    IRService::instance().loop();
  }
}
