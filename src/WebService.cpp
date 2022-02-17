#include "WebService.h"

WebService::WebService() {}
bool WebService::captiveMode = false;
Preferences WebService::preferences = Preferences();
DNSServer WebService::dnsServer = DNSServer();

void WebService::init()
{
#if defined(WEBSOCKETS_ETH)
    ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
#endif
    WiFi.disconnect();
    Logger::instance().println("Init Webservices...");

#if (DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP > 0)
    uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
#endif
    if (hasWifiCredentials())
    {
        Logger::instance().print("Connecting to WIFI");
#if defined(WIFI_SSID) && defined(WIFI_PSK)
        Logger::instance().print(" using hardcoded credentials");
        WiFi.begin(WIFI_SSID, WIFI_PSK);
#else
        preferences.begin("wifi", true);
        String ssid = preferences.getString("ssid");
        String psk = preferences.getString("psk");
        preferences.end();
        Logger::instance().print(" using configured credentials");
        WiFi.mode(WIFI_MODE_STA);
        WiFi.begin(ssid.c_str(), psk.c_str());
#endif
        while (WiFi.status() != WL_CONNECTED)
        {
            Logger::instance().print(".");
            delay(1000);
        }
        Logger::instance().printf("\nConnected! IP address: %s\n", WiFi.localIP().toString().c_str());
        WSEvent::instance().init();
        HAClient::instance().init();
    }
    else
    {
        captiveMode = true;
        Logger::instance().print("Configuring access point...");
        IPAddress apIP(192, 168, 4, 1);
        IPAddress netMsk(255, 255, 255, 0);
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIP, apIP, netMsk);
        WiFi.softAP("BLERC", "0987654321");
        delay(500); // Without delay IsoftAP_ssid, softAP_password've seen the IP address blank
        Logger::instance().print("AP IP address: ");
        Logger::instance().println(WiFi.softAPIP().toString().c_str());
        dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    }

#if (DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP > 0)
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);
#endif
    HTTPEvent::instance().init();
}

void WebService::run()
{
    Logger::instance().println("Webservices startup...");
    if (!captiveMode)
    {
        WSEvent::instance().run();
        HAClient::instance().run();
    }
    HTTPEvent::instance().run();
}

void WebService::loop()
{
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
            Logger::instance().printf("Attempt WiFi reconnect... %d tries left.\n", reconnectTries);
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