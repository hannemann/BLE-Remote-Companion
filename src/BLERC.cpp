#include "BLERC.h"

String BLERC::configJSON = "{}";
String BLERC::remoteMappingsJSON = "{}";
JSONVar BLERC::remoteMappings = JSONVar();
String BLERC::room = "";
String BLERC::ha_ip = "";
String BLERC::ha_token = "";
uint16_t BLERC::ha_port = 8123;
bool BLERC::ha_api_enable = false;
bool BLERC::ha_send_assigned = false;
bool BLERC::ws_br_enable = false;
bool BLERC::ws_br_send_assigned = false;
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
  readMappings();
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
        String data = BLERC::preferences.getString("config", "{}");
        JSONVar config = JSON.parse(data);
        BLERC::preferences.end();

        if (config.hasOwnProperty("room"))
        {
            room = String((const char *)config["room"]);
        }
        if (config.hasOwnProperty("ha_ip"))
        {
            ha_ip = String((const char *)config["ha_ip"]);
        }
        if (config.hasOwnProperty("ha_token"))
        {
            ha_token = String((const char *)config["ha_token"]);
            config["ha_token"] = true;
        }
        if (config.hasOwnProperty("ha_port"))
        {
            ha_port = (uint16_t)(long)config["ha_port"];
        }
        else
        {
            config["ha_port"] = ha_port;
        }
        if (config.hasOwnProperty("ha_api_enable"))
        {
            ha_api_enable = bool(config["ha_api_enable"]);
        }
        if (config.hasOwnProperty("ha_send_assigned"))
        {
            ha_send_assigned = bool(config["ha_send_assigned"]);
        }
        if (config.hasOwnProperty("ws_br_enable"))
        {
            ws_br_enable = bool(config["ws_br_enable"]);
        }
        if (config.hasOwnProperty("ws_br_send_assigned"))
        {
            ws_br_send_assigned = bool(config["ws_br_send_assigned"]);
        }

        JSONVar tmp;
        tmp["config"] = config;
        configJSON = JSON.stringify(tmp);
    }
}

void BLERC::saveConfig(JSONVar &params)
{
    BLERC::preferences.begin("blerc", false);
    JSONVar cfg = JSON.parse(BLERC::preferences.getString("config", "{}"));
    if (params.hasOwnProperty("config"))
    {
        JSONVar config = params["config"];

        if (config.hasOwnProperty("ha_token"))
        { // we dont want to delet a previously defined token since its not send again by the client
            cfg["ha_token"] = (const char *)config["ha_token"];
        }
        cfg["ha_api_enable"] = bool(config.hasOwnProperty("ha_api_enable"));
        cfg["ha_send_assigned"] = bool(config.hasOwnProperty("ha_send_assigned"));
        cfg["ws_br_enable"] = bool(config.hasOwnProperty("ws_br_enable"));
        cfg["ws_br_send_assigned"] = bool(config.hasOwnProperty("ws_br_send_assigned"));

        cfg["room"] = undefined;
        cfg["ha_port"] = undefined;
        cfg["ha_ip"] = undefined;
        if (config.hasOwnProperty("room"))
        {
            cfg["room"] = (const char *)config["room"];
        }
        if (config.hasOwnProperty("ha_ip"))
        {
            cfg["ha_ip"] = (const char *)config["ha_ip"];
        }
        if (config.hasOwnProperty("ha_port"))
        {
            cfg["ha_port"] = (uint16_t)(long)config["ha_port"];
        }
    }
    BLERC::preferences.putString("config", JSON.stringify(cfg).c_str());
    BLERC::preferences.end();
    instance().readConfig();
}

void BLERC::readMappings()
{
    BLERC::preferences.begin("remote", true);
    if (BLERC::preferences.isKey("mappings"))
    {
        BLERC::remoteMappingsJSON = BLERC::preferences.getString("mappings");
        BLERC::remoteMappings = JSON.parse(BLERC::preferences.getString("mappings"));
    }
    else
    {
        BLERC::preferences.end();
        BLERC::preferences.begin("remote", false);
        BLERC::preferences.putString("mappings", BLERC::remoteMappingsJSON);
    }
    BLERC::preferences.end();
}

bool BLERC::addRemoteMapping(JSONVar &params)
{
    BLERC::preferences.begin("remote", false);
    BLERC::remoteMappings[params["old"]] = (const char *)params["new"];
    BLERC::remoteMappingsJSON = JSON.stringify(BLERC::remoteMappings);
    BLERC::preferences.putString("mappings", BLERC::remoteMappingsJSON);
    BLERC::preferences.end();

    return true;
}

bool BLERC::removeRemoteMapping(JSONVar &params)
{
    BLERC::preferences.begin("remote", false);
    BLERC::remoteMappings[params["default"]] = undefined;
    BLERC::remoteMappingsJSON = JSON.stringify(BLERC::remoteMappings);
    BLERC::preferences.putString("mappings", BLERC::remoteMappingsJSON);
    BLERC::preferences.end();

    return true;
}

void BLERC::loop()
{
  WebService::instance().loop();
  if (!WebService::captiveMode)
  {
    IRService::instance().loop();
  }
}
