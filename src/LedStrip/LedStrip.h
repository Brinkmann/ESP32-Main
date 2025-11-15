#ifndef LED_STRIP_H
#define LED_STRIP_H
#include "Arduino.h"
#include <FastLED.h>
#include "../SystemSettings.h"
#include "../commands/feqCommands.h"

#ifndef SYSTEM_LED_BRIGHTNESS_DEFAULT
#define SYSTEM_LED_BRIGHTNESS_DEFAULT 255
#endif 

typedef struct LedPatternAattributes{

    int patternActive;
    bool state;
    bool receivedUpdate;

} LedPatternAttributes;

typedef struct LedNodeUpdateAttributes{

    bool receivedUpdate;
    FeqCmd* cmd;

} LedNodeUpdateAttributes;

class LedStrip{

    public:
     LedStrip();
     void init(void);
    private:

};

#endif