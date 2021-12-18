#ifndef IR_H
#define IR_H

#include "BLIRC.h"

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
        void learn(JSONVar params) {learning = params;};
        JSONVar learning = JSON.parse("{}");
        JSONVar getConfig();
        void clearConfig();
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
        String getConfigKeyFromIr();
        int16_t getHidUsageFromIr();
        uint8_t getTypeId();
        uint16_t getKeyId();
        String getConfigValue();
        JSONVar config;

};

#endif // IR_H