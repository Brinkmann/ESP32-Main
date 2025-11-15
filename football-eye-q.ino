#include <Arduino.h>
#include <esp_cpu.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp32-hal-log.h"
#include "src/SystemTypes.h"
#include "src/SystemSettings.h"
#include "src/DeviceModel.h"


/**
 * @note Libraries used and versions.
 * 
 * Used library        Version Path                                                                                         
 * SPIFFS              2.0.0   C:\Home\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SPIFFS
 * FS                  2.0.0   C:\Home\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\FS    
 * ArduinoJson         6.21.3  C:\Home\Documents\Arduino\libraries\ArduinoJson                                      
 * WiFi                2.0.0   C:\Home\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\WiFi  
 * ESP Async WebServer 1.2.3   C:\Home\Documents\Arduino\libraries\ESPAsyncWebServer                                
 * AsyncTCP            1.1.4   C:\Home\Documents\Arduino\libraries\AsyncTCP                                         
 * FastLED             3.6.0   C:\Home\Documents\Arduino\libraries\FastLED 
 * 
 */


/**
 * @brief To erase all flash i.e 
 * 
 *   esptool --port COM5 --chip esp32s3 erase_flash
 * 
 */

#ifndef MAIN_LOOP_SLEEP_TIME
#define MAIN_LOOP_SLEEP_TIME 2000 
#endif


FeqDeviceModelType* device = nullptr;

static bool isDeviceProvisioned(CredentialsType* cred){  
  if(cred == nullptr) return false;
  if(!strcmp(cred->ssid,SSID_PLACE_HOLDER_STR) ||  
     !strcmp(cred->pwrd,PWRD_PLACE_HOLDER_STR)){
      return false;
  }
  return true;
}

static void setDeviceMcuSpeed(void){

  setCpuFrequencyMhz(ESP32_CLOCK_SPEED_WIFI_BLE);
  log_i("System clock frequency %d MHz",getCpuFrequencyMhz());

}

static void disableBrownOutReset(void){

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

}

static void initDeviceModel(void){

  static FeqDeviceModelType deviceModel;
  memset(&deviceModel,0,sizeof(FeqDeviceModelType));
  device = &deviceModel;

}

static void initDeviceHardware(void){

  disableBrownOutReset();
  setDeviceMcuSpeed();

}

static void initFileStorage(void){

  static FileStorage fileStorage;
  device->fileStorage = &fileStorage;
  device->fileStorage->init();

}

static void initLedStrip(void){

#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER
  if(device->provisioned){

    static LedStrip ledStrip;
    device->ledStrip = &ledStrip;
    device->ledStrip->init();

  }
#else

  static LedStrip ledStrip;
  device->ledStrip = &ledStrip;
  device->ledStrip->init();

#endif

}

static void initWebServer(CredentialsType* cred){

  static FeqWebServer feqWebServer;
  device->webServer = &feqWebServer;
  device->webServer->init(cred);
  device->wifiManager = nullptr;

}

static void initWiFiManager(void){

  static WiFiManager wifiManager;
  device->wifiManager = &wifiManager;
  device->wifiManager->init();
  device->webServer = nullptr;

}

static void setDeviceControllerOperationMode(void){

  CredentialsType* cred = device->fileStorage->getCredentials();

  if(isDeviceProvisioned(cred)){

    log_i("FEQ operating as a controller.");
    log_i("FEQ Access point provisioned with: ");
    log_i("SSID:  %s , length = %d",cred->ssid,cred->ssidLength);
    log_i("Password: %s , length = %d",cred->pwrd,cred->pwrdLength);
    device->fileStorage->readFilesPatterns();
    initWebServer(cred);
    device->provisioned = true;

  }
  else{

    log_i("Device starting as a WiFi Manager for provisioning.");
    log_i("Default -> AP-SSID = %s , AP-PWRD = %s",
          DEVICE_WIFI_MANAGER_SSID,
          DEVICE_WIFI_MANAGER_PWRD);
    initWiFiManager();
    device->provisioned = false;

  }

}

static void meshNetworkInit(void){

#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER

  if(device->provisioned){

    static MeshNetwork network;
    device->network = &network;
    device->network->init();

  }
#else 

    static MeshNetwork network;
    device->network = &network;
    device->network->init();

#endif


}

void setup(void){

  initDeviceHardware();
  initDeviceModel();
#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER
  initFileStorage();
  setDeviceControllerOperationMode();
#endif
  initLedStrip();
  meshNetworkInit();

}

void loop(void){
  vTaskDelay(pdMS_TO_TICKS(MAIN_LOOP_SLEEP_TIME));
}