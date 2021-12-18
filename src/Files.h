#ifndef FILES_H
#define FILES_H

#include "SPIFFS.h"

class Files {
    public:
        Files() {};
        static Files& instance() {
            static Files instance;
            return instance;
        }
        void init();
        FS getFs();
        static String scripts;
        static String css;
    private:
        String readFile(const char* path);
};

#endif // FILES_H