#include "WebService.h"

WebService::WebService() {}
bool WebService::captiveMode = false;
Preferences WebService::preferences = Preferences();
DNSServer WebService::dnsServer = DNSServer();

void WebService::init() {
    WiFi.disconnect();
    Serial.println("Init Webservices...");

#if (DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP > 0)
    uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
#endif
    if (hasWifiCredentials())
    {
        Serial.print("Connecting to WIFI");
#if defined(WIFI_SSID) && defined(WIFI_PSK)
        Serial.print(" using hardcoded credentials");
        WiFi.begin(WIFI_SSID, WIFI_PSK);
#else
        preferences.begin("wifi", true);
        String ssid = preferences.getString("ssid");
        String psk = preferences.getString("psk");
        preferences.end();
        Serial.print(" using configured credentials");
        WiFi.mode(WIFI_MODE_STA);
        WiFi.begin(ssid.c_str(), psk.c_str());
#endif
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(1000);
        }
        Serial.printf("\nConnected! IP address: %s\n", WiFi.localIP().toString().c_str());
        WSEvent::instance().init();
        HAClient::instance().init();
    }
    else
    {
        captiveMode = true;
        Serial.print("Configuring access point...");
        IPAddress apIP(192, 168, 4, 1);
        IPAddress netMsk(255, 255, 255, 0);
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIP, apIP, netMsk);
        WiFi.softAP("BLERC", "0987654321");
        delay(500); // Without delay IsoftAP_ssid, softAP_password've seen the IP address blank
        Serial.print("AP IP address: ");
        Serial.println(WiFi.softAPIP());
        dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    }

#if (DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP > 0)
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);
#endif
    HTTPEvent::instance().init();
}

void WebService::run() {
    Serial.println("Webservices startup...");
    if (!captiveMode)
    {
        WSEvent::instance().run();
        HAClient::instance().run();
    }
    HTTPEvent::instance().run();
}

void WebService::loop() {
    if (!captiveMode)
    {
        wifiHealth();
        if (WiFi.status() == WL_CONNECTED)
        {
            WSEvent::instance().loop();
            if (HAClient::running)
            {
                HAClient::instance().loop();
            }
        }
    }
    else
    {
        dnsServer.processNextRequest();
    }
    HTTPEvent::instance().loop();
}

void WebService::wifiHealth()
{
    if ((WiFi.status() != WL_CONNECTED))
    {
        if (reconnectTries < 0)
        {
            ESP.restart();
        }
        uint64_t now = millis();
        if ((now - lastConnectTry) >= reconnectInterval)
        {
            Serial.printf("Attempt WiFi reconnect... %d tries left.\n", reconnectTries);
            reconnectTries--;
            WiFi.disconnect();
            WiFi.reconnect();
            lastConnectTry = now;
        }
    }
    else
    {
        reconnectTries = 10;
    }
}

bool WebService::hasWifiCredentials()
{
#if defined(WIFI_SSID) && defined(WIFI_PSK)
    return true;
#else
    preferences.begin("wifi", false);
    bool result = preferences.getBool("has_wifi", false);
    preferences.putBool("has_wifi", result);
    preferences.end();
    return result;
#endif
}

void WebService::saveCredentials(const char *ssid, const char *psk)
{
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("psk", psk);
    preferences.putBool("has_wifi", true);
    preferences.end();
}

void WebService::deleteCredentials()
{
    preferences.begin("wifi", false);
    preferences.remove("ssid");
    preferences.remove("psk");
    preferences.putBool("has_wifi", false);
    preferences.end();
}