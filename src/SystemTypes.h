#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H
#include "esp32-hal-log.h"
#include <FastLED.h>


/**
 * @note Credentials constant settings.
 * 
 */
#ifndef SSID_PLACE_HOLDER_STR
#define SSID_PLACE_HOLDER_STR "PlaceHolderSSID"
#endif 
#ifndef PWRD_PLACE_HOLDER_STR
#define PWRD_PLACE_HOLDER_STR "PlaceHolderPassword"
#endif 
#ifndef SYSTEM_CREDENTIALS_MAX_LENGTH
#define SYSTEM_CREDENTIALS_MAX_LENGTH 32U
#endif
#ifndef SYSTEM_CREDENTIALS_MIN_LENGTH
#define SYSTEM_CREDENTIALS_MIN_LENGTH 4U
#endif

typedef struct CredentialsType{

    char pwrd[SYSTEM_CREDENTIALS_MAX_LENGTH];
    uint8_t pwrdLength;
    char ssid[SYSTEM_CREDENTIALS_MAX_LENGTH];
    uint8_t ssidLength;

}CredentialsType;

#ifndef PATTERN_NAME_MAX_LENGTH
#define PATTERN_NAME_MAX_LENGTH 32
#endif

#ifndef PATTERN_PHASES_MAX_N
#define PATTERN_PHASES_MAX_N 32
#endif

#ifndef PATTERN_NODE_ACTIONS_MAX_N
#define PATTERN_NODE_ACTIONS_MAX_N 32
#endif

#ifndef PATTERNS_MAX_SUPPORTED_N
#define PATTERNS_MAX_SUPPORTED_N 32
#endif

typedef struct NodeAction{

    uint8_t index;
    CRGB colour;
    uint8_t duration;

} NodeAction;

typedef struct PatternPhase{

    NodeAction actions[PATTERN_NODE_ACTIONS_MAX_N];
    uint8_t nodeCount;
    uint8_t index;

} PatternPhase;

typedef struct SystemPattern{

    char name[PATTERN_NAME_MAX_LENGTH];
    uint8_t nameLength;
    PatternPhase phases[PATTERN_PHASES_MAX_N];
    uint8_t phasesCount;
    uint8_t duration;

} SystemPattern;

typedef struct FeqPatterns{

    SystemPattern patterns[PATTERNS_MAX_SUPPORTED_N];
    uint8_t patternsCount; 

} FeqPatterns;

#endif 