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
        void endConfig();
        static String getAssignedIRDataByKey(const char *type, const char *code);
        static int getIrProtocolByKey(const char *type, const char *code);
        static uint64_t getIrCodeByKey(const char *type, const char *code);
        static bool mouseMode;
        static int8_t mouseStep;
        static int8_t maxMouseStep;

    private:
        decode_results results;
        static IRrecv irrecv;
        uint8_t debounce = 150;
        uint64_t lastSteady = 0;
        uint64_t lastFlicker = 0;
        uint64_t current = 0;
        uint16_t currentKey = 0;
        int16_t currentHid = 0;
        uint8_t currentType = 0;
        int protocol = 0;
        unsigned long lastDebounceTime = 0;
        void press();
        void release();
        void mouseMove();
        bool isDpad();
        void storeLearned();
        void deleteLearned();
        void saveConfig();
        void printConfig();
        String getConfigKeyFromIr();
        int16_t getHidUsageFromIr();
        uint8_t getTypeId();
        uint16_t getKeyId();
        String getConfigValue();
        void notifyClients(const int16_t key, const char *method);
        JSONVar config;
        unsigned long start = 0;
};

#endif // IR_H