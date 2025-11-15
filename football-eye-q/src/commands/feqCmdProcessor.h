#ifndef FEQ_CMD_PROCESSOR_H
#define FEQ_CMD_PROCESSOR_H
#include "feqCommands.h"
/**
 * @note Command structure.
 * 
 *       1. Start of command. -> 0x4645
 *       2. Nonce.            -> Random number to be applied in encr/decr.  
 *       3. Command length.   -> 0xFF max (Includes all bytes in the packet).
 *       4. Command type.     -> 0x01 i.e FEQ Restar node. 
 *       5. Node Target.      -> (Unicast/Broadcast).
 *       6. Command contents. -> Depending on command type this will vary.
 *       7. CRC16 Calculaion. -> run over all command except the CRC16.
 *    
 */

class FeqCmdProcessor {

    public: 
        static bool decodeCmd(BinaryPacket*,FeqCmd*);
        static void encodeCmd(FeqCmd*,BinaryPacket*);
    private: 
        static uint16_t getCrc16(const uint8_t*,uint8_t);
        static void applyEncrypterDecrypter(uint8_t*,uint8_t,uint16_t);
        /**
         * @brief All system encoders. 
         * 
         */
        static void encodeFillStripCommand(FeqCmd*,BinaryPacket*);
        static void encodeClearStripCommand(FeqCmd*,BinaryPacket*);
        static void encodePingRequestCommand(FeqCmd*,BinaryPacket*);
        /**
         * @brief All system decoders.
         * 
         */
        static void decodeFillStripCommand(FeqCmd*,BinaryPacket*);
        static void decodeClearStripCommand(FeqCmd*,BinaryPacket*);
        static void decodePingRequestCommand(FeqCmd*,BinaryPacket*);
};


#endif