#include "feqCmdProcessor.h"
#include "esp32-hal-log.h"

void FeqCmdProcessor::decodeFillStripCommand(FeqCmd* cmd,BinaryPacket* raw){

    FeqCmdFillStrip* fill = &cmd->context.fill;
    fill->colour.r    = raw->data[6];
    fill->colour.g    = raw->data[7];
    fill->colour.b    = raw->data[8];
    fill->delayToSart = raw->data[9];
    fill->fillType    = raw->data[10];
    fill->pattern     = raw->data[11];

}

void FeqCmdProcessor::decodeClearStripCommand(FeqCmd* cmd,BinaryPacket* raw){

    FeqCmdClearStrip* clear = &cmd->context.clear;
    clear->delayToClear     = raw->data[6];
    clear->clearType        = raw->data[7];

}


void FeqCmdProcessor::decodePingRequestCommand(FeqCmd* cmd,BinaryPacket* raw){

    FeqCmdPingRequest* pingReq = &cmd->context.pingReq;
    pingReq->reqType = raw->data[6];

}

bool FeqCmdProcessor::decodeCmd(BinaryPacket* raw, FeqCmd* cmd){

    uint16_t nonce = (uint16_t)(raw->data[0] << 8) | (raw->data[1]);
    log_d("Received nonce 0x%04X",nonce);
    uint16_t crcCalculated = getCrc16(raw->data,raw->length-2);
    applyEncrypterDecrypter(&raw->data[2],
                            raw->length-4,
                            nonce);
    uint16_t feqStartCmd = (uint16_t)(raw->data[2] << 8) | (raw->data[3]);
    log_d("Start = 0x%04X",feqStartCmd);
    if(FEQ_START_OF_CMD != feqStartCmd){
        log_e("Command does not contain valid start.");
        return false;
    }
    uint8_t cmdLength = raw->data[4];
    log_d("Command length = %d",cmdLength);
    cmd->id = (FeqCommandsIds)raw->data[5];
    switch(cmd->id){

        case FEQ_CMD_CLEAR_STRIP:{
            decodeClearStripCommand(cmd,raw);
        }
        break;
        case FEQ_CMD_RESTART_NODE:{

        }
        break;
        case FEQ_CMD_PING_REQUEST:{
            decodePingRequestCommand(cmd,raw);
        }
        break;
        case FEQ_CMD_FILL_STRIP:{
            decodeFillStripCommand(cmd,raw);
        }
        break;
        case FEQ_CMD_PERFORM_PATTERN:{

        }
        break;
        case FEQ_CMD_PERFORM_SELF_COMPLETE:{

        }
        break;
        default:
            return false;
    }
    uint8_t  crcReceivedOffset   = cmdLength +  5;
    uint16_t crcReceived = uint16_t(raw->data[crcReceivedOffset] << 8) | 
                                    raw->data[crcReceivedOffset+1];

    log_d("CRC calculated = 0x%04X , CRC received = 0x%04X",
                        crcCalculated,crcReceived);

    if(crcCalculated != crcReceived){
        log_d("Received a CRC mismatch !");
        return false;
    }

    return true;

}


void FeqCmdProcessor::encodeFillStripCommand(FeqCmd* cmd,BinaryPacket* raw){

    const FeqCmdFillStrip* fill = &cmd->context.fill;
    raw->data[raw->length++] = 0x07;
    raw->data[raw->length++] = FEQ_CMD_FILL_STRIP;
    raw->data[raw->length++] = fill->colour.r;
    raw->data[raw->length++] = fill->colour.g;
    raw->data[raw->length++] = fill->colour.b;
    raw->data[raw->length++] = fill->delayToSart;
    raw->data[raw->length++] = fill->fillType;
    raw->data[raw->length++] = fill->pattern;

}

void FeqCmdProcessor::encodeClearStripCommand(FeqCmd* cmd,BinaryPacket* raw){

    const FeqCmdClearStrip* clear = &cmd->context.clear;
    raw->data[raw->length++] = 0x03;
    raw->data[raw->length++] = FEQ_CMD_CLEAR_STRIP;
    raw->data[raw->length++] = clear->delayToClear;
    raw->data[raw->length++] = clear->clearType;

}

void FeqCmdProcessor::encodePingRequestCommand(FeqCmd* cmd,BinaryPacket* raw){

    const FeqCmdPingRequest* pingReq = &cmd->context.pingReq;
    raw->data[raw->length++] = 0x02;
    raw->data[raw->length++] = FEQ_CMD_PING_REQUEST;
    raw->data[raw->length++] = pingReq->reqType;

}

void FeqCmdProcessor::encodeCmd(FeqCmd* cmd, BinaryPacket* raw){

    uint16_t nonce = random(65536);
    raw->data[raw->length++] = (uint8_t)(nonce >> 8) & 0xFF;
    raw->data[raw->length++] = (uint8_t)(nonce & 0xFF);
    raw->data[raw->length++] = (uint8_t)(FEQ_START_OF_CMD >> 8) & 0xFF;
    raw->data[raw->length++] = (uint8_t)(FEQ_START_OF_CMD) & 0xFF;

    switch (cmd->id)
    {
    case FEQ_CMD_CLEAR_STRIP:{
        encodeClearStripCommand(cmd,raw);
    }
    break;
    case FEQ_CMD_RESTART_NODE:{

    }
    break;
    case FEQ_CMD_PING_REQUEST:{
        encodePingRequestCommand(cmd,raw);
    }
    break;
    case FEQ_CMD_FILL_STRIP:{
        encodeFillStripCommand(cmd,raw);
    }
    break;
    case FEQ_CMD_PERFORM_PATTERN:{

    }
    break;
    case FEQ_CMD_PERFORM_SELF_COMPLETE:{

    }
    break;
    default:
        break;
    }
    applyEncrypterDecrypter(&raw->data[2],raw->length,nonce);
    uint16_t crc = getCrc16(raw->data,raw->length);
    raw->data[raw->length++] = (uint8_t)(crc >> 8) & 0xFF;
    raw->data[raw->length++] = (uint8_t)(crc & 0xFF);

}

uint16_t FeqCmdProcessor::getCrc16(const uint8_t* data, uint8_t length){
    uint16_t crc = 0xFFFF; 
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i]; 
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001; 
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}


void FeqCmdProcessor::applyEncrypterDecrypter(uint8_t* data,
                                              uint8_t length,
                                              uint16_t key){
    uint8_t keyA[2];
    keyA[0] = (uint8_t)(key & 0xFF);
    keyA[1] = (uint8_t)(key >> 8) & 0xFF;
    for(uint8_t k = 0; k < length; k ++){
        data[k] ^= keyA[ k % 2];
    }

}
