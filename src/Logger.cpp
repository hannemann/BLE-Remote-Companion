#include "Logger.h"

Logger::Logger(){};

Logger *Logger::setBufferSize(uint16_t size)
{
    bufferSize = size;
    return this;
}

Logger *Logger::setClient(int8_t client)
{
    wsClient = client;
    return this;
}

void Logger::print(const char *message)
{
    Serial.print(message);
    if (wsClient > -1)
    {
        wsSend(message);
    }
}

void Logger::println(const char *message)
{
    Serial.println(message);
    if (wsClient > -1)
    {
        String msg = String(message);
        msg += "\n";
        wsSend(msg.c_str());
    }
}

void Logger::printf(const char *format, ...)
{
    char buffer[bufferSize];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, bufferSize, format, args);
    print(buffer);
    va_end(args);
}

void Logger::wsSend(const char *message)
{
    JSONVar payload;
    payload["log"] = message;
    WSEvent::instance().sendTXT(wsClient, JSON.stringify(payload).c_str());
}