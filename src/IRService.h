#ifndef IR_H
#define IR_H

#include "BLERC.h"

class IRService {
    public:
        IRService() {};
        static IRService& instance()
        {
            static IRService instance;
            return instance;
        }
        IRService& init();
        void run();
        void loop();
        void learn(JSONVar params) { learning = params; };
        void forget(JSONVar params) { forgetRemoteBtn = params; };
        JSONVar learning = JSON.parse("{}");
        JSONVar forgetRemoteBtn = JSON.parse("{}");
        JSONVar getConfig();
        void clearConfig();
        void endConfig(uint8_t client);

    private:
        decode_results results;
        static IRrecv irrecv;
        static Preferences preferences;
        uint8_t debounce = 150;
        uint64_t lastSteady = 0;
        uint64_t lastFlicker = 0;
        uint64_t current = 0;
        uint8_t protocol = 0;
        unsigned long lastDebounceTime = 0;
        void press();
        void release();
        void storeLearned();
        void deleteLearned();
        void saveConfig();
        void printConfig();
        String getConfigKeyFromIr();
        int16_t getHidUsageFromIr();
        uint8_t getTypeId();
        uint16_t getKeyId();
        String getConfigValue();
        JSONVar config;

};

#endif // IR_H