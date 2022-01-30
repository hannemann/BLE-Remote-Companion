#ifndef LOGGER_H
#define LOGGER_H

#include "BLERC.h"

#define DEFAULT_BUFFER_SIZE 256

class Logger
{
public:
    Logger();
    static Logger &instance()
    {
        static Logger instance;
        return instance;
    }
    void printf(const char *format, ...);
    void print(const char *message);
    void println(const char *message);
    Logger *setBufferSize(uint16_t size = DEFAULT_BUFFER_SIZE);
    Logger *setClient(int8_t client);

private:
    int8_t wsClient = -1;
    uint16_t defaultBufferSize = DEFAULT_BUFFER_SIZE;
    uint16_t bufferSize = DEFAULT_BUFFER_SIZE;
    void wsSend(const char *message);
};

#endif // LOGGER_H
