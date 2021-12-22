#ifndef FILES_H
#define FILES_H

#include "SPIFFS.h"

class Files {
    public:
        static FS getFs();
        static String readFile(const char* path);
};

#endif // FILES_H