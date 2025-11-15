#include "FeqWebServer.h"
#include "Arduino.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "../ledStrip/LedStrip.h"
#include "../DeviceModel.h"
#include "../SystemSettings.h"
#include "../storage/FileStorage.h"

// REMOVED: SessionManager.h is no longer needed
// #include "../SessionManager.h" 

#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER

IPAddress localIPServer(192, 168, 4, 1);

AsyncWebServer serverFeq(80);

FeqWebServer::FeqWebServer(){}

extern volatile LedPatternAttributes ledPatternStrip;

static uint8_t stationsConnected = 0;

void stationConnectedCallbackFeqServer(WiFiEvent_t event, WiFiEventInfo_t inf){
    log_i("Wi-Fi Station connected No = %d",++stationsConnected);
}

bool resetCredentials(void){
    return device->fileStorage->resetCredentials(SPIFFS);
}

void FeqWebServer::init(CredentialsType* cred){

    if(cred == nullptr){
        log_e("Invalid credentials for web server.");
        return;
    }
    WiFi.softAPConfig(localIPServer, IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

    if(!WiFi.softAP((const char*)cred->ssid,
                    (const char*)cred->pwrd,
                    FEQ_SERVER_WIFI_COMMUNICATION_CHANNEL,
                    FEQ_SERVER_BROADCAST_SSID,
                    FEQ_SERVER_AP_MAX_CONNECTIONS)){
        log_e("Failed setting Web server access point.");
        return;
    }

    WiFi.onEvent(stationConnectedCallbackFeqServer,ARDUINO_EVENT_WIFI_AP_STACONNECTED);

    // REMOVED: The GET /patterns endpoint is no longer needed
    /*
    serverFeq.on("/patterns", HTTP_GET, [](AsyncWebServerRequest *request){
        log_i("Serving number of patterns : 29");
        char response[50];
        sprintf(response,"Found=29 Patterns in system.");
        request->send(200, "text/plain", response);
    });
    */

    serverFeq.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });

    serverFeq.on(
        "/patterns",
        HTTP_POST,
        [](AsyncWebServerRequest * request){},
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
                    request->send(200, "text/plain", "Here your response to patterns .");
                    const int pattern = jsonData["pattern"];
                    const bool state  = jsonData["state"];
                    log_d("Pattern request for: number = %d , state = %d",pattern,state);
                    
                    // SIMPLIFIED: We just set the global vars.
                    // The LedStrip task handles the rest.
                    ledPatternStrip.patternActive = pattern; 
                    ledPatternStrip.state = state;
                    ledPatternStrip.receivedUpdate = true;
                }
            }
    });

    // REMOVED: The POST /session endpoint is gone.
    /*
    serverFeq.on(
        "/session",
        HTTP_POST,
        ...
    );
    */

    serverFeq.on(
        "/reprovision",
        HTTP_POST,
        // ... (this handler is unchanged)
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
                    request->send(200, "text/plain", "Here your response to reprovisioning .");
                    const char* reprovision = jsonData["reprov"];
                    log_i("Reprovisioning command has been received: %s",reprovision);
                    if(!strcmp(reprovision,"reprovision")){
                        log_w("Proceed to execute erasing of credentials as well as reset.");
                        if(resetCredentials()){
                            log_i("Credentials reset successfully, device will reset in 5 seconds");
                        }
                        else{
                            log_e("Unable to reset credentials, device resetting !");
                        }
                    }
                    else{
                        log_e("Invalid reprovision command :/ : %s",reprovision);;
                    }
                    delay(3000);
                    ESP.restart();
                }
            }
    });

    serverFeq.serveStatic("/", SPIFFS, "/");
    serverFeq.begin();
}

#endif