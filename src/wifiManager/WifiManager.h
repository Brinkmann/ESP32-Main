#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#include "Arduino.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "../SystemSettings.h"
#include "../storage/FileStorage.h"


#ifndef WIFI_MGR_COMMUNICATION_CHANNEL
#define WIFI_MGR_COMMUNICATION_CHANNEL 6
#endif

#ifndef WIFI_MGR_AP_MAX_CONNECTIONS
#define WIFI_MGR_AP_MAX_CONNECTIONS 1
#endif

#ifndef WIFI_MGR_BROADCAST_SSID
#define WIFI_MGR_BROADCAST_SSID 0
#endif


class WiFiManager{

    public: 
        WiFiManager(void);
        void init(void);
    private:

};


#endif