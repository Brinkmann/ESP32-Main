#include "WifiManager.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "../DeviceModel.h"

IPAddress localIP(192, 168, 1, 1);
AsyncWebServer server(80);

WiFiManager::WiFiManager(void){}

void stationConnectedCallback(WiFiEvent_t event, WiFiEventInfo_t inf){
    log_i("Wi-Fi Station connected !");
}

bool saveCredentials(CredentialsType* cred){

    return device->fileStorage->writeCredentials(SPIFFS,cred);

}

void WiFiManager::init(void){
    
    WiFi.softAPConfig(localIP, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

    if(!WiFi.softAP(DEVICE_WIFI_MANAGER_SSID, 
                    DEVICE_WIFI_MANAGER_PWRD,
                    WIFI_MGR_COMMUNICATION_CHANNEL,
                    WIFI_MGR_BROADCAST_SSID,
                    WIFI_MGR_AP_MAX_CONNECTIONS)){
        log_e("WiFi Manager was unable to start !");
        return;
    }
    
    WiFi.onEvent(stationConnectedCallback,ARDUINO_EVENT_WIFI_AP_STACONNECTED);
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/wifi_manager.html", "text/html");
    });

    server.on(
        "/setup",
        HTTP_POST,
        [](AsyncWebServerRequest * request){
        },
            NULL,
        [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
            char jsonBuffer[1024];
            size_t jsonLen = 0;
            if (jsonLen + len < sizeof(jsonBuffer) - 1) {
                memcpy(jsonBuffer + jsonLen, data, len);
                jsonLen += len;
            } else {
                log_e("JSON request larger than expected.");
                request->send(500);
                return;
            }
            if (index + len == total) {
                jsonBuffer[jsonLen] = '\0';
                DynamicJsonDocument jsonDocument(1024); 
                DeserializationError error = deserializeJson(jsonDocument, jsonBuffer);
                if (error) {
                    log_e("Failed to parse JSON");
                    request->send(400); 
                } else {
                    JsonObject jsonData = jsonDocument.as<JsonObject>();
                    const char* ssidFromJson = jsonData["ssid"];
                    const char* passwordFromJson = jsonData["password"];
                    log_i("SSID from JSON request: %s",ssidFromJson);
                    log_i("Password from JSON request: %s",passwordFromJson);
                    CredentialsType cred;
                    uint8_t ssidLength     = measureJson(jsonData["ssid"]);
                    uint8_t passwordLength = measureJson(jsonData["password"]);
                    cred.pwrdLength = passwordLength - 1;
                    for(uint8_t k = 0; k < cred.pwrdLength; k++){
                        cred.pwrd[k] = passwordFromJson[k];
                    }
                    cred.ssidLength = ssidLength - 1;
                    for(uint8_t k = 0; k < cred.ssidLength; k++){
                        cred.ssid[k] = ssidFromJson[k];
                    }
                    if(saveCredentials(&cred)){
                        request->send(200, "text/plain", "FEQ WiFi Credentials saved correctly.");
                    }
                    else{
                        log_e("Failed to save credentials");
                        request->send(400); 
                    }
                    log_i("Device Credentials saved succesfully !");
                    delay(3000);
                    ESP.restart();
                }
            }
    });
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
    log_i("Wi-Fi Mgr WEB server intialized successfully.");
}