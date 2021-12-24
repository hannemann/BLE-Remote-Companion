#include "WebService.h"

WebService::WebService() {}
bool WebService::captiveMode = false;
Preferences WebService::preferences = Preferences();
DNSServer WebService::dnsServer = DNSServer();

void WebService::init() {
    WiFi.disconnect();
    Serial.println("Init Webservices...");
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
        WiFi.begin(ssid.c_str(), psk.c_str());
#endif
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(1000);
        }
        Serial.printf("\nConnected! IP address: %s\n", WiFi.localIP().toString().c_str());
        WSEvent::instance().init();
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
    HTTPEvent::instance().init();
}

void WebService::run() {
    Serial.println("Webservices startup...");
    if (!captiveMode)
    {
        WSEvent::instance().run();
    }
    HTTPEvent::instance().run();
}

void WebService::loop() {
    if (!captiveMode)
    {
        WSEvent::instance().loop();
    }
    else
    {
        dnsServer.processNextRequest();
    }
    HTTPEvent::instance().loop();
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