#ifndef DEVICE_MODEL_H
#define DEVICE_MODEL_H
#include "storage/FileStorage.h"
#include "wifiManager/WifiManager.h"
#include "webServer/FeqWebServer.h"
#include "ledStrip/LedStrip.h"
#include "meshNetwork/MeshNetwork.h"


#ifndef DEVICE_MODE_CONTROLLER
#define DEVICE_MODE_CONTROLLER 0
#endif

#ifndef DEVICE_MODE_NODE
#define DEVICE_MODE_NODE 1
#endif

#ifndef DEVICE_MODE_OPERATION
#define DEVICE_MODE_OPERATION DEVICE_MODE_CONTROLLER
#endif

#ifndef FEQ_DEVICE_MAC_LENGTH
#define FEQ_DEVICE_MAC_LENGTH 6
#endif

typedef struct FeqDeviceModelType{

    FeqWebServer* webServer;
    FileStorage* fileStorage;
    LedStrip* ledStrip;
    WiFiManager* wifiManager;
    MeshNetwork* network;
    uint8_t mac[FEQ_DEVICE_MAC_LENGTH];
    uint8_t meshId;
    bool provisioned; 

}FeqDeviceModelType;

extern FeqDeviceModelType* device;


#endif