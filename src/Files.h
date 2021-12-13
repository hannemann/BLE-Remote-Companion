#include "SPIFFS.h"

#ifndef FILES_H
#define FILES_H

class Files {
    public:
        Files() {};
        static Files& instance() {
            static Files instance;
            return instance;
        }
        void init();
        String readFile(const char* path);
        FS getFs();
    private:
        void initSpiffs();
};

#endif // FILES_H