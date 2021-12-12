#include <IRrecv.h>
#include <Preferences.h>

#ifndef IR_H
#define IR_H

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
        void learn(const char* keyCode) {learning = keyCode;};
        std::string learning = "-";
    private:
        decode_results results;
        static IRrecv irrecv;
        static Preferences preferences;
        uint8_t debounce = 150;
        uint64_t lastSteady = 0;
        uint64_t lastFlicker = 0;
        uint64_t current = 0;
        unsigned long lastDebounceTime = 0;
        void press(uint64_t code);
        void release(uint64_t code);
        int16_t getKeyIndex(uint64_t code);

};

#endif // IR_H