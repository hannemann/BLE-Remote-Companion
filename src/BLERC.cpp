#include "BLERC.h"

String BLERC::configJSON = "{}";
String BLERC::room = "";
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
        Serial.printf("config: %s\n", configJSON.c_str());
        BLERC::preferences.end();
        JSONVar config = JSON.parse(configJSON);
        if (config.hasOwnProperty("config"))
        {
            JSONVar cfg = config["config"];
            if (cfg.hasOwnProperty("room"))
            {
                room = cfg["room"];
            }
        }
    }
}

void BLERC::loop()
{
  WebService::instance().loop();
  if (!WebService::captiveMode)
  {
    IRService::instance().loop();
  }
}
