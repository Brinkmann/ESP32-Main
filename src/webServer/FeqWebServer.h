#ifndef FEQ_WEB_SERVER_H
#define FEQ_WEB_SERVER_H
#include <Arduino.h>
#include "../SystemTypes.h"

#ifndef FEQ_SERVER_WIFI_COMMUNICATION_CHANNEL
#define FEQ_SERVER_WIFI_COMMUNICATION_CHANNEL 6
#endif

#ifndef FEQ_SERVER_AP_MAX_CONNECTIONS
#define FEQ_SERVER_AP_MAX_CONNECTIONS 1
#endif

#ifndef FEQ_SERVER_BROADCAST_SSID
#define FEQ_SERVER_BROADCAST_SSID 0
#endif

#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER

class FeqWebServer{

    public:
        FeqWebServer();
        void init(CredentialsType* cred);
    private:

};

#endif

#endif