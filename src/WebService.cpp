#include "WebService.h"

WebService::WebService() {}
bool WebService::captiveMode = false;
Preferences WebService::preferences = Preferences();
DNSServer WebService::dnsServer = DNSServer();
bool WebService::eth_connected = false;

void WebService::init()
{
#ifdef LAN_NETWORK
    WiFi.onEvent(WebService::WiFiEvent);
    ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
#endif
    Logger::instance().println("Init Webservices...");

#ifndef LAN_NETWORK
#if (DISABLE_BROWNOUT_DETECTION_DURING_WIFI_STARTUP > 0)
    uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
#endif
    WiFi.disconnect();
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
        uint8_t maxWait = 61;
        while (WiFi.status() != WL_CONNECTED)
        {
            if (maxWait > 0)
            {
                Logger::instance().print(".");
                maxWait -= 1;
                delay(1000);
            }
            else
            {
                deleteCredentials();
                ESP.restart();
            }
        }
        Logger::instance().printf("\nConnected! IP address: %s\n", WiFi.localIP().toString().c_str());
#endif

#ifdef LAN_NETWORK
        while (eth_connected == false)
        {
            delay(1000);
        }
        Logger::instance().printf("\nConnected! IP address: %s\n", ETH.localIP().toString().c_str());
#endif
        WSEvent::instance().init();
        HAClient::instance().init();

#ifndef LAN_NETWORK
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
#ifndef LAN_NETWORK
        wifiHealth();
#endif
        if (WiFi.status() == WL_CONNECTED || eth_connected)
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

void WebService::WiFiEvent(WiFiEvent_t event)
{
#ifdef LAN_NETWORK
    String hostname = "blerc-eth-" + ETH.macAddress();
    hostname.replace(":", "");
    switch (event)
    {
    case SYSTEM_EVENT_ETH_START:
        Serial.println("ETH Started");
        // set eth hostname here
        ETH.setHostname(hostname.c_str());
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        Serial.println("ETH Connected");
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());
        if (ETH.fullDuplex())
        {
            Serial.print(", FULL_DUPLEX");
        }
        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        eth_connected = true;
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        eth_connected = false;
        break;
    case SYSTEM_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        eth_connected = false;
        break;
    default:
        break;
    }
#endif
}