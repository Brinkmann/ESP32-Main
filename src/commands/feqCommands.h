#ifndef FEQ_COMMANDS_H
#define FEQ_COMMANDS_H
#include <stdint.h>
#include <FastLED.h>


typedef enum FeqCommandsIds{

    FEQ_CMD_CLEAR_STRIP              = 0x00,
    FEQ_CMD_RESTART_NODE             = 0x01,
    FEQ_CMD_PING_REQUEST             = 0x02,
    FEQ_CMD_FILL_STRIP               = 0x03,
    FEQ_CMD_PERFORM_PATTERN          = 0x04,
    FEQ_CMD_PERFORM_SELF_COMPLETE    = 0x05,


} FeqCommandsIds;

#ifndef FEQ_START_OF_CMD
#define FEQ_START_OF_CMD 0x4645U
#endif 


#ifndef FEQ_CMD_TARGET_BROADCAST
#define FEQ_CMD_TARGET_BROADCAST 0xFF
#endif

/**
 * @note: 
 *          CMD fill example:   | nonce  | Start |lenght| Cmd |  Colour  | Delay to start | Fill type | Pattern |   CRC-16  |
 *                              | 00 00  | 46 45 |  07  |  03 | 01 02 03 |      00        |    00     |   00    |   12 45   | 
 *          CMD clear example:  | nonce  | Start |lenght| Cmd | Delay to clear | clear type |  CRC-16  |  
 *                              | 00 00  | 46 45 |  03  |  00 |       00       |      00    |  12 45   |      
 *          CMD ping example:   | nonce  | Start |lenght| Cmd | req type |  CRC-16  |             
 *                              | 00 00  | 46 45 |  02  |  00 |   00     |  12 45   |
 */

typedef enum FeqCommmandsOffsets{

    FEQ_CMD_OFFSET_NONCE  = 0x00,
    FEQ_CMD_OFFSET_START  = 0x02,
    FEQ_CMD_OFFSET_LENGTH = 0x04,
    FEQ_CMD_OFFSET_CMD    = 0x05, 

} FeqCommmandsOffsets;

typedef struct BinaryPacket{

    uint8_t* data;
    uint8_t length;

} BinaryPacket;


typedef struct FeqCmdClearStrip{

    uint8_t delayToClear; 
    uint8_t clearType;

} FeqCmdClearStrip;

typedef struct FeqCmdResetNode{

    uint8_t delayToReset;
    uint8_t displayBeforeReset;

}FeqCmdResetNode;

typedef struct FeqCmdFillStrip{

    CRGB colour;
    uint8_t delayToSart;
    uint8_t fillType;
    uint8_t pattern;

} FeqCmdFillStrip;

typedef struct FeqCmdPingRequest{

    uint8_t reqType;

} FeqCmdPingRequest;

typedef union FeqCmdOptions{

    FeqCmdClearStrip clear;
    FeqCmdResetNode reset;
    FeqCmdFillStrip fill;
    FeqCmdPingRequest pingReq;

} FeqCmdOptions;

typedef struct FeqCmd{

    FeqCmdOptions context;
    FeqCommandsIds id;

} FeqCmd;

#ifndef CMD_DELAY_TO_START_FILL_DEFAULT
#define CMD_DELAY_TO_START_FILL_DEFAULT 0x00
#endif

#ifndef CMD_FILL_TYPE_DEFAULT
#define CMD_FILL_TYPE_DEFAULT 0xFF
#endif

#ifndef CMD_PATTERN_TYPE_DEFAULT
#define CMD_PATTERN_TYPE_DEFAULT 0xAA
#endif


#endif